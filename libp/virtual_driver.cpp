// virtual_driver.cpp
//
// A Bus Driver for the CallCommander Virtual Protocol
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

#include "virtual_driver.h"

VirtualDriver::VirtualDriver(unsigned id,QObject *parent,const char *name)
  : BusDriver(id,parent,name)
{
  bus_ptr=0;
  bus_console=-1;
  bus_watchdog_running=false;

  //
  // The Network Socket
  //
  bus_socket=new Q3Socket();
  QObject::connect(bus_socket,SIGNAL(hostFound()),this,SLOT(hostFoundData()));
  QObject::connect(bus_socket,SIGNAL(connected()),this,SLOT(connectedData()));
  QObject::connect(bus_socket,SIGNAL(connectionClosed()),
	  this,SLOT(connectionClosedData()));
  bus_socket_timer=new QTimer(this,"bus_socket_timer");
  QObject::connect(bus_socket_timer,SIGNAL(timeout()),
		   this,SLOT(timeoutData()));

  //
  // Timers
  //
  QTimer *timer=new QTimer(this,"read_timer");
  QObject::connect(timer,SIGNAL(timeout()),this,SLOT(readyReadData()));
  timer->start(100);

  bus_watchdog_timer=new QTimer(this,"bus_watchdog_timer");
  QObject::connect(bus_watchdog_timer,SIGNAL(timeout()),
		   this,SLOT(watchdogData()));

  bus_heartbeat_timer=new QTimer(this,"bus_heartbeat_timer");
  QObject::connect(bus_heartbeat_timer,SIGNAL(timeout()),
		   this,SLOT(heartbeatData()));
}


VirtualDriver::~VirtualDriver()
{
  delete bus_socket;
}


QString VirtualDriver::deviceText()
{
  return QString("CallCommander Virtual Phone System");
}


QString VirtualDriver::defaultUserName()
{
  return QString();
}


QString VirtualDriver::defaultUserPassword()
{
  return QString();
}


BusDriver::ConnectionType VirtualDriver::connectionType()
{
  return BusDriver::TcpConnection;
}


Q_INT16 VirtualDriver::defaultTcpPort()
{
  return ML_VIRTUAL_TCP_PORT;
}


QString VirtualDriver::escapeString(const QString &str)
{
  QString ret;

  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {
	case ' ':
	case '!':
	case '\\':
	  ret+="\\";
	  ret+=str.at(i);
	  break;

	default:
	  ret+=str.at(i);
	  break;
    }
  }
  return ret;
}


QString VirtualDriver::restoreString(const QString &str)
{
  bool esc=false;
  QString ret;

  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {    
	case '\\':
	  if(esc) {
	    ret+="\\";
	    esc=false;
	  }
	  else {
	    esc=true;
	  }
	  break;

      default:
	ret+=str.at(i);
	esc=false;
	break;
    }
  }

  return ret;
}


QString VirtualDriver::stripQuotes(QString str)
{
  if(str.left(1)=="\"") {
    str=str.right(str.length()-1);
  }
  if(str.right(1)=="\"") {
    str=str.left(str.length()-1);
  }
  return str;
}


int VirtualDriver::totalConnectionSteps()
{
  return 3;
}


void VirtualDriver::connect(const QString &host_name,const QString &user_name,
			    const QString &user_passwd,Q_UINT16 port,
			    const QString &show_name,
			    const QString &show_passwd,
			    const QString &studio_name,int console,
			    const QString &console_name)
{
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;
  bus_show_name=show_name;
  bus_show_passwd=show_passwd;
  bus_studio_name=studio_name;
  bus_console=console;
  bus_console_name=console_name;
  BusDriver::connect(host_name,user_name,user_passwd,port,show_name,
		     show_passwd,studio_name,console,console_name);
  watchdogData();
}


void VirtualDriver::connect(const QString &tty_port,
			    const QString &show_name,
			    const QString &show_passwd,
			    const QString &studio_name,int console,
			    const QString &console_name)
{
}


void VirtualDriver::connectToHost(const QString &host_name,
				  const QString &user_name,
				  const QString &user_passwd,Q_UINT16 port)
{
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;

  emit connectionStepChanged(id(),1);
  bus_socket->connectToHost(bus_hostname,bus_port);
  bus_socket_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
  BusDriver::connectToHost(host_name,user_name,user_passwd,port);
}


void VirtualDriver::disconnect()
{
  bus_socket_timer->stop();
  bus_heartbeat_timer->stop();
  bus_watchdog_timer->stop();
  bus_socket->close();
  emit connectionClosed(id());
}


void VirtualDriver::requestShowList()
{
  SendCommand("SL!");
}


bool VirtualDriver::connectToShow(const QString &show_name,
				  const QString &show_passwd,
				  const QString &studio_name)
{
  bus_show_name=show_name;
  bus_show_passwd=show_passwd;
  bus_studio_name=studio_name;
  SendCommand(QString().sprintf("LS %s \"%s\" %s!",
		(const char *)VirtualDriver::escapeString(studio_name),
		(const char *)VirtualDriver::escapeString(show_passwd),
		(const char *)VirtualDriver::escapeString(show_name)));
  BusDriver::connectToShow(show_name,show_passwd,studio_name);
  return false;
}


void VirtualDriver::requestConsoleList()
{
  SendCommand("CL!");
}


void VirtualDriver::connectToConsole(int console,const QString &console_name)
{
  bus_console=console;
  bus_console_name=console_name;
  SendCommand(QString().sprintf("LC %d %s!",console,
		    (const char *)VirtualDriver::escapeString(console_name)));
  bus_heartbeat_timer->start(VIRTUAL_DRIVER_HEARTBEAT_INTERVAL);
  bus_watchdog_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
  BusDriver::connectToConsole(console,console_name);
}


void VirtualDriver::requestLineState()
{
  SendCommand("SS!");
}


void VirtualDriver::selectLine(unsigned line,int console,int bank)
{
  SendCommand(QString().sprintf("LA %u %d %d!",line,bank,console));
}


void VirtualDriver::hold(int bank)
{
  hold(bank,bus_console);
}


void VirtualDriver::hold(int bank,int console)
{
  SendCommand(QString().sprintf("HD %d %d!",bank,console));
}


void VirtualDriver::drop(int bank)
{
  drop(bank,bus_console);
}


void VirtualDriver::drop(int bank,int console)
{
  SendCommand(QString().sprintf("DP %d %d!",bank,console));
}


void VirtualDriver::requestBusyAllState()
{
  SendCommand("BS!");
}


void VirtualDriver::busyAll()
{
  SendCommand("BA!");
}


void VirtualDriver::requestDumpDelayState()
{
  SendCommand("DS!");
}


void VirtualDriver::dumpDelay(bool state)
{
  SendCommand(QString().sprintf("DD %d!",state));
}


void VirtualDriver::requestRecordState()
{
  SendCommand("RS!");
}


void VirtualDriver::toggleRecord()
{
  SendCommand("RD!");
}


void VirtualDriver::takeNext(int bank)
{
  SendCommand(QString().sprintf("NX %d!",bank));
}


void VirtualDriver::sendDtmf(char key)
{
  SendCommand(QString().sprintf("TO %c!",key));
}


void VirtualDriver::flash()
{
  SendCommand("FS!");
}


void VirtualDriver::screen(bool state)
{
  SendCommand(QString().sprintf("SM %d!",state));
}


void VirtualDriver::mute(bool state)
{
  SendCommand(QString().sprintf("MM %d!",state));
}


void VirtualDriver::conference(bool state)
{
  SendCommand(QString().sprintf("CM %d!",state));
}


void VirtualDriver::vip(bool state)
{
  SendCommand(QString().sprintf("VM %d!",state));
}


void VirtualDriver::sendString(BusDriver::Element elem,unsigned line,
			       const QString &str)
{
  if(dumpStrings()) {
    printf("SENT: Line:%u  Elem:%u  String:\"%s\"\n",line,elem,
	   (const char *)str.toAscii());
  }
  SendCommand(QString().sprintf("TX %d %d %s!",line,elem,
       			(const char *)VirtualDriver::escapeString(str)));
}


void VirtualDriver::scanDevice()
{
  readyReadData();
}


void VirtualDriver::hostFoundData()
{
  emit connectionStepChanged(id(),2);
}


void VirtualDriver::connectedData()
{
  bus_socket_timer->stop();
  emit connectionStepChanged(id(),3);
  SendCommand(QString().sprintf("LH %s \"%s\"!",
		(const char *)VirtualDriver::escapeString(bus_username),
		(const char *)VirtualDriver::escapeString(bus_userpasswd)));
}


void VirtualDriver::connectionClosedData()
{
  if(!bus_watchdog_running) {
    bus_watchdog_timer->stop();
    watchdogData();
  }
}


void VirtualDriver::errorData(int err)
{
  printf("errorData(%d)\n",err);
  if(bus_watchdog_running) {
    watchdogData();
  }
}


void VirtualDriver::timeoutData()
{
  if(bus_watchdog_running) {
    watchdogData();
  }
  else {
    emit connected(id(),false);
  }
}


void VirtualDriver::readyReadData()
{
  char buf[256];
  int n;
  bool esc=false;

  if(!bus_socket->isOpen()) {
    return;
  }
  while((n=bus_socket->readBlock(buf,256))>0) {
    buf[n]=0;
    for(int i=0;i<n;i++) {
      if(!iscntrl(buf[i])) {
	bus_buffer[bus_ptr++]=buf[i];
	bus_buffer[bus_ptr]=0;
	switch(buf[i]) {
	    case '\\':
	      esc=!esc;
	      break;

	    case '!':
	      if(esc) {
		esc=false;
	      }
	      else {
		bus_buffer[bus_ptr]=0;
		ParseLine();
	      }
	      break;

	    default:
	      esc=false;
	      break;
	}
	if(bus_ptr==ML_VIRTUAL_MAX_MSG_LENGTH) {
	  bus_ptr=0;
	}
      }
    }
  }
}


void VirtualDriver::watchdogData()
{
  if(!bus_watchdog_running) {
    bus_watchdog_running=true;
    emit watchdogStateChanged(id(),true);
  }
  if(bus_heartbeat_timer->isActive()) {
    bus_heartbeat_timer->stop();
  }
  bus_socket->close();
  bus_socket->connectToHost(bus_hostname,bus_port);
  bus_socket_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
}


void VirtualDriver::heartbeatData()
{
  sendString(BusDriver::HeartbeatGlobalElement,0,"tick");
}


void VirtualDriver::ParseLine()
{
  // printf("RECV: %s\n",bus_buffer);

  bool esc=false;
  std::vector<QString> args;

  args.push_back(QString());
  for(unsigned i=0;i<(strlen(bus_buffer)-1);i++) {
    switch(bus_buffer[i]) {
	case '\\':
	  args.back()+=bus_buffer[i];
	  esc=!esc;
	  break;

	case ' ':
	  if(esc) {
	    args.back()+=bus_buffer[i];
	    esc=false;
	  }
	  else {
	    args.push_back(QString());
	  }
	  break;

	default:
	  args.back()+=bus_buffer[i];
	  break;
    }
  }
  bus_ptr=0;
  if(args.size()<1) {
    return;
  }
  args.back().stripWhiteSpace();

  if((args[0]=="LH")&&(args.size()==4)) {    // Login to Host
    if(args[3]=="+") {
      bus_clients[0]->setIpAddress(bus_socket->address());
      bus_clients[0]->setTcpPort(bus_socket->port());
      if(bus_watchdog_running) {
	SendCommand(QString().sprintf("LS %s \"%s\" %s!",
		    (const char *)VirtualDriver::escapeString(bus_studio_name),
		    (const char *)VirtualDriver::escapeString(bus_show_passwd),
		    (const char *)VirtualDriver::escapeString(bus_show_name)));
      }
      else {
	emit connected(id(),true);
      }
    }
    else {
      if(bus_watchdog_running) {
	watchdogData();
      }
      else {
	emit connected(id(),false);
      }
    }
    return;
  }

  if(args[0]=="SL") {    // Return Show List
    if(args.size()==4) {
      bus_show_data.
	push_back(new ShowData(args[3],args[1],args[2].toInt(),true));
    }
    if(args.size()==1) {
      emit currentShowList(id(),&bus_show_data);
      for(unsigned i=0;i<bus_show_data.size();i++) {
	delete bus_show_data[i];
      }
      bus_show_data.clear();
    }
    return;
  }

  if((args[0]=="LS")&&(args.size()==5)) {   // Login to Show
    if(args[4]=="+") {
      if(bus_watchdog_running) {
	SendCommand(QString().sprintf("LC %d %s!",bus_console,
		(const char *)VirtualDriver::escapeString(bus_console_name)));
      }
      else {
	emit currentShow(id(),VirtualDriver::restoreString(args[3]),true);
      }
    }
    else {
      if(bus_watchdog_running) {
	watchdogData();
      }
      else {
	emit currentShow(id(),VirtualDriver::restoreString(args[3]),false);
      }
    }
    return;
  }

  if(args[0]=="CL") {    // Return Console List
    RollupArgs(&args,4);
    if(args.size()>=5) {
      bus_console_data.
	push_back(new ConsoleData(VirtualDriver::restoreString(args[4]),
				  args[1].toInt(),
				  (ConsoleData::Mode)args[2].toInt(),
				  args[3].toInt()));
    }
    if(args.size()==1) {
      emit currentConsoleList(id(),&bus_console_data);
      for(unsigned i=0;i<bus_console_data.size();i++) {
	delete bus_console_data[i];
      }
      bus_console_data.clear();
    }
    return;
  }

  if((args[0]=="LC")&&(args.size()==4)) {   // Connect to Console
    if(args[3]=="+") {
      if(bus_clients[0]->consoleMode()==ConsoleData::Unavailable) {
	bus_clients[0]->setConsoleMode((ConsoleData::Mode)args[2].toInt());
      }
      if(bus_console<0) {
	bus_console=args[1].toInt();
      }
      if(bus_console==args[1].toInt()) {
	emit currentConsole(id(),args[1].toInt(),
			    (ConsoleData::Mode)args[2].toInt());
      }
      if(bus_watchdog_running) {
	bus_watchdog_running=false;
	emit watchdogStateChanged(id(),false);
	bus_heartbeat_timer->start(VIRTUAL_DRIVER_HEARTBEAT_INTERVAL);
	bus_watchdog_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
	updateConnectionState(true);
      }
    }
    else {
      if(bus_watchdog_running) {
	watchdogData();
      }
    }
  }

  if((args[0]=="SS")&&(args.size()==5)) {   // Update Line State
    emit currentLineState(id(),(BusDriver::LineState)args[2].toInt(),
			  args[1].toUInt(),args[3].toInt(),args[4].toInt());
  }

  if((args[0]=="BA")&&(args.size()==2)) {   // Update Busy All State
    if(args[1]=="+") {
      emit currentBusyAllState(id(),true);
    }
    else {
      emit currentBusyAllState(id(),false);
    }
  }

  if((args[0]=="DD")&&(args.size()==2)) {   // Update Delay Dump State
    if(args[1]=="+") {
      emit currentDumpDelayState(id(),true);
    }
    else {
      emit currentDumpDelayState(id(),false);
    }
  }

  if((args[0]=="RD")&&(args.size()==2)) {   // Update Record State
    if(args[1]=="+") {
      emit currentRecordState(id(),true);
    }
    else {
      emit currentRecordState(id(),false);
    }
  }

  if((args[0]=="SM")&&(args.size()==2)) {   // Update Screen Mode State
    if(args[1]=="+") {
      emit currentScreenState(id(),true);
    }
    else {
      emit currentScreenState(id(),false);
    }
  }

  if((args[0]=="MM")&&(args.size()==2)) {   // Update Mute Mode State
    if(args[1]=="+") {
      emit currentMuteState(id(),true);
    }
    else {
      emit currentMuteState(id(),false);
    }
  }

  if((args[0]=="CM")&&(args.size()==2)) {   // Update Conference Mode State
    if(args[1]=="+") {
      emit currentConferenceState(id(),true);
    }
    else {
      emit currentConferenceState(id(),false);
    }
  }

  if((args[0]=="VM")&&(args.size()==2)) {   // Update Vip Mode State
    if(args[1]=="+") {
      emit currentVipState(id(),true);
    }
    else {
      emit currentVipState(id(),false);
    }
  }

  if((args[0]=="TX")&&(args.size()>=4)) {   // Process Text String
    RollupArgs(&args,3);
    if((BusDriver::Element)args[2].toInt()==
       BusDriver::HeartbeatGlobalElement) {
      if(bus_watchdog_timer->isActive()) {
	bus_watchdog_timer->stop();
	bus_watchdog_timer->start(VIRTUAL_DRIVER_WATCHDOG_TIMEOUT,true);
      }
    }
    processString((BusDriver::Element)args[2].toInt(),args[1].toUInt(),
		  VirtualDriver::restoreString(args[3]));
  }
}


void VirtualDriver::SendCommand(const QString &cmd)
{
  // printf("Sending: %s\n",(const char *)cmd);
  bus_socket->writeBlock(cmd,cmd.length());
}


void VirtualDriver::RollupArgs(std::vector<QString> *args,unsigned n)
{
  for(unsigned i=n+1;i<args->size();i++) {
    args->at(n)+=(" "+args->at(i));
  }
}


#ifdef WIN32
int VirtualDriver::iscntrl(int c)
{
  if(c<32) {
    return 1;
  }
  return 0;
}
#endif  // WIN32
