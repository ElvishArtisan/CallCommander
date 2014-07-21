// telos100_driver.cpp
//
// A CallCommander Bus Protocol Driver for Telos 100 Phone Systems
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
#include "telos100_driver.h"

Telos100Driver::Telos100Driver(Telos100Driver::SubType type,unsigned id,
			       QObject *parent,const char *name)
  : BusDriver(id,parent,name)
{
  bus_subtype=type;
  bus_tty_device=new MLTTYDevice(this);
  bus_tty_device->setSpeed(TELOS100_TTY_SPEED);
  QObject::connect(bus_tty_device,SIGNAL(readyRead()),this,SLOT(receiveTty()));
  bus_console=ML_MAX_CONSOLES;
  bus_cmd='U';
  bus_istate=0;
  bus_record_state=false;
  bus_states=new Telos100States();
  bus_watchdog_active=false;

  //
  // Dump Timer
  //
  bus_dump_timer=new QTimer(this,"bus_dump_timer");
  QObject::connect(bus_dump_timer,SIGNAL(timeout()),this,SLOT(dumpData()));

  //
  // DTMF Queue Timer
  //
  bus_dtmf_key_timer=new QTimer(this,"bus_dtmf_key_timer");
  QObject::connect(bus_dtmf_key_timer,SIGNAL(timeout()),
		   this,SLOT(dtmfKeyTimeoutData()));
}


Telos100Driver::~Telos100Driver()
{
  delete bus_states;
  delete bus_tty_device;
}


QString Telos100Driver::deviceText()
{
  return "Telos 100";
}


QString Telos100Driver::deviceText(Telos100Driver::SubType type)
{
  switch(type) {
      case Telos100Driver::KeyType:
	return "Telos 100 1A2 Key Module";

      case Telos100Driver::DimType:
	return "Telos 100 Direct Interface Module";

      case Telos100Driver::OneXSixType:
	return "Telos ONEx6 Phone System";
  }
  return "Telos 100";
}


BusDriver::ConnectionType Telos100Driver::connectionType()
{
  return BusDriver::SerialConnection;
}


int Telos100Driver::totalConnectionSteps()
{
  return 2;
}


void Telos100Driver::connect(const QString &host_name,const QString &user_name,
			     const QString &user_passwd,Q_UINT16 port,
			     const QString &show_name,const QString &show_passwd,
			     const QString &studio_name,int console,
			     const QString &console_name)
{
}


void Telos100Driver::connect(const QString &tty_port,
			     const QString &show_name,
			     const QString &show_passwd,
			     const QString &studio_name,int console,
			     const QString &console_name)
{
  bus_tty_device->setName(tty_port);
  bus_console=console;
  BusDriver::connect(tty_port,show_name,show_passwd,studio_name,
		     console,console_name);
  if(console==ML_MAX_CONSOLES) {
    bus_clients[0]->setConsoleMode(ConsoleData::Talent);
  }
  else {
    bus_clients[0]->setConsoleMode(ConsoleData::Producer);
  }
  WatchdogRestart();
}


void Telos100Driver::connectToSerial(const QString &tty_port)
{
  bus_tty_device->setName(tty_port);
  BusDriver::connectToSerial(tty_port);
  emit connected(id(),bus_tty_device->open(QIODevice::ReadWrite));
}


void Telos100Driver::disconnect()
{
  bus_tty_device->close();
  updateConnectionState(false);
  emit connectionClosed(id());
}


void Telos100Driver::requestShowList()
{
  std::vector<ShowData *> show_data;
  switch(bus_subtype) {
      case Telos100Driver::KeyType:
	show_data.push_back(new ShowData("Hybrid1&2","1A2Key",true,false));
	break;

      case Telos100Driver::DimType:
	show_data.push_back(new ShowData("Hybrid1&2","DIM",true,false));
	break;

      case Telos100Driver::OneXSixType:
	show_data.push_back(new ShowData("Hybrid1&2","ONEx6",true,false));
	break;
  }
  emit currentShowList(id(),&show_data);
  delete show_data.back();
}


bool Telos100Driver::connectToShow(const QString &show_name,
				   const QString &show_passwd,
				   const QString &studio_name)
{
  BusDriver::connectToShow(show_name,show_passwd,studio_name);
  emit currentShow(id(),show_name,true);
  return true;
}


void Telos100Driver::requestConsoleList()
{
  std::vector<ConsoleData *> console_data;
  switch(bus_subtype) {
      case Telos100Driver::KeyType:
	break;

      case Telos100Driver::DimType:
	console_data.push_back(new ConsoleData("Screener 1",0,
					       ConsoleData::Producer,true));
	console_data.push_back(new ConsoleData("Screener 2",1,
					       ConsoleData::Producer,true));
	break;

      case Telos100Driver::OneXSixType:
	console_data.push_back(new ConsoleData("Screener",0,
					       ConsoleData::Producer,true));
	break;
  }
  console_data.push_back(new ConsoleData("On Air",ML_MAX_CONSOLES,
					 ConsoleData::Talent,true));
  emit currentConsoleList(id(),&console_data);
  for(unsigned i=0;i<console_data.size();i++) {
    delete console_data[i];
  }
}


void Telos100Driver::connectToConsole(int console,const QString &console_name)
{
  bus_console=console;
  bus_tty_device->write("M",1);
  bus_tty_device->write("Q00",3);   // Kickoff Line Autoprobing
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


void Telos100Driver::requestLineState()
{
  for(unsigned i=0;i<bus_states->size();i++) {
    bus_tty_device->write(QString().sprintf("Q%02d",i),3);
  }
}
 

void Telos100Driver::selectLine(unsigned line,int console,int bank)
{
  switch(bank) {
      case 0:   // Main Bank
	switch(console) {
	    case 0:   // Screener 1
	      switch(bus_states->state(line)) {
		  case 'F':
		  case 'H':
		  case 'S':
		  case 'R':
		bus_tty_device->write(QString().sprintf("P%02d",line),3);
	      }
	      break;

	    case 1:   // Screener 2
	      switch(bus_states->state(line)) {
		  case 'F':
		  case 'H':
		  case 'S':
		  case 'R':
		bus_tty_device->write(QString().sprintf("O%02d",line),3);
	      }
	      break;

	    default:  // On Air
	      if((bus_states->state(line)!='N')&&
		 (!bus_states->locked(line))) {
		bus_tty_device->write(QString().sprintf("N%02d",line),3);
	      }
	      else {
		bus_tty_device->write(QString().sprintf("L%02d",line),3);
	      }
	      break;
	}
	break;

      case 1:   // Conference Bank
	switch(console) {
	    case 0:   // Screener 1
	      break;

	    case 1:   // Screener 2
	      break;

	    default:  // On Air
	      if((bus_states->state(line)!='C')&&
		 (!bus_states->locked(line))) {
		bus_tty_device->write(QString().sprintf("C%02d",line),3);
	      }
	      else {
		bus_tty_device->write(QString().sprintf("L%02d",line),3);
	      }
	      break;
	}
	break;
  }
}


void Telos100Driver::hold(int bank)
{
  hold(bank,bus_console);
}


void Telos100Driver::hold(int bank,int console)
{
  switch(console) {
      case 0:   // Screener 1
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(bus_states->state(i)=='P') {
	    if(bank==0) {
	      bus_tty_device->write(QString().sprintf("H%02u",i),3);
	    }
	    else {
	      bus_tty_device->write(QString().sprintf("S%02u",i),3);
	    }
	  }
	}
	break;

      case 1:   // Screener 2
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(bus_states->state(i)=='O') {
	    if(bank==0) {
	      bus_tty_device->write(QString().sprintf("H%02u",i),3);
	    }
	    else {
	      bus_tty_device->write(QString().sprintf("S%02u",i),3);
	    }
	  }
	}
	break;
	
      default:   // On Air
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(!bus_states->locked(i)) {
	    if(((bus_states->state(i)=='N')&&(bank==0))||
	       ((bus_states->state(i)=='C')&&(bank==1))) {
	      bus_tty_device->write(QString().sprintf("H%02u",i),3);
	    }
	  }
	}
  }
}


void Telos100Driver::drop(int bank)
{
  drop(bank,bus_console);
}


void Telos100Driver::drop(int bank,int console)
{
  switch(console) {
      case 0:   // Screener 1
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(bus_states->state(i)=='P') {
	    bus_tty_device->write(QString().sprintf("F%02u",i),3);
	  }
	}
	break;
	
      case 1:   // Screener 2
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(bus_states->state(i)=='O') {
	    bus_tty_device->write(QString().sprintf("F%02u",i),3);
	  }
	}
	break;
	
      default:   // On Air
	for(unsigned i=0;i<bus_states->size();i++) {
	  if(!bus_states->locked(i)) {
	    if(((bus_states->state(i)=='N')&&(bank==0))||
	       ((bus_states->state(i)=='C')&&(bank==1))) {
	      bus_tty_device->write(QString().sprintf("F%02u",i),3);
	    }
	  }
	}
  }
}


void Telos100Driver::requestBusyAllState()
{
  for(unsigned i=0;i<bus_states->size();i++) {
    if(bus_states->state(i)=='B') {
      emit currentBusyAllState(id(),true);
      return;
    }
  }
  emit currentBusyAllState(id(),false);
}


void Telos100Driver::busyAll()
{
  bus_tty_device->write("U00",3);
}


void Telos100Driver::requestDumpDelayState()
{
  emit currentDumpDelayState(id(),bus_dump_timer->isActive());
}


void Telos100Driver::dumpDelay(bool state)
{
  bus_tty_device->write("U04",3);
  emit currentDumpDelayState(id(),true);
  bus_dump_timer->start(TELOS100_DUMP_LENGTH,true);
}


void Telos100Driver::requestRecordState()
{
  emit currentRecordState(id(),bus_record_state);
}


void Telos100Driver::toggleRecord()
{
  if(bus_record_state) {
    bus_tty_device->write("U3",3);
  }
  else {
    bus_tty_device->write("U2",3);
  }
  bus_record_state=!bus_record_state;
  requestRecordState();
}


void Telos100Driver::takeNext(int bank)
{
  if(bus_console==ML_MAX_CONSOLES) {
    bus_tty_device->write("U05",3);
  }
}


void Telos100Driver::sendDtmf(char key)
{
//  printf("Telos100Driver: sending key: %c\n",key);
  bus_dtmf_keys.push(key);
  if(!bus_dtmf_key_timer->isActive()) {
    bus_dtmf_key_timer->start(TELOS100_DTMF_KEY_INTERVAL,true);
  }
}


void Telos100Driver::flash()
{
  bus_tty_device->write("U01",3);
}


void Telos100Driver::screen(bool state)
{
}


void Telos100Driver::mute(bool state)
{
}


void Telos100Driver::conference(bool state)
{
}


void Telos100Driver::vip(bool state)
{
}


void Telos100Driver::sendString(BusDriver::Element elem,unsigned line,
				const QString &str)
{
}


void Telos100Driver::scanDevice()
{
}


void Telos100Driver::receiveTty()
{
  char buffer[256];
  int n;

  n=bus_tty_device->read(buffer,256);
  buffer[n]=0;
  /*
  if(n>0) {
    printf("Telos100Driver Received: %s   istate: %d\n",buffer,bus_istate);
  }
  */
  for(int i=0;i<n;i++) {
    switch(bus_istate) {
	case 0:    // Command Byte
	  if((buffer[i]=='F')||(buffer[i]=='N')||(buffer[i]=='H')||
	     (buffer[i]=='S')||(buffer[i]=='C')||(buffer[i]=='L')||
	     (buffer[i]=='B')||(buffer[i]=='E')||(buffer[i]=='R')||
	     (buffer[i]=='P')||(buffer[i]=='O')||(buffer[i]=='U')) {
	    bus_cmd=buffer[i];
	    bus_istate=1;
	    break;
	  }
	  if(buffer[i]=='I') {   // Could be DIM restart!
	    bus_istate=10;
	    break;
	  }
	  if(buffer[i]=='g') {   // Could be 1A2 restart!
	    bus_istate=20;
	    break;
	  }
	  break;

	case 1:    // First Line Number Byte
	  if(buffer[i]=='0') {
	    bus_istate=2;
	    break;
	  }
	  if(buffer[i]=='I') {   // Could be DIM restart!
	    bus_istate=10;
	    break;
	  }
	  if(buffer[i]=='g') {   // Could be 1A2 restart!
	    bus_istate=20;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 2:   // Second Line Number Byte
	  if((buffer[i]>='0')&&(buffer[i]<='9')) {
	    DispatchStatus(bus_cmd,buffer[i]-'0');
	    bus_istate=0;
	    break;
	  }
	  if(buffer[i]=='I') {   // Could be DIM restart!
	    bus_istate=10;
	    break;
	  }
	  if(buffer[i]=='g') {   // Could be 1A2 restart!
	    bus_istate=20;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 10:  // DIM Restart Sequence
	  if(buffer[i]=='n') {
	    bus_istate=11;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 11:  // DIM Restart Sequence
	  if(buffer[i]=='i') {
	    bus_istate=12;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 12:  // DIM Restart Sequence
	  if(buffer[i]=='t') {
	    WatchdogRestart();
	  }
	  bus_istate=0;
	  break;

	case 20:  // 1A2 Restart Sequence
	  if(buffer[i]=='o') {
	    bus_istate=21;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 21:  // 1A2 Restart Sequence
	  if(buffer[i]=='t') {
	    bus_istate=22;
	    break;
	  }
	  bus_istate=0;
	  break;

	case 22:  // 1A2 Restart Sequence
	  if(buffer[i]=='c') {
	    WatchdogRestart();
	  }
	  bus_istate=0;
	  break;

	default:
	  bus_istate=0;
	  break;
    }
  }
}


void Telos100Driver::dumpData()
{
  emit currentDumpDelayState(id(),false);
}


void Telos100Driver::dtmfKeyTimeoutData()
{
  char key=bus_dtmf_keys.front();
  if(key=='*') {
    bus_tty_device->write("T11",3);
  }
  else {
    if(key=='#') {
      bus_tty_device->write("T12",3);
    }
    else {
      bus_tty_device->write(QString().sprintf("T%02d",key-'0'),3);
    }
  }
  bus_dtmf_keys.pop();
  if(bus_dtmf_keys.size()>0) {
    bus_dtmf_key_timer->start(TELOS100_DTMF_KEY_INTERVAL,true);
  }
}


void Telos100Driver::DispatchStatus(char cmd,int line)
{
  if(bus_states->state(line)=='Z') {  // Autoprobe for next line
    bus_tty_device->write(QString().sprintf("Q%02d",line+1),3);
  }

  if(bus_watchdog_active) {   // Process Watchdog Restart
    bus_watchdog_active=false;
    emit watchdogStateChanged(id(),false);
  }

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
	      bus_dump_timer->start(TELOS100_DUMP_LENGTH,true);
	      break;
	}
	return;

      case 'L':
	bus_states->setLocked(line,true);
	SendState(line);
	break;

      case 'N':
	bus_states->setState(line,'N');
	bus_states->setLocked(line,false);
	SendState(line);
	break;

      case 'C':
	bus_states->setState(line,'C');
	bus_states->setLocked(line,false);
	SendState(line);
	break;

      case 'F':
	bus_states->setState(line,'F');
	bus_states->setLocked(line,false);
	SendState(line);
	break;

      default:
	bus_states->setState(line,cmd);
	SendState(line);
	break;
  }
}


void Telos100Driver::SendState(int line)
{
  switch(bus_states->state(line)) {
      case 'F':    // Off
	emit currentLineState(id(),BusDriver::LineIdle,line,255,255);
	break;

      case 'N':    // On Main Bank
	if(!bus_states->locked(line)) {
	  emit currentLineState(id(),BusDriver::LineOnAir,line,0,8);
	}
	else {
	  emit currentLineState(id(),BusDriver::LineLocked,line,0,8);
	}
	break;

      case 'C':    // On Conference Bank
	if(!bus_states->locked(line)) {
	  emit currentLineState(id(),BusDriver::LineOnAir,line,1,8);
	}
	else {
	  emit currentLineState(id(),BusDriver::LineLocked,line,1,8);
	}
	break;

      case 'L':    // Locked
	switch(bus_states->state(line)) {
	    case 'N':
	      emit currentLineState(id(),BusDriver::LineLocked,line,0,8);
	      break;

	    case 'C':
	      emit currentLineState(id(),BusDriver::LineLocked,line,1,8);
	      break;
	}
	break;

      case 'H':    // On Hold
	emit currentLineState(id(),BusDriver::LineHold,line,255,255);
	break;

      case 'S':    // On Screened Hold	
	emit currentLineState(id(),BusDriver::LineScreenedHold,line,255,255);
	break;

      case 'B':    // Busied Out
	emit currentLineState(id(),BusDriver::LineBusied,line,255,255);
	break;

      case 'P':    // Screener Phone 1 Off Hook
	emit currentLineState(id(),BusDriver::LineHandset,line,255,0);
	break;

      case 'O':    // Screener Phone 2 Off Hook
	emit currentLineState(id(),BusDriver::LineHandset,line,255,1);
	break;

      case 'E':    // Off Hook Elsewhere
	emit currentLineState(id(),BusDriver::LineElsewhere,line,255,255);
	break;

      case 'R':    // Ringing
	emit currentLineState(id(),BusDriver::LineRinging,line,255,255);
	break;
  }
}


void Telos100Driver::WatchdogRestart()
{
  bus_watchdog_active=true;
  emit watchdogStateChanged(id(),true);
#ifndef WIN32
  sleep(TELOS100_WATCHDOG_PAUSE);
#endif  // WIN32
  bus_tty_device->write("M",1);
  for(unsigned i=0;i<bus_states->size();i++) {
    bus_tty_device->write(QString().sprintf("Q%02u",i),3);
  }
}
