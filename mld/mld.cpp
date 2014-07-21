// mld.cpp
//
// A telephone services server for Call Commander
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

#include <signal.h>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtCore/QObject>
#include <QtSql/QSqlQuery>
#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>
#include <vector>
#include <syslog.h>
#include <netdb.h>
#include <escapestring.h>

#include <mlconf.h>
#include <mld_socket.h>

#include <telos100_driver.h>
#include <ts612_driver.h>
#include <virtual_driver.h>
#include <asterisk_driver.h>
#include <ml.h>
#include <mld.h>
#include <globals.h>

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 255
#endif  // HOST_NAME_MAX

//
// Global Variables
//
MldConfig *mld_config;
volatile bool restart=false;


void SigHandler(int signum)
{
  switch(signum) {
      case SIGHUP:
	restart=true;
	break;

      case SIGINT:
      case SIGTERM:
	syslog(LOG_DAEMON|LOG_NOTICE,"shutting down normally");
	closelog();
	unlink(MLD_PID_FILE);
	exit(0);
	break;
  }
}


MainObject::MainObject(QObject *parent,const char *name)
  :QObject(parent,name)
{
  //
  // Open the Syslog Connection
  //
  openlog("mld",0,LOG_DAEMON);

  mld_server=NULL;
  
  //
  // Read Command Switches
  //
  mld_debug=false;
  mld_conf_file=DEFAULT_CONF_FILE;
  for(int i=1;i<qApp->argc();i++) {
    if(qApp->argv()[i]==QString("-d")) {
      mld_debug=true;
    }
    else {
      mld_conf_file=qApp->argv()[i];
    }
  }

  //
  // The Reload Socket
  //
  mld_reload_socket=new Q3SocketDevice(Q3SocketDevice::Datagram);
  mld_reload_socket->bind(QHostAddress(),ML_MLD_RELOAD_PORT);
  mld_reload_socket->setBlocking(false);

  //
  // Initialize Data Structures
  //
  syslog(LOG_DAEMON|LOG_NOTICE,"starting up");
  Init();

  //
  // Detach
  //
  if(!mld_debug) {
    if(!MLDetach()) {
      fprintf(stderr,"mld: unable to detach\n");
      exit(1);
    }
  }

  //
  // Log PID
  //
  FILE *pidfile;
  if((pidfile=fopen(MLD_PID_FILE,"w"))==NULL) {
    perror("mld");
  }
  else {
    fprintf(pidfile,"%d",getpid());
    fclose(pidfile);
  }

  signal(SIGHUP,SigHandler);
  signal(SIGINT,SigHandler);
  signal(SIGTERM,SigHandler);

  //
  // Restart Timer
  //
  QTimer *timer=new QTimer(this,"restart_timer");
  connect(timer,SIGNAL(timeout()),this,SLOT(restartTimerData()));
  timer->start(RESTART_INTERVAL);
}


void MainObject::newConnection(int fd)
{
  mld_connections[fd]=new MldConnection(new Q3Socket(this));
  mld_connections[fd]->socket()->setSocket(fd);
  mld_ready_mapper->setMapping(mld_connections[fd]->socket(),fd);
  connect(mld_connections[fd]->socket(),SIGNAL(readyRead()),
	  mld_ready_mapper,SLOT(map()));
  mld_kill_mapper->setMapping(mld_connections[fd]->socket(),fd);
  connect(mld_connections[fd]->socket(),SIGNAL(connectionClosed()),
	  mld_kill_mapper,SLOT(map()));
}


void MainObject::socketKill(int fd)
{
  mld_connections[fd]->setZombie(true);
  mld_zombie_timer->start(10,true);
}


void MainObject::zombieData()
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->zombie()) {
      delete ci->second;
      mld_connections.erase(ci);
    }
  }
}


void MainObject::readyReadData(int fd)
{
  char buf[256];
  int n;
  bool esc=false;

  while((n=mld_connections[fd]->socket()->readBlock(buf,256))>0) {
    buf[n]=0;
    for(int i=0;i<n;i++) {
      if(!iscntrl(buf[i])) {
	mld_connections[fd]->buffer[mld_connections[fd]->ptr++]=buf[i];
	switch(buf[i]) {
	    case '\\':
	      esc=!esc;
	      break;

	    case '!':
	      if(esc) {
		esc=false;
	      }
	      else {
		mld_connections[fd]->buffer[mld_connections[fd]->ptr]=0;
		ParseLine(fd);
	      }
	      break;

	    default:
	      esc=false;
	      break;
	}
	if(mld_connections[fd]->ptr==ML_VIRTUAL_MAX_MSG_LENGTH) {
	  fprintf(stderr,"client message exceeded max length -- ignoring\n");
	  mld_connections[fd]->ptr=0;
	}
      }
    }
  }
}


void MainObject::connectedData(unsigned id,bool state)
{
  if(state) {
    mld_systems[id]->busDriver()->
      connectToShow(mld_systems[id]->connectionData()->showName(),
		    mld_systems[id]->connectionData()->showPassword(),
		    mld_systems[id]->connectionData()->location());
    if(!mld_systems[id]->connected()) {
      syslog(LOG_DAEMON|LOG_NOTICE,"connection \"%s\" established",
	     (const char *)mld_systems[id]->connectionData()->name());
      mld_systems[id]->setConnected(true);
    }
  }
  else {
    if(mld_debug) {
      fprintf(stderr,"mld: unable to open connection \"%s\"\n",
	      (const char *)mld_systems[id]->connectionData()->name());
    }
    syslog(LOG_DAEMON|LOG_ERR,"unable to open connection \"%s\"",
	   (const char *)mld_systems[id]->connectionData()->name());
  }
}


void MainObject::currentShowData(unsigned id,const QString &show_name,
				 bool state)
{
  if(state) {
    mld_systems[id]->busDriver()->
      connectToConsole(mld_systems[id]->connectionData()->console(),
		       mld_systems[id]->connectionData()->consoleName());
  }
  else {
    if(mld_debug) {
      fprintf(stderr,"mld: unable to connect to show \"%s\"\n",
	      (const char *)show_name);
    }
    syslog(LOG_DAEMON|LOG_ERR,"unable to connect to show \"%s\"",
	   (const char *)show_name);
  }
}


void MainObject::currentConsoleData(unsigned id,int console,
				    ConsoleData::Mode mode)
{
  // printf("currentConsoleData(%u,%d,%d)\n",id,console,mode);
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      ci->second->setConsole(console);
      SendCommand(ci->first,QString().sprintf("LC %d %d +!",console,mode));
    }
  }
}


void MainObject::currentConsoleListData(unsigned id,
					std::vector<ConsoleData *> *data)
{
  // printf("currentConsoleListData(%d)\n",id);
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      for(unsigned i=0;i<data->size();i++) {
	SendCommand(ci->first,QString().
		    sprintf("CL %d %d %d %s!",
			    data->at(i)->console(),
			    data->at(i)->mode(),
			    data->at(i)->isFree(),
			    (const char *)VirtualDriver::
			    escapeString(data->at(i)->name())));
      }
      SendCommand(ci->first,"CL!");
    }
  }
}


void MainObject::errorData(unsigned id,BusDriver::Error err)
{
}


void MainObject::lineStateData(unsigned id,BusDriver::LineState lstate,
			       unsigned line,int bank,int console)
{
  // printf("lineStateData(%u,%d,%u,%d,%d)\n",id,lstate,line,bank,console);
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      SendCommand(ci->first,QString().
		  sprintf("SS %d %d %d %d!",line,lstate,bank,console));
    }
  }
}


void MainObject::busyAllStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"BA +!");
      }
      else {
	SendCommand(ci->first,"BA -!");
      }
    }
  }
}


void MainObject::dumpDelayStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"DD +!");
      }
      else {
	SendCommand(ci->first,"DD -!");
      }
    }
  }
}


void MainObject::recordStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"RD +!");
      }
      else {
	SendCommand(ci->first,"RD -!");
      }
    }
  }
}


void MainObject::screenStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"SM +!");
      }
      else {
	SendCommand(ci->first,"SM -!");
      }
    }
  }
}


void MainObject::muteStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"MM +!");
      }
      else {
	SendCommand(ci->first,"MM -!");
      }
    }
  }
}


void MainObject::conferenceStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"CM +!");
      }
      else {
	SendCommand(ci->first,"CM -!");
      }
    }
  }
}


void MainObject::vipStateData(unsigned id,bool state)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      if(state) {
	SendCommand(ci->first,"VM +!");
      }
      else {
	SendCommand(ci->first,"VM -!");
      }
    }
  }
}


void MainObject::receivedStringData(unsigned id,BusDriver::Element elem,
				    unsigned line,const QString &str)
{
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    if(ci->second->systemId()==id) {
      SendCommand(ci->first,QString().
		  sprintf("TX %u %d %s!",line,elem,(const char *)str));
    }
  }
}


void MainObject::restartTimerData()
{
  char buffer[1024];

  int n=mld_reload_socket->readBlock(buffer,1024);
  if(n>0) {
    restart|=AuthenticateReloadPacket(buffer,n);
  }
  if(!restart) {
    return;
  }
  syslog(LOG_DAEMON|LOG_NOTICE,"reloading configuration");
  restart=false;
  Release();
  Init();
}


void MainObject::watchdogData(unsigned id,bool state)
{
  if(state) {
    syslog(LOG_DAEMON|LOG_ERR,
	   "connection \"%s[%u]\" lost, attempting reconnect...",
	   (const char *)mld_systems[id]->connectionData()->name(),id);
  }
  else {
    syslog(LOG_DAEMON|LOG_ERR,"connection \"%s[%u]\" reestablished",
	   (const char *)mld_systems[id]->connectionData()->name(),id);
  }
}


void MainObject::ParseLine(int fd)
{
  // printf("RECV: %s  LEN: %u\n",(const char *)mld_connections[fd]->buffer,
  //	 strlen(mld_connections[fd]->buffer));

  bool esc=false;
  QString sql;
  QSqlQuery *q;
  std::vector<QString> args;

  args.push_back(QString());
  for(unsigned i=0;i<(strlen(mld_connections[fd]->buffer)-1);i++) {

    switch(mld_connections[fd]->buffer[i]) {
	case '\\':
	  args.back()+=mld_connections[fd]->buffer[i];
	  esc=!esc;
	  break;

	case ' ':
	  if(esc) {
	    args.back()+=mld_connections[fd]->buffer[i];
	    esc=false;
	  }
	  else {
	    args.push_back(QString());
	  }
	  break;

	default:
	  args.back()+=mld_connections[fd]->buffer[i];
	  esc=false;
	  break;
    }
  }
  mld_connections[fd]->ptr=0;
  if(args.size()<1) {
    return;
  }
  args.back().stripWhiteSpace();

  //
  // Unprivileged Commands
  //
  if((args[0]=="LH")&&(args.size()==3)) {    // Login to Host
    sql=QString().sprintf("select LOGIN_NAME from USERS where \
                           (LOGIN_NAME=\"%s\")&&\
                           (PASSWORD=\"%s\")&&\
                           (SCREENER_PRIV=\"Y\")",
			  (const char *)VirtualDriver::restoreString(args[1]),
			  (const char *)VirtualDriver::
			  stripQuotes(VirtualDriver::restoreString(args[2])));
    q=new QSqlQuery(sql);
    if(q->first()) {
      mld_connections[fd]->setUserName(VirtualDriver::restoreString(args[1]));
      mld_connections[fd]->setAuthenticated(true);
      EchoArgs(fd,&args,true);
    }
    else {
      EchoArgs(fd,&args,false);
    }
    delete q;
    return;
  }

  //
  // Privileged Commands
  //
  if(!mld_connections[fd]->authenticated()) {
    return;
  }

  if((args[0]=="SL")&&(args.size()==1)) {    // Request Show List
    for(std::map<int,MldSystem *>::iterator ci=mld_systems.begin();
	ci!=mld_systems.end();++ci) {
      SendCommand(fd,QString().sprintf("SL %s 1 %s!",
				       (const char *)ci->second->location(),
				       (const char *)ci->second->showName()));
    }
    SendCommand(fd,"SL!");
    return;
  }

  if((args[0]=="LS")&&(args.size()>=4)) {    // Login to Show
    RollupArgs(&args,3);
    for(std::map<int,MldSystem *>::iterator ci=mld_systems.begin();
	ci!=mld_systems.end();++ci) {
      if((VirtualDriver::restoreString(args[3])==ci->second->showName())&&
	 (VirtualDriver::stripQuotes(VirtualDriver::restoreString(args[2]))==
	  ci->second->showPassword())) {
	mld_connections[fd]->setSystemId(ci->second->id());
	EchoArgs(fd,&args,true);
	return;
      }
    }
    EchoArgs(fd,&args,false);
    return;
  }

  //
  // All subsequent commands require a valid show login
  //
  if(mld_connections[fd]->systemId()==0xFFFFFFFF) {
    return;
  }

  if((args[0]=="CL")&&(args.size()==1)) {    // Request Console List
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      requestConsoleList();
    return;
  }

  if((args[0]=="LC")&&(args.size()==3)) {    // Connect to Console
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      connectToConsole(args[1].toInt(),args[2]);
    return;
  }

  //
  // All subsequent commands require a valid console connection
  //
  if(mld_connections[fd]->console()<0) {
    return;
  }

  if((args[0]=="SS")&&(args.size()==1)) {    // Request Line States
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      requestLineState();
    return;
  }

  if((args[0]=="LA")&&(args.size()==4)) {    // Select Line
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      selectLine(args[1].toUInt(),args[3].toInt(),args[2].toInt());
    return;
  }

  if((args[0]=="HD")&&(args.size()==3)) {    // Hold
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      hold(args[1].toInt(),args[2].toInt());
    return;
  }

  if((args[0]=="DP")&&(args.size()==3)) {    // Drop
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      drop(args[1].toInt(),args[2].toInt());
    return;
  }

  if((args[0]=="BS")&&(args.size()==1)) {    // Request Busy All State
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      requestBusyAllState();
    return;
  }

  if((args[0]=="BA")&&(args.size()==1)) {    // Busy All
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->busyAll();
    return;
  }

  if((args[0]=="DS")&&(args.size()==1)) {    // Request Delay Dump State
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      requestDumpDelayState();
    return;
  }

  if((args[0]=="DD")&&(args.size()==2)) {    // Dump Delay
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      dumpDelay(args[1].toUInt());
    return;
  }

  if((args[0]=="RS")&&(args.size()==1)) {    // Request Record State
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      requestRecordState();
    return;
  }

  if((args[0]=="RD")&&(args.size()==1)) {    // Toggle Record
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->toggleRecord();
    return;
  }

  if((args[0]=="NX")&&(args.size()==2)) {    // Take Next
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      takeNext(args[1].toInt());
    return;
  }

  if((args[0]=="TO")&&(args.size()==2)) {    // Send DTMF
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      sendDtmf(((const char *)args[1])[0]);
    return;
  }

  if((args[0]=="FS")&&(args.size()==1)) {    // Flash Switchhook
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      flash();
    return;
  }

  if((args[0]=="SM")&&(args.size()==2)) {    // Screen
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      screen(args[1].toUInt());
    return;
  }

  if((args[0]=="MM")&&(args.size()==2)) {    // Mute
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      mute(args[1].toUInt());
    return;
  }

  if((args[0]=="CM")&&(args.size()==2)) {    // Conference
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      conference(args[1].toUInt());
    return;
  }

  if((args[0]=="VM")&&(args.size()==2)) {    // Vip
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      vip(args[1].toUInt());
    return;
  }

  if((args[0]=="TX")&&(args.size()>=4)) {    // Send Text
    if((BusDriver::Element)args[2].toInt()==
       BusDriver::HeartbeatGlobalElement) {
      SendCommand(fd,QString().sprintf("TX 0 %d tock!",
				       BusDriver::HeartbeatGlobalElement));
      return;
    }
    RollupArgs(&args,3);
    mld_systems[mld_connections[fd]->systemId()]->busDriver()->
      sendString((BusDriver::Element)args[2].toInt(),args[1].toUInt(),args[3]);
    for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
	ci!=mld_connections.end();++ci) {
      if((ci->second->socket()->socket()!=fd)&&
	 (ci->second->systemId()==mld_connections[fd]->systemId())) {
	SendCommand(ci->first,QString().
		    sprintf("TX %d %d %s!",args[1].toInt(),args[2].toInt(),
			    (const char *)args[3]));
      }
    }
    return;
  }

  //
  // No match -- send an error response
  //
  EchoArgs(fd,&args,false);
}


void MainObject::SendCommand(int fd,QString cmd)
{
  // printf("SEND %d:%s\n",fd,(const char *)cmd);
  mld_connections[fd]->socket()->writeBlock(cmd,cmd.length());
}


void MainObject::EchoArgs(int fd,vector<QString> *args,bool state)
{
  if(args->size()<1) {
    return;
  }
  QString str=args->at(0);
  for(unsigned i=1;i<args->size();i++) {
    str+=" "+args->at(i);
  }
  if(state) {
    str+=" +!";
  }
  else {
    str+=" -!";
  }
  mld_connections[fd]->socket()->writeBlock(str,str.length());
}


void MainObject::Init(bool initial_startup)
{
  QString sql;
  QSqlQuery *q;
  int id=-1;

  //
  // Load Configuration
  //
  mld_config=new MldConfig();
  mld_config->load();

  //
  // Start the Database Connection
  //
  if(mld_config->mysqlHostname().isEmpty()) {
    syslog(LOG_DAEMON|LOG_CRIT,"invalid database server, exiting");
    if(initial_startup||mld_debug) {
      fprintf(stderr,"mld: invalid database server\n");
    }
    exit(256);
  }
  mld_database=QSqlDatabase::addDatabase(mld_config->mysqlDbtype());
  /*
  if(!mld_database) {
    syslog(LOG_DAEMON|LOG_CRIT,
	   "unable to connect to database server, exiting");
    if(initial_startup||mld_debug) {
      fprintf(stderr,"mld: unable to connect to database server\n");
    }
    exit(256);
  }
  */
  mld_database.setDatabaseName(mld_config->mysqlDbname());
  mld_database.setUserName(mld_config->mysqlUsername());
  mld_database.setPassword(mld_config->mysqlPassword());
  mld_database.setHostName(mld_config->mysqlHostname());
  if(!mld_database.open()) {
    mld_database.removeDatabase(mld_config->mysqlDbname());
    if(initial_startup||mld_debug) {
      fprintf(stderr,"mld: unable to log in to database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,"unable to log in to database server, exiting");
    exit(256);
  }

  //
  // Start the System Connections
  //
  QString hostname=GetCanonicalName();
  sql=QString().sprintf("select ID,SHOW_NAME,SHOW_PASSWORD,LOCATION,\
                         CONNECTION \
                         from VIRTUAL_SYSTEMS where HOSTNAME=\"%s\"",
			(const char *)hostname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    id=q->value(0).toInt();
    mld_systems[id]=new MldSystem(id);
    mld_systems[id]->setShowName(q->value(1).toString());
    mld_systems[id]->setShowPassword(q->value(2).toString());
    mld_systems[id]->setLocation(q->value(3).toString());
    mld_systems[id]->
      setConnectionData(new ConnectionData(ConnectionData::StorageDatabase));
    mld_systems[id]->connectionData()->setName(q->value(4).toString());
    if(!mld_systems[id]->connectionData()->load()) {
      syslog(LOG_DAEMON|LOG_ERR,"unable to load connection \"%s\"",
	     (const char *)q->value(4).toString());
      if(initial_startup||mld_debug) {
	fprintf(stderr,"mld: unable to load connection \"%s\"\n",
		(const char *)q->value(4).toString());
	exit(256);
      }
    }
    switch(mld_systems[id]->connectionData()->systemType()) {
	case ConnectionData::TypeAsterisk:
	  mld_systems[id]->setBusDriver(new AsteriskDriver(id,this));
	  break;

	case ConnectionData::TypeTelos100Key:
	  mld_systems[id]->
	    setBusDriver(new Telos100Driver(Telos100Driver::KeyType,id,this));
	  break;

	case ConnectionData::TypeTelos100Dim:
	  mld_systems[id]->
	    setBusDriver(new Telos100Driver(Telos100Driver::DimType,id,this));
	  break;

	case ConnectionData::TypeTelos100OneXSix:
	  mld_systems[id]->
	    setBusDriver(new Telos100Driver(Telos100Driver::OneXSixType,id,
					    this));
	  break;

	case ConnectionData::TypeGentnerTs612:
	  mld_systems[id]->setBusDriver(new Ts612Driver(id,this));
	  break;

	default:
	  syslog(LOG_DAEMON|LOG_ERR,
		 "unable to load invalid system type in connection \"%s\"",
		 (const char *)mld_systems[id]->connectionData()->name());
	  if(initial_startup||mld_debug) {
	    fprintf(stderr,"mld: unable to load invalid system type in connection \"%s\"\n",
		 (const char *)mld_systems[id]->connectionData()->name());
	    exit(256);
	  }
	  break;
    }
    connect(mld_systems[id]->busDriver(),SIGNAL(connected(unsigned,bool)),
	    this,SLOT(connectedData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentShow(unsigned,const QString &,bool)),
	    this,SLOT(currentShowData(unsigned,const QString &,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentConsole(unsigned,int,ConsoleData::Mode)),
	    this,SLOT(currentConsoleData(unsigned,int,ConsoleData::Mode)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentConsoleList(unsigned,std::vector<ConsoleData *> *)),
	    this,
	    SLOT(currentConsoleListData(unsigned,
					std::vector<ConsoleData *> *)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(error(unsigned,BusDriver::Error)),
	    this,SLOT(errorData(unsigned,BusDriver::Error)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentLineState(unsigned,BusDriver::LineState,
				    unsigned,int,int)),this,
	    SLOT(lineStateData(unsigned,BusDriver::LineState,
			       unsigned,int,int)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentBusyAllState(unsigned,bool)),
	    this,SLOT(busyAllStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentDumpDelayState(unsigned,bool)),
	    this,SLOT(dumpDelayStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentRecordState(unsigned,bool)),
	    this,SLOT(recordStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentVipState(unsigned,bool)),
	    this,SLOT(vipStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentScreenState(unsigned,bool)),
	    this,SLOT(screenStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentConferenceState(unsigned,bool)),
	    this,SLOT(conferenceStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(currentMuteState(unsigned,bool)),
	    this,SLOT(muteStateData(unsigned,bool)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(receivedString(unsigned,BusDriver::Element,unsigned,
				  const QString &)),
	    this,SLOT(receivedStringData(unsigned,BusDriver::Element,unsigned,
					 const QString &)));
    connect(mld_systems[id]->busDriver(),
	    SIGNAL(watchdogStateChanged(unsigned,bool)),
	    this,SLOT(watchdogData(unsigned,bool)));
    switch(mld_systems[id]->connectionData()->systemType()) {
	case ConnectionData::TypeTelos100Key:
	case ConnectionData::TypeTelos100Dim:
	case ConnectionData::TypeTelos100OneXSix:
	case ConnectionData::TypeGentnerTs612:
	  mld_systems[id]->busDriver()->
	    connectToSerial(mld_systems[id]->connectionData()->ttyPort());
	  break;

      case ConnectionData::TypeAsterisk:
	mld_systems[id]->busDriver()->
	  connectToHost(mld_systems[id]->connectionData()->hostName(),
			mld_systems[id]->connectionData()->userName(),
			mld_systems[id]->connectionData()->userPassword(),
			mld_systems[id]->connectionData()->tcpPort());
	break;

      default:
	break;
    }
  }
  delete q;

  //
  // Start Up Server Socket
  //
  mld_ready_mapper=new QSignalMapper(this,"mld_ready_mapper");
  connect(mld_ready_mapper,SIGNAL(mapped(int)),this,SLOT(readyReadData(int)));
  mld_kill_mapper=new QSignalMapper(this,"mld_kill_mapper");
  connect(mld_kill_mapper,SIGNAL(mapped(int)),this,SLOT(socketKill(int)));
  mld_server=new MldSocket(ML_VIRTUAL_TCP_PORT,0,this,"socket");
  if(!mld_server->ok()) {
    syslog(LOG_DAEMON|LOG_CRIT,"unable to bind server socket, exiting");
    exit(256);
    if(initial_startup||mld_debug) {
      fprintf(stderr,"mld: unable to bind server socket\n");
    }
  }
  connect(mld_server,SIGNAL(connection(int)),this,SLOT(newConnection(int)));
  mld_zombie_timer=new QTimer(this,"mld_zombie_timer");
  connect(mld_zombie_timer,SIGNAL(timeout()),this,SLOT(zombieData()));
}


void MainObject::Release()
{
  //
  // Shut down network connections
  //
  for(std::map<int,MldConnection *>::iterator ci=mld_connections.begin();
      ci!=mld_connections.end();++ci) {
    delete ci->second;
  }
  mld_connections.clear();
  delete mld_zombie_timer;
  mld_zombie_timer=NULL;
  delete mld_server;
  mld_server=NULL;
  delete mld_ready_mapper;
  mld_ready_mapper=NULL;
  delete mld_kill_mapper;
  mld_kill_mapper=NULL;

  //
  // Shut down System Connections
  //
  for(std::map<int,MldSystem *>::iterator ci=mld_systems.begin();
      ci!=mld_systems.end();++ci) {
    syslog(LOG_DAEMON|LOG_NOTICE,"connection \"%s\" shut down",
	   (const char *)ci->second->connectionData()->name());
    delete ci->second;
  }
  mld_systems.clear();

  //
  // Shut Down Database Connection
  //
  mld_database.removeDatabase(mld_config->mysqlDbname());

  //
  // Free Configuration
  //
  delete mld_config;
  mld_config=NULL;
}


void MainObject::RollupArgs(std::vector<QString> *args,unsigned n)
{
  for(unsigned i=n+1;i<args->size();i++) {
    args->at(n)+=(" "+args->at(i));
  }
}


QString MainObject::GetCanonicalName() const
{
  char hostname[HOST_NAME_MAX+1];
  struct hostent *hostent;

  gethostname(hostname,HOST_NAME_MAX);
  if((hostent=gethostbyname(hostname))==NULL) {
    syslog(LOG_DAEMON|LOG_ERR,"unable to determine canonical host name");
    return hostname;
  }
  syslog(LOG_DAEMON|LOG_NOTICE,"using canonical name \"%s\"",hostent->h_name);
  return QString(hostent->h_name);
}


bool MainObject::AuthenticateReloadPacket(char *b,int n)
{
  char username[1024];
  char password[1024];

  b[n]=0;
  sscanf(b,"%s%s",username,password);
  return (username==mld_config->mysqlUsername())&&
    (password==mld_config->mysqlPassword());
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
