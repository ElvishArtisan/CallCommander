// ts612_driver.cpp
//
// A CallCommander Bus Protocol Driver for Gentner TS-612 Systems.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as
//   published by the Free Software Foundation; either version 2 of
//   the License, or (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include "ml.h"
#include "ts612_driver.h"

Ts612Driver::Ts612Driver(unsigned id,QObject *parent,const char *name)
  : BusDriver(id,parent,name)
{
  bus_tty_device=new MLTTYDevice(this);
  bus_tty_device->setSpeed(TS612_TTY_SPEED);
  bus_console=ML_MAX_CONSOLES;
  bus_record_state=false;
  bus_busy_state=false;
  bus_screen_state=false;
  bus_mute_state=false;
  bus_conf_state=false;
  bus_vip_state=false;
  for(int i=0;i<TS612_MAX_LINES;i++) {
    bus_state[i]='F';
  }
  for(int i=0;i<32;i++) {
    bus_raw_button[i]=255;
  }

  //
  // Dump Release Timer
  //
  bus_dump_timer=new QTimer(this,"bus_dump_timer");
  QObject::connect(bus_dump_timer,SIGNAL(timeout()),this,SLOT(dumpData()));

  //
  // Watchdog Timer
  //
  bus_watchdog_timer=new QTimer(this,"bus_watchdog_timer");
  QObject::connect(bus_watchdog_timer,SIGNAL(timeout()),
		   this,SLOT(watchdogData()));
  bus_watchdog_timer->start(TS612_WATCHDOG_INTERVAL);
}


Ts612Driver::~Ts612Driver()
{
  delete bus_tty_device;
}


QString Ts612Driver::deviceText()
{
  return "Comrex/Gentner TS-612";
}


BusDriver::ConnectionType Ts612Driver::connectionType()
{
  return BusDriver::SerialConnection;
}


int Ts612Driver::totalConnectionSteps()
{
  return 2;
}


void Ts612Driver::connect(const QString &host_name,const QString &user_name,
			  const QString &user_passwd,Q_UINT16 port,
			  const QString &show_name,const QString &show_passwd,
			  const QString &studio_name,int console,
			  const QString &console_name)
{
}


void Ts612Driver::connect(const QString &tty_port,
			  const QString &show_name,const QString &show_passwd,
			  const QString &studio_name,int console,
			  const QString &console_name)
{
  bus_tty_device->setName(tty_port);
  bus_console=console;
  BusDriver::connect(tty_port,show_name,show_passwd,studio_name,
		     console,console_name);
  watchdogData();
}


void Ts612Driver::connectToSerial(const QString &tty_port)
{
  bus_tty_device->setName(tty_port);
  BusDriver::connectToSerial(tty_port);
  emit connected(id(),bus_tty_device->open(QIODevice::ReadWrite));
}


void Ts612Driver::disconnect()
{
  bus_watchdog_timer->stop();
  bus_dump_timer->stop();
  bus_tty_device->close();
  updateConnectionState(false);
  emit connectionClosed(id());
}


void Ts612Driver::requestShowList()
{
  std::vector<ShowData *> show_data;
  show_data.push_back(new ShowData("Hybrid1&2","TS-612",true,false));
  emit currentShowList(id(),&show_data);
  delete show_data.back();
}


bool Ts612Driver::connectToShow(const QString &show_name,
				const QString &show_passwd,
				const QString &studio_name)
{
  BusDriver::connectToShow(show_name,show_passwd,studio_name);
  emit currentShow(id(),show_name,true);
  return true;
}


void Ts612Driver::requestConsoleList()
{
  std::vector<ConsoleData *> console_data;
  console_data.push_back(new ConsoleData("On Air",ML_MAX_CONSOLES,
					 ConsoleData::Talent,true));
  emit currentConsoleList(id(),&console_data);
  delete console_data.back();
}


void Ts612Driver::connectToConsole(int console,const QString &console_name)
{
  bus_console=console;
  QTimer *timer=new QTimer(this,"tty_timer");
  QObject::connect(timer,SIGNAL(timeout()),this,SLOT(receiveTty()));
  timer->start(TS612_TTY_INTERVAL);
  BusDriver::connectToConsole(console,console_name);
  if(console==ML_MAX_CONSOLES) {
    bus_clients[0]->setConsoleMode(ConsoleData::Talent);
    emit currentConsole(id(),console,ConsoleData::Talent);
  }
  else {
    bus_clients[0]->setConsoleMode(ConsoleData::Producer);
    emit currentConsole(id(),console,ConsoleData::Producer);
  }
  updateConnectionState(true);
}


void Ts612Driver::requestLineState()
{
  for(int i=0;i<TS612_MAX_LINES;i++) {
    SendState(i);
  }
}
 

void Ts612Driver::selectLine(unsigned line,int console,int bank)
{
  if(((bus_state[line]=='F')||
      (bus_state[line]=='H')||
      (bus_state[line]=='R')||
      (bus_state[line]=='S'))&&(!bus_busy_state)) {
    if(!bus_conf_state) {
      bus_tty_device->writeBlock(QString().sprintf("%c%c",0x00,0x01),2);
    }
    if(line<6) {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c",16+32*line,17+32*line),2);
    }
    else {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c",24+32*(line-6),25+32*(line-7)),2);
    }
  }
  if((bus_state[line]=='N')&&(!bus_busy_state)&&bus_conf_state) {
    if(line<6) {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c",16+32*line,17+32*line),2);
    }
    else {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c",24+32*(line-6),25+32*(line-7)),2);
    }	    
  }
}


void Ts612Driver::hold(int bank)
{
  hold(bank,bus_console);
}


void Ts612Driver::hold(int bank,int console)
{
  for(int i=0;i<TS612_MAX_LINES;i++) {
    if(bus_state[i]=='N') {
      bus_tty_device->writeBlock(QString().sprintf("%c%c",0x20,0x21),2);
    }
  }
}


void Ts612Driver::drop(int bank)
{
  drop(bank,bus_console);
}


void Ts612Driver::drop(int bank,int console)
{
  for(int i=0;i<TS612_MAX_LINES;i++) {
    if(bus_state[i]=='N') {
      bus_tty_device->writeBlock(QString().sprintf("%c%c",0x00,0x01),2);
    }
  }
}


void Ts612Driver::requestBusyAllState()
{
  emit currentBusyAllState(id(),false);
}


void Ts612Driver::busyAll()
{
  if(bus_busy_state) {
    for(int i=0;i<(TS612_MAX_LINES/2);i++) {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c%c%c",16+32*i,17+32*i,0x01,0x00),4);
#ifndef WIN32
      usleep(TS612_TTY_DELAY);
#endif
      bus_tty_device->writeBlock(QString().sprintf("%c%c",24+32*i,25+32*i),4);
#ifndef WIN32
      usleep(TS612_TTY_DELAY);
#endif
    }
    bus_busy_state=false;
    emit currentBusyAllState(id(),false);
  }
  else {
    for(int i=0;i<(TS612_MAX_LINES/2);i++) {
      bus_tty_device->
	writeBlock(QString().sprintf("%c%c%c%c",16+32*i,17+32*i,0x21,0x20),4);
#ifndef WIN32
      usleep(TS612_TTY_DELAY);
#endif
    }
    bus_busy_state=true;
    emit currentBusyAllState(id(),true);
  }
}


void Ts612Driver::requestDumpDelayState()
{
  emit currentDumpDelayState(id(),bus_dump_timer->isActive());
}


void Ts612Driver::dumpDelay(bool state)
{
  if(state) {
    bus_tty_device->writeBlock(QString().sprintf("%c%c",0xA8,0xA9),2);
    emit currentDumpDelayState(id(),true);
    bus_dump_timer->start(TS612_DUMP_LENGTH,true);
  }
}


void Ts612Driver::requestRecordState()
{
}


void Ts612Driver::toggleRecord()
{
  bus_tty_device->writeBlock(QString().sprintf("%c%c",0x08,0x09),2);
  bus_record_state=!bus_record_state;
  emit currentRecordState(id(),bus_record_state);
}


void Ts612Driver::takeNext(int bank)
{
  bus_tty_device->writeBlock(QString().sprintf("%c%c",0x40,0x41),2);
}


void Ts612Driver::sendDtmf(char key)
{
  char alt=0;

  switch(key) {
      case '0':
	alt=0x8D;
	break;

      case '1':
	alt=0x05;
	break;
	
      case '2':
	alt=0x25;
	break;
	
      case '3':
	alt=0x45;
	break;
	
      case '4':
	alt=0x0D;
	break;
	
      case '5':
	alt=0x2D;
	break;
	
      case '6':
	alt=0x4D;
	break;
	
      case '7':
	alt=0x65;
	break;
	
      case '8':
	alt=0x85;
	break;
	
      case '9':
	alt=0xA5;
	break;
	
      case '*':
	alt=0x6D;
	break;
	
      case '#':
	alt=0xAD;
	break;
  }
  bus_tty_device->writeBlock(QString().sprintf("%c%c",alt,alt-1),2);
}


void Ts612Driver::flash()
{
  bus_tty_device->writeBlock(QString().sprintf("%c%c",0x68,0x69),2);
}


void Ts612Driver::screen(bool state)
{
  if((state&&(!bus_screen_state))||((!state)&&(bus_screen_state))) {
    bus_tty_device->writeBlock(QString().sprintf("%c%c",0x60,0x61),2);
    bus_screen_state=!bus_screen_state;
    emit currentScreenState(id(),bus_screen_state);
  }
}


void Ts612Driver::mute(bool state)
{
  if((state&&(!bus_mute_state))||((!state)&&(bus_mute_state))) {
    bus_tty_device->writeBlock(QString().sprintf("%c%c",0x28,0x29),2);
    bus_mute_state=!bus_mute_state;
    emit currentMuteState(id(),bus_mute_state);
  }
}


void Ts612Driver::conference(bool state)
{
  if((state&&(!bus_conf_state))||((!state)&&(bus_conf_state))) {
    bus_tty_device->writeBlock(QString().sprintf("%c%c",0x48,0x49),2);
    bus_conf_state=!bus_conf_state;
    emit currentConferenceState(id(),bus_conf_state);
  }
}


void Ts612Driver::vip(bool state)
{
  if((state&&(!bus_vip_state))||((!state)&&(bus_vip_state))) {
    bus_tty_device->writeBlock(QString().sprintf("%c%c",0x80,0x81),2);
    bus_vip_state=!bus_vip_state;
    emit currentVipState(id(),bus_conf_state);
  }
}


void Ts612Driver::sendString(BusDriver::Element elem,unsigned line,
				const QString &str)
{
}


void Ts612Driver::scanDevice()
{
}


void Ts612Driver::receiveTty()
{
  unsigned char buffer[256];
  int n;
  int row;
  int col;
  int led;

  n=bus_tty_device->readBlock((char *)buffer,256);
  for(int i=0;i<n;i++) {
    if(bus_raw_button[buffer[i]>>3]!=(buffer[i]&0x07)) {
      led=buffer[i]&0x07;
      row=(buffer[i]>>3)&0x03;
      col=(buffer[i]>>5)&0x07;
      switch(row) {
	  case 0:  // Miscelaneous Keys
	    switch(col) {	 
		case 3:  // SCRN
		  bus_screen_state=(led!=0);
		  emit currentScreenState(id(),bus_screen_state);
		  break;

		case 4:  // VIP
		  bus_vip_state=(led!=0);
		  emit currentVipState(id(),bus_vip_state);
		  break;
	    }
	    break;

	  case 1:  // Miscelaneous Keys
	    switch(col) {	 
		case 0:  // REC
		  emit currentRecordState(id(),led!=0);
		  break;

		case 1:  // MUTE
		  bus_mute_state=(led!=0);
		  emit currentMuteState(id(),bus_mute_state);
		  break;

		case 2:  // CONF
		  bus_conf_state=(led!=0);
		  emit currentConferenceState(id(),bus_conf_state);
		  break;
	    }
	    break;

	  case 2:  // Line Buttons 1 - 6
	  case 3:  // Line Buttons 7 - 12
	    switch(led) {
		case 0:  // Idle
		  DispatchStatus('F',6*(row-2)+col);
		  break;
		case 1:  // On Air
		  DispatchStatus('N',6*(row-2)+col);
		  break;
		case 2:  // Off Hook at Screener
		  DispatchStatus('O',6*(row-2)+col);
		  break;
		case 3:  // Off Hook Elsewhere
		  DispatchStatus('E',6*(row-2)+col);
		  break;
		case 4:  // RED@1 Hz (Not Used)
		  break;
		case 5:  // Screened Hold
		  DispatchStatus('S',6*(row-2)+col);
		  break;
		case 6:  // On Hold
		  if(bus_busy_state) {
		    DispatchStatus('B',6*(row-2)+col);
		  }
		  else {
		    DispatchStatus('H',6*(row-2)+col);
		  }
		  break;
		case 7:  // Ringing
		  DispatchStatus('R',6*(row-2)+col);
		  break;
	    }
	    break;
      }
      bus_raw_button[buffer[i]>>3]=buffer[i]&0x07;
    }
  }
}


void Ts612Driver::dumpData()
{
  emit currentDumpDelayState(id(),false);
}


void Ts612Driver::watchdogData()
{
  requestLineState();
}


void Ts612Driver::DispatchStatus(int cmd,int line)
{
  switch(cmd) {
  case 'U':   // Console function buttons
    switch(line) {
	case 0:  // The Busyout Button
	  break;
	  
	case 1:  // The Flash Button
	  break;
	  
	case 2:  // The Start Record Button
	  emit currentRecordState(id(),true);
	  break;
	  
	case 3:  // The Stop Record Button
	  emit currentRecordState(id(),false);
	  break;
	  
	case 4:  // The DelayDump Button
	  emit currentDumpDelayState(id(),true);
	  break;
    }
    return;

      case 'N':
	if((cmd!=bus_state[line])) {
	  bus_state[line]=cmd;
	  SendState(line);
	}
	break;

      case 'C':
	if((cmd!=bus_state[line])) {
	  bus_state[line]=cmd;
	  SendState(line);
	}
	break;

      default:
	if(cmd!=bus_state[line]) {
	  bus_state[line]=cmd;
	  SendState(line);
	}
	break;
  }
}


void Ts612Driver::SendState(int line)
{
  switch(bus_state[line]) {
      case 'F':    // Off
	emit currentLineState(id(),BusDriver::LineIdle,line,255,255);
	break;

      case 'N':    // On Main Bank
	emit currentLineState(id(),BusDriver::LineOnAir,line,0,255);
	break;

      case 'C':    // On Conference Bank
	emit currentLineState(id(),BusDriver::LineOnAir,line,1,255);
	break;

      case 'H':    // On Hold
	if(bus_busy_state) {
	  emit currentLineState(id(),BusDriver::LineBusied,line,255,255);
	}
	else {
	  emit currentLineState(id(),BusDriver::LineHold,line,255,255);
	}
	break;

      case 'S':    // On Screened Hold	
	emit currentLineState(id(),BusDriver::LineScreenedHold,line,255,255);
	break;

      case 'B':    // Busied Out
	emit currentLineState(id(),BusDriver::LineBusied,line,255,255);
	break;

      case 'O':    // Off Hook at Screener
	emit currentLineState(id(),BusDriver::LineHandset,line,255,0);
	break;

      case 'E':    // Off Hook Elsewhere
	emit currentLineState(id(),BusDriver::LineElsewhere,line,255,255);
	break;

      case 'R':    // Ringing
	emit currentLineState(id(),BusDriver::LineRinging,line,255,255);
	break;
  }
}
