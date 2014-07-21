// telos2101_driver.cpp
//
// A CallCommander Bus Driver for the Telos2101/TWOx12
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

#include <Qt3Support/q3socketdevice.h>

#include "ml.h"
#include "telos2101_driver.h"

Telos2101Driver::Telos2101Driver(unsigned id,QObject *parent,const char *name)
  : BusDriver(id,parent,name)
{
  bus_client=NULL;
  bus_current_show_auth=false;
  bus_console_found=false;
  bus_watchdog_active=false;
  bus_hostname="";
  bus_username="";
  bus_userpasswd="";
  bus_socket=NULL;

  //
  // Timers
  //
  bus_message_timer=new QTimer(this,"bus_message_timer");
  QObject::connect(bus_message_timer,SIGNAL(timeout()),
		   this,SLOT(messageScanData()));

  bus_show_timer=new QTimer(this,"bus_show_timer");
  QObject::connect(bus_show_timer,SIGNAL(timeout()),
		   this,SLOT(showTimerData()));

  bus_watchdog_timer=new QTimer(this,"bus_watchdog_timer");
  QObject::connect(bus_watchdog_timer,SIGNAL(timeout()),
		   this,SLOT(watchdogTimerData()));

  bus_heartbeat_timer=new QTimer(this,"bus_heartbeat_timer");
  QObject::connect(bus_heartbeat_timer,SIGNAL(timeout()),
		   this,SLOT(heartbeatTimerData()));

  Init();
}


Telos2101Driver::~Telos2101Driver()
{
}


QString Telos2101Driver::deviceText()
{
  return QString(tr("Telos 2101/TWOx12 Multistudio/Multiline System"));
}


QString Telos2101Driver::defaultUserName()
{
  return QString(TELOS2101_DRIVER_DEFAULT_USER_NAME);
}


QString Telos2101Driver::defaultUserPassword()
{
  return QString(TELOS2101_DRIVER_DEFAULT_USER_PASSWORD);
}


BusDriver::ConnectionType Telos2101Driver::connectionType()
{
  return BusDriver::TcpConnection;
}


Q_INT16 Telos2101Driver::defaultTcpPort()
{
  return TELOS2101_DEFAULT_PORT;
}


int Telos2101Driver::totalConnectionSteps()
{
  return 2;
}


void Telos2101Driver::connect(const QString &host_name,
			      const QString &user_name,
			      const QString &user_passwd,Q_UINT16 port,
			      const QString &show_name,
			      const QString &show_passwd,
			      const QString &studio_name,int console,
			      const QString &console_name)
{
  bus_connected=true;
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;
  bus_showname=show_name;
  bus_showpasswd=show_passwd;
  bus_studioname=studio_name;
  bus_console=console;
  BusDriver::connect(host_name,user_name,user_passwd,port,show_name,
		     show_passwd,studio_name,console,console_name);
  watchdogTimerData();
}


void Telos2101Driver::connect(const QString &tty_port,
			      const QString &show_name,
			      const QString &show_passwd,
			      const QString &studio_name,int console,
			      const QString &console_name)
{
}


void Telos2101Driver::connectToHost(const QString &host_name,
			      const QString &user_name,
			      const QString &user_passwd,Q_UINT16 port)
{
  int fd;

  if(bus_connected) {
    return;
  }
  emit connectionStepChanged(id(),1);
  if(!TlsAP_Connect(bus_client,(const char *)host_name,(const char *)user_name,
		   (const char *)user_passwd,port,&fd)) {
    emit connected(id(),false);
    return;
  }
  if(bus_socket!=NULL) {
    delete bus_socket;
  }
  bus_socket=new Q3SocketDevice();
  bus_socket->setSocket(fd,Q3SocketDevice::Stream);
  bus_clients[0]->setIpAddress(bus_socket->address());
  bus_clients[0]->setTcpPort(bus_socket->port());

  bus_connected=true;
  bus_hostname=host_name;
  bus_username=user_name;
  bus_userpasswd=user_passwd;
  bus_port=port;
  bus_message_timer->start(TELOS2101_DRIVER_TICK_INTERVAL);
  BusDriver::connectToHost(host_name,user_name,user_passwd,port);
  emit connected(id(),true);
  emit connectionStepChanged(id(),2);
  TlsAP_EnumerateShows(bus_client);
}


void Telos2101Driver::disconnect()
{
  bus_heartbeat_timer->stop();
  bus_watchdog_timer->stop();
  bus_message_timer->stop();
  bus_show_timer->stop();
  TlsAP_Disconnect(bus_client);
  Init();
  updateConnectionState(false);
  emit connectionClosed(id());
}


void Telos2101Driver::requestShowList()
{
  if(!bus_connected) {
    return;
  }
  for(unsigned i=0;i<bus_shows.size();i++) {
    delete bus_shows[i];
  }
  bus_shows.clear();
  TlsAP_EnumerateShows(bus_client);
}


bool Telos2101Driver::connectToShow(const QString &show_name,
				    const QString &show_passwd,
				    const QString &studio_name)
{
  bus_current_show_auth=false;
  if(!TlsAP_ConnectToShow(bus_client,TELOS2101_DRIVER_SESSION_ID,
			  (const char *)show_name,
			  (const char *)show_passwd,
			  (const char *)studio_name)) {
    return false;
  }
  bus_showname=show_name;
  bus_showpasswd=show_passwd;
  bus_studioname=studio_name;
  bus_show_timer->start(TELOS2101_DRIVER_SHOW_TIMEOUT,true);
  BusDriver::connectToShow(show_name,show_passwd,studio_name);

  return true;
}


void Telos2101Driver::requestConsoleList()
{
  if(!bus_connected) {
    return;
  }
  for(unsigned i=0;i<bus_consoles.size();i++) {
    delete bus_consoles[i];
  }
  TlsAP_EnumerateDirectors(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::connectToConsole(int console,const QString &console_name)
{
  bus_console=console;
  bus_console_found=false;
  bus_consoles.clear();
  bus_heartbeat_timer->start(TELOS2101_DRIVER_HEARTBEAT_INTERVAL);
  bus_watchdog_timer->start(TELOS2101_DRIVER_WATCHDOG_TIMEOUT,true);
  BusDriver::connectToConsole(console,console_name);
  if(console>=ML_MAX_CONSOLES) {
    TlsAP_AttachAsTalent(bus_client,TELOS2101_DRIVER_SESSION_ID);
    bus_clients[0]->setConsoleMode(ConsoleData::Talent);
    emit currentConsole(id(),console,ConsoleData::Talent);
    return;
  }
  TlsAP_AttachToDirector(bus_client,TELOS2101_DRIVER_SESSION_ID,console_name);
  TlsAP_EnumerateDirectors(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::requestLineState()
{
  TlsAP_PostUpdateAll(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::selectLine(unsigned line,int console,int bank)
{
  TlsAP_PostLineButton(bus_client,TELOS2101_DRIVER_SESSION_ID,line,bank);
}


void Telos2101Driver::hold(int bank)
{
  TlsAP_PostHoldButton(bus_client,TELOS2101_DRIVER_SESSION_ID,bank);
}


void Telos2101Driver::hold(int bank,int console)
{
  hold(bank);
}


void Telos2101Driver::drop(int bank)
{
  TlsAP_PostDropButton(bus_client,TELOS2101_DRIVER_SESSION_ID,bank);
}


void Telos2101Driver::drop(int bank,int console)
{
  drop(bank);
}


void Telos2101Driver::requestBusyAllState()
{
}


void Telos2101Driver::busyAll()
{
  TlsAP_PostBusyAllButton(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::requestDumpDelayState()
{
}


void Telos2101Driver::dumpDelay(bool state)
{
  TlsAP_PostDelayDumpButton(bus_client,TELOS2101_DRIVER_SESSION_ID,state);
}


void Telos2101Driver::requestRecordState()
{
}


void Telos2101Driver::toggleRecord()
{
  TlsAP_PostRecordButton(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::takeNext(int bank)
{
  TlsAP_PostNextButton(bus_client,TELOS2101_DRIVER_SESSION_ID);
}


void Telos2101Driver::sendDtmf(char key)
{
  TlsAP_PostDialKey(bus_client,TELOS2101_DRIVER_SESSION_ID,key);
}


void Telos2101Driver::flash()
{
}


void Telos2101Driver::screen(bool state)
{
}


void Telos2101Driver::mute(bool state)
{
}


void Telos2101Driver::conference(bool state)
{
}


void Telos2101Driver::vip(bool state)
{
}


void Telos2101Driver::sendString(BusDriver::Element elem,unsigned line,
				 const QString &str)
{
  if(dumpStrings()) {
    printf("SENT: Line:%u  Elem:%u  String:\"%s\"\n",line,elem,
	   (const char *)str.toAscii());
  }
  TlsAP_PostText(bus_client,TELOS2101_DRIVER_SESSION_ID,(int)elem,line,str);
}


void Telos2101Driver::scanDevice()
{
  TlsAP_WaitEvents(bus_client,10);
}


void Telos2101Driver::showListData(const QString &show_name,
				   const QString &host_name,
				   bool is_active,bool passwd_reqd,
				   bool is_last)
{
  bus_shows.push_back(new ShowData(show_name,host_name,is_active,passwd_reqd));
  if(is_last) {
    emit currentShowList(id(),&bus_shows);
  }
}


void Telos2101Driver::showInfoData(unsigned max_lines,unsigned max_hybrids)
{
  bus_show_timer->stop();
  bus_current_show_auth=true;
  emit currentShow(id(),bus_showname,true);
  updateConnectionState(true);
}


void Telos2101Driver::showTimerData()
{
  //
  // FIXME: This ALWAYS returns success for a show assignment!
  //
  emit currentShow(id(),bus_showname,true);
}


void Telos2101Driver::consoleListData(const QString &console_name,
				      ConsoleData::Mode mode,bool is_free,
				      bool is_last)
{
  /*
  printf("consoleListData(%s,%d,%d,%d)\n",(const char *)console_name.toAscii(),
	 mode,is_free,is_last);
  */

  if(QString(console_name).left(2)!="AP") {  // Ignore Asst Producer Sessions
    int console=console_name.right(1).toInt()-1;
    bus_consoles.push_back(new ConsoleData(console_name,console,mode,is_free));
    if(console==bus_console) {
      bus_console_found=true;
      emit currentConsole(id(),console,mode);
      bus_clients[0]->setConsoleMode(mode);
      updateConnectionState(true);
    }
  }
  if(is_last) {
    bus_consoles.
      push_back(new ConsoleData(TELOS2101_DRIVER_VIRTUAL_CONSOLE_NAME,
				ML_MAX_CONSOLES,ConsoleData::Talent,true));
    emit currentConsoleList(id(),&bus_consoles);
    if((!bus_console_found)&&(bus_console>0)&&(bus_console<ML_MAX_CONSOLES)) {
      emit currentConsole(id(),bus_console,ConsoleData::Unavailable);
      updateConnectionState(false);
    }
  }
}


void Telos2101Driver::lineStateData(int line,int bank,int line_state)
{
  //
  // Work around a bug in the Nx12 that emits line states for
  // 'phantom' banks when in shared-show mode.
  //
  if((bank>1)&&((bus_showname=="Hybrid 1&2")||(bus_showname=="Hybrid 3&4"))) {
    return;
  }

  BusDriver::LineState lstate=BusDriver::LineInactive;
  int console=-1;

  switch(line_state) {
      case LINE_STATE_INVALID:
	lstate=BusDriver::LineInactive;
	break;

      case LINE_STATE_IDLE:
      case LINE_STATE_RESERVED:
	lstate=BusDriver::LineIdle;
	break;

      case LINE_STATE_RINGING:
	lstate=BusDriver::LineRinging;
	break;

      case LINE_STATE_HOLD:
	lstate=BusDriver::LineHold;
	break;

      case LINE_STATE_READY_HOLD:
	lstate=BusDriver::LineScreenedHold;
	break;

      case LINE_STATE_READY_NEXT:
	lstate=BusDriver::LineNext;
	break;

      case LINE_STATE_TALENT_HOLD:
	lstate=BusDriver::LineHold;
	break;

      case LINE_STATE_ON_AIR:
	lstate=BusDriver::LineOnAir;
	break;

      case LINE_STATE_ON_AIR_LOCKED:
	lstate=BusDriver::LineLocked;
	break;

      case LINE_STATE_HANDSET:
	lstate=BusDriver::LineHandset;
	console=bus_console;
	break;

      case LINE_STATE_HANDSET_LOCKED:
	lstate=BusDriver::LineHandset;
	console=bus_console;
	break;

      case LINE_STATE_SPEAKER:
	lstate=BusDriver::LineSpeaker;
	console=bus_console;
	break;

      case LINE_STATE_SPEAKER_LOCKED:
	lstate=BusDriver::LineSpeaker;
	console=bus_console;
	break;

      case LINE_STATE_SHARED_UNAVAILABLE:
      case LINE_STATE_USED_ELSEWHERE:
	lstate=BusDriver::LineElsewhere;
	break;

      case LINE_STATE_BUSY_ALL:
	lstate=BusDriver::LineBusied;
	break;

      default:
	fprintf(stderr,"rtelos2101: uncognized line status code 0x%04X\n",
		line_state);
	return;
  }
  emit currentLineState(id(),lstate,line,bank,console);
}


void Telos2101Driver::modeData(int mode)
{
  emit currentConsole(id(),bus_console,(ConsoleData::Mode)mode);
  bus_clients[0]->setConsoleMode((ConsoleData::Mode)mode);
  updateConnectionState(true);
}


void Telos2101Driver::recordData(int state)
{
  emit currentRecordState(id(),state);
}


void Telos2101Driver::busyAllData(int state)
{
  emit currentBusyAllState(id(),state);
}


void Telos2101Driver::delayDumpData(int state)
{
  emit currentDumpDelayState(id(),state);
}


void Telos2101Driver::callerIdData(int line,const char *pnum)
{
  if(strlen(pnum)>0) {
    emit receivedString(id(),BusDriver::PhoneElement,line,QString(pnum));
  }
}


void Telos2101Driver::textData(int type,int line,const QString &str)
{
  processString((BusDriver::Element)type,line,str);
}


void Telos2101Driver::errorData(long err_num,const char *err_text)
{
  if(bus_watchdog_timer->isActive()) {
    bus_watchdog_timer->stop();
    bus_watchdog_timer->start(TELOS2101_DRIVER_WATCHDOG_TIMEOUT,true);
  }
}


void Telos2101Driver::messageScanData()
{
  scanDevice();
}


void Telos2101Driver::watchdogTimerData()
{
  int fd;

  if(!bus_watchdog_active) {
    emit watchdogStateChanged(id(),true);
  }
  updateConnectionState(false);
  bus_watchdog_active=true;
  if(bus_heartbeat_timer->isActive()) {
    bus_heartbeat_timer->stop();
  }
  if(bus_message_timer->isActive()) {
    bus_message_timer->stop();
  }
  if(bus_show_timer->isActive()) {
    bus_show_timer->stop();
  }
  Init();

  //
  // Reconnect To Host
  //
  if(bus_socket!=NULL) {
    delete bus_socket;
    bus_socket=NULL;
  }
  if(!TlsAP_Connect(bus_client,(const char *)bus_hostname,
		    (const char *)bus_username,
		    (const char *)bus_userpasswd,bus_port,&fd)) {
    bus_watchdog_timer->start(TELOS2101_DRIVER_HEARTBEAT_INTERVAL,true);
    printf("TlsAP_Connect() failed!\n");
    return;
  }
  bus_socket=new Q3SocketDevice();
  bus_socket->setSocket(fd,Q3SocketDevice::Stream);
  bus_clients[0]->setIpAddress(bus_socket->address());
  bus_clients[0]->setTcpPort(bus_socket->port());
  bus_connected=true;
  bus_message_timer->start(TELOS2101_DRIVER_TICK_INTERVAL);

  //
  // Reconnect To Show
  //
  if(!TlsAP_ConnectToShow(bus_client,TELOS2101_DRIVER_SESSION_ID,
			  (const char *)bus_showname,
			  (const char *)bus_showpasswd,
			  (const char *)bus_studioname)) {
    TlsAP_Disconnect(bus_client);
    Init();
    bus_watchdog_timer->start(TELOS2101_DRIVER_HEARTBEAT_INTERVAL,true);
    return;
  }
  bus_show_timer->start(TELOS2101_DRIVER_SHOW_TIMEOUT,true);
  
  //
  // Reconnect To Console
  //
  if(bus_console>=ML_MAX_CONSOLES) {
    TlsAP_AttachAsTalent(bus_client,TELOS2101_DRIVER_SESSION_ID);
    bus_clients[0]->setConsoleMode(ConsoleData::Talent);
  }
  else {
    TlsAP_AttachToDirector(bus_client,TELOS2101_DRIVER_SESSION_ID,
			   QString().sprintf("DD%d",bus_console+1));
    TlsAP_EnumerateDirectors(bus_client,TELOS2101_DRIVER_SESSION_ID);
  }
  bus_heartbeat_timer->start(TELOS2101_DRIVER_HEARTBEAT_INTERVAL);
  bus_watchdog_timer->start(TELOS2101_DRIVER_WATCHDOG_TIMEOUT,true);
  bus_watchdog_active=false;
  updateConnectionState(true);
  emit watchdogStateChanged(id(),false);
}


void Telos2101Driver::heartbeatTimerData()
{
  TlsAP_PostText(bus_client,TELOS2101_DRIVER_SESSION_ID,
		 (int)BusDriver::HeartbeatGlobalElement,0,"tick");
}


bool Telos2101Driver::Init()
{
  if(bus_client!=NULL) {
    TlsAP_Destroy(bus_client);
  }
  bus_connected=false;
  bus_current_show_auth=false;
  if((bus_client=TlsAP_Create(this,&ph_callbacks))==NULL) {
    return false;
  }
  return true;
}
