// mcidmd.cpp
//
// A telephone services server for Call Commander
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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
#include <unistd.h>
#include <syslog.h>
#include <limits.h>
#include <time.h>
#include <netdb.h>

#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtSql/QSqlQuery>

#include <mlconf.h>
#include <ml.h>
#include <mldconfig.h>
#include <cmdswitch.h>
#include <telos2101_driver.h>
#include <telos100_driver.h>
#include <ts612_driver.h>
#include <virtual_driver.h>
#include <asterisk_driver.h>
#include <ml_timeslotlist.h>

#include <mcidmd.h>

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

//
// Global Variables
//
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
	unlink(MCIDMD_PID_FILE);
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
  openlog("mcidmd",0,LOG_DAEMON);

  //
  // Read Command Options
  //
  cid_debug=false;
  CmdSwitch *cmd=
    new CmdSwitch(qApp->argc(),qApp->argv(),"mcidmd",MCIDMD_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      cid_debug=true;
    }
  }
  delete cmd;

  //
  // Time Engines
  //
  cid_start_engine=new MLTimeEngine(this,"cid_start_engine");
  connect(cid_start_engine,SIGNAL(timeout(int)),
	  this,SLOT(connectionStartData(int)));
  cid_stop_engine=new MLTimeEngine(this,"cid_stop_engine");
  connect(cid_stop_engine,SIGNAL(timeout(int)),
	  this,SLOT(connectionStopData(int)));

  //
  // Reload Socket
  //
  cid_reload_socket=new Q3SocketDevice(Q3SocketDevice::Datagram);
  cid_reload_socket->bind(QHostAddress(),ML_MCIDMD_RELOAD_PORT);
  cid_reload_socket->setBlocking(false);

  //
  // Strobe Timer
  //
  cid_strobe_timer=new QTimer(this);
  connect(cid_strobe_timer,SIGNAL(timeout()),this,SLOT(strobeData()));

  //
  // Initialize Data Structures
  //
  syslog(LOG_DAEMON|LOG_NOTICE,"starting up");
  Init(true);

  //
  // Detach
  //
  if(!cid_debug) {
    if(!MLDetach()) {
      fprintf(stderr,"mcidmd: unable to detach\n");
      exit(256);
    }
  }

  //
  // Log PID
  //
  FILE *pidfile;
  if((pidfile=fopen(MCIDMD_PID_FILE,"w"))==NULL) {
    perror("mcidmd");
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


void MainObject::connectionStartData(int eid)
{
  int dow=QDate::currentDate().dayOfWeek();
  if(dow>7) {
    dow=1;
  }
  if(((unsigned)eid)>=cid_eid_index.size()) {
    syslog(LOG_DAEMON|LOG_WARNING,
	   "received invalid start connection engine id %d, ignoring",eid);
    return;
  }
  if(cid_monitors[cid_eid_index[eid]]->slotList()->dowActive(dow)) {
    StartConnection(cid_monitors[cid_eid_index[eid]]);
  }
}


void MainObject::connectionStopData(int eid)
{
  int dow=QDate::currentDate().dayOfWeek();
  if(dow>7) {
    dow=1;
  }
  if(((unsigned)eid)>=cid_eid_index.size()) {
    syslog(LOG_DAEMON|LOG_WARNING,
	   "received invalid stop connection engine id %d, ignoring",eid);
    return;
  }
  if(cid_monitors[cid_eid_index[eid]]->slotList()->dowActive(dow)) {
    StopConnection(cid_monitors[cid_eid_index[eid]]);
  }
}


void MainObject::callerIdReceivedData(int id,CallerIdDetail *cid)
{
  if(cid_debug) {
    printf("CallerID Data Received\n");
    printf(" Id: %d\n",id);
    printf("  Line: %d\n",cid->lineNumber());
    printf("  Direction: %c\n",cid->direction());
    printf("  State: %c\n",cid->state());
    printf("  Length: %d secs\n",cid->length());
    printf("  Ring Type: %c\n",cid->ringType());
    printf("  Ring Quantity: %d\n",cid->ringQuantity());
    printf("  Date/Time: %s\n",
	   (const char *)cid->datetime().toString("MM/dd/yyyy hh:mm:ss"));
    printf("  Number: %s\n",(const char *)cid->number());
    printf("  Name: %s\n",(const char *)cid->name());
    printf("\n");
  }
  if(cid->numberIsValid()) {
    syslog(LOG_DAEMON|LOG_DEBUG,
	   "received callerid number \"%s\" from cid source %d",
	   (const char *)cid->number(),id);
  }
  else {
    syslog(LOG_DAEMON|LOG_WARNING,
	   "received invalid callerid number \"%s\" from cid source %d, changed to \"%s\"",
	   (const char *)cid->number(),id,ML_CALLERID_NULL_NUMBER);
    cid->setNumber(ML_CALLERID_NULL_NUMBER);
  }
  for(unsigned i=0;i<cid_monitors.size();i++) {
    if(cid_monitors[i]->cidTrap()!=NULL) {
      if(cid_monitors[i]->cidTrap()->id()==id) {
	ProcessId(cid_monitors[i],cid);
      }
    }
  }
}


void MainObject::currentLineStateData(unsigned id,BusDriver::LineState lstate,
				      unsigned line,int bank,int console)
{
  //printf("currentLineState(%u,%u,%u,%d,%d)\n",id,lstate,line,bank,console);

  if(bank>cid_config->mcidmdMaxBank()) {
    return;
  }

  QDateTime current_datetime=
    QDateTime(QDate::currentDate(),QTime::currentTime());

  switch(lstate) {
      case BusDriver::LineIdle:
	if(cid_monitors[id]->slotList()->slotsActive(current_datetime)) {
	  LogCall(cid_monitors[id],line);
	}
	cid_monitors[id]->metaData(line)->clear();
	cid_monitors[id]->setPhoneNumber(line,"");
	cid_monitors[id]->setPhoneName(line,"");
	cid_monitors[id]->setRouteAction(line,MldConfig::Accept);
	cid_monitors[id]->setWarningText(line,"");
	cid_monitors[id]->setStartDateTime(line,QDateTime());
	cid_monitors[id]->clearTimers(line);
	cid_awards[id]->clear(line);
	break;

      case BusDriver::LineRinging:
	if(!cid_monitors[id]->slotList()->slotsActive(current_datetime)) {
	  return;
	}
	cid_monitors[id]->setStartDateTime(line,current_datetime);
	cid_monitors[id]->metaData(line)->
	  setElement(BusDriver::PhoneElement,
		     cid_monitors[id]->phoneNumber(line));
	cid_monitors[id]->metaData(line)->
	  setElement(BusDriver::DetailIdElement,
		     cid_monitors[id]->detailId(line));
	cid_monitors[id]->metaData(line)->
	  sendData(cid_monitors[id]->busDriver(),line);
	switch(cid_monitors[id]->routeAction(line)) {
	    case MldConfig::Accept:
	      break;

	    case MldConfig::Warn:
	      cid_monitors[id]->busDriver()->
		sendString(BusDriver::LineWarningGlobalElement,
			   line,cid_monitors[id]->warningText(line));
	      break;

	    case MldConfig::Drop:
	      cid_monitors[id]->busDriver()->
		selectLine(line,cid_monitors[id]->connection()->console(),0);
	      break;
	}
	break;

      case BusDriver::LineHandset:
      case BusDriver::LineSpeaker:
      case BusDriver::LineOnAir:
	if(cid_monitors[id]->startDateTime(line).isNull()) {
	  cid_monitors[id]->setStartDateTime(line,current_datetime);
	}
	if(cid_monitors[id]->routeAction(line)==MldConfig::Drop) {
	  cid_monitors[id]->busDriver()->drop(MCIDMD_KILL_BANK);
	}
	break;

      default:
	break;
  }
  //  printf("setLineState(%u,%u)\n",line,lstate);
  cid_monitors[id]->setLineState(line,lstate);
}


void MainObject::strobeData()
{
  for(unsigned i=0;i<cid_monitors.size();i++) {
    cid_monitors[i]->strobeTimers();
  }
}


void MainObject::receivedStringData(unsigned id,BusDriver::Element elem,
				    unsigned line,const QString &str)
{
  // printf("receivedString(%u,%u,%u,\"%s\")\n",id,elem,line,(const char *)str);
  CallerIdDetail *cid;
  QDateTime current_datetime=
    QDateTime(QDate::currentDate(),QTime::currentTime());
  switch(elem) {
    case BusDriver::RequestMetadataGlobalElement:
      cid_monitors[id]->busDriver()->
	sendString(BusDriver::LocalAreaCodeGlobalElement,0,
		   cid_monitors[id]->areaCode());
      break;

    case BusDriver::PhoneElement:
      cid=new CallerIdDetail(line+1,CallerIdDetail::CallInbound,
			     CallerIdDetail::CallStart,0,0,0,
			     QDateTime(QDate::currentDate(),
				       QTime::currentTime()),str,"");
      if(cid->numberIsValid()) {
	ProcessId(cid_monitors[id],cid);
      }
      delete cid;
      break;

    default:
      cid_monitors[id]->metaData(line)->update(elem,str);
      break;
  }
  cid_awards[id]->processString(elem,line,str);
}


void MainObject::connectionClosedData(unsigned id)
{
  if(cid_monitors[id]->connected()) {
    syslog(LOG_DAEMON|LOG_ERR,"connection \"%s[%u]\" closed",
	   (const char *)cid_monitors[id]->connection()->name(),id);
    cid_monitors[id]->setConnected(false);
  }
}


void MainObject::watchdogData(unsigned id,bool state)
{
  if(cid_monitors[id]->connected()) {
    if(state) {
      syslog(LOG_DAEMON|LOG_ERR,
	     "connection \"%s[%u]\" lost, attempting reconnect...",
	     (const char *)cid_monitors[id]->connection()->name(),id);
    }
    else {
      syslog(LOG_DAEMON|LOG_ERR,"connection \"%s[%u]\" reestablished",
	     (const char *)cid_monitors[id]->connection()->name(),id);
    }
  }
  else {
    if(!state) {
      syslog(LOG_DAEMON|LOG_NOTICE,"connection \"%s[%u]\" opened",
	     (const char *)cid_monitors[id]->connection()->name(),id);
      cid_monitors[id]->setConnected(true);
      cid_monitors[id]->busDriver()->
	sendString(BusDriver::LocalAreaCodeGlobalElement,0,
		   cid_monitors[id]->areaCode());
    }
  }
}


void MainObject::restartTimerData()
{
  char buffer[1024];

  int n=cid_reload_socket->readBlock(buffer,1024);
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


void MainObject::Init(bool initial_startup)
{
  QString hostname;
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;
  unsigned next_eid=0;
  QDateTime current_datetime(QDate::currentDate(),QTime::currentTime());

  //
  //
  // Load Configuration
  //
  hostname=GetCanonicalName();
  cid_config=new MldConfig();
  cid_config->load();

  //
  // Start the Database Connection
  //
  if(cid_config->mysqlHostname().isEmpty()) {
    if(initial_startup||cid_debug) {
      fprintf(stderr,"mcidmd: invalid database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,"invalid database server, exiting");
    exit(256);
  }
  cid_db=QSqlDatabase::addDatabase(cid_config->mysqlDbtype());
  /*
  if(!cid_db) {
    if(initial_startup||cid_debug) {
      fprintf(stderr,"mcidmd: unable to connect to database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,
	   "unable to connect to database server, exiting");
    exit(256);
  }
  */
  cid_db.setDatabaseName(cid_config->mysqlDbname());
  cid_db.setUserName(cid_config->mysqlUsername());
  cid_db.setPassword(cid_config->mysqlPassword());
  cid_db.setHostName(cid_config->mysqlHostname());
  if(!cid_db.open()) {
    cid_db.removeDatabase(cid_config->mysqlDbname());
    if(initial_startup||cid_debug) {
      fprintf(stderr,"mcidmd: unable to login to database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,"unable to login to database server, exiting");
    exit(256);
  }

  //
  // Load the Traps
  //
  sql=QString().sprintf("select HOST_ID,TTY_PORT from CID_SOURCES \
                         where HOSTNAME=\"%s\"",
			(const char *)hostname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    cid_traps.push_back(new CallerIdTrap(q->value(0).toInt(),this));
    cid_traps.back()->setSimplifyNumber(true);
    cid_traps.back()->setDeviceName(q->value(1).toString());
    connect(cid_traps.back(),SIGNAL(idReceived(int,CallerIdDetail *)),
	    this,SLOT(callerIdReceivedData(int,CallerIdDetail *)));
    if(!cid_traps.back()->open()) {
      syslog(LOG_DAEMON|LOG_ERR,"unable to open TTY port \"%s\"",
		(const char *)q->value(1).toString());
      if(initial_startup||cid_debug) {
	fprintf(stderr,"mcidmd: unable to open TTY port \"%s\"\n",
		(const char *)q->value(1).toString());
	exit(256);
      }
    }
  }
  delete q;

  //
  // Load the Monitor Configs
  //
  sql=QString().sprintf("select CID_CONNECTION,LOCAL_AREA_CODE,\
                         KILL_CALLS,DEFAULT_LABEL_ACTION,CID_SOURCE_ID,\
                         SHOW_CODE from SHOWS\
                         where (IS_ACTIVE=\"Y\")&&(CID_HOSTNAME=\"%s\")",
			(const char *)hostname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    cid_monitors.push_back(new CidMonitor());

    //
    // Load Monitor Configuration
    //
    cid_monitors.back()->setAreaCode(q->value(1).toString());
    cid_monitors.back()->setKillCalls(MLBool(q->value(2).toString()));
    cid_monitors.back()->setShowCode(q->value(5).toString());

    //
    // Load the Connection Monitor
    //
    cid_monitors.back()->
      setConnection(new ConnectionData(ConnectionData::StorageDatabase));
    cid_monitors.back()->connection()->setName(q->value(0).toString());
    if(!cid_monitors.back()->connection()->load()) {
      syslog(LOG_DAEMON|LOG_ERR,
	     "unable to load connection \"%s\" from database",
	     (const char *)q->value(0).toString());
      if(initial_startup||cid_debug) {
	fprintf(stderr,
		"mcidmd: unable to load connection \"%s\" from database\n",
		(const char *)q->value(0).toString());
	exit(256);
      }
    }

    //
    // Load the Award Data Context
    //
    cid_awards.push_back(new AwardData(q->value(5).toString()));

    //
    // Load the Bus Driver
    //
    switch(cid_monitors.back()->connection()->systemType()) {
	case ConnectionData::TypeAsterisk:
	  cid_monitors.back()->
	    setBusDriver(new AsteriskDriver(cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeTelos2101:
	  cid_monitors.back()->
	    setBusDriver(new Telos2101Driver(cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeTelos100Key:
	  cid_monitors.back()->
	    setBusDriver(new Telos100Driver(Telos100Driver::KeyType,
					    cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeTelos100Dim:
	  cid_monitors.back()->
	    setBusDriver(new Telos100Driver(Telos100Driver::DimType,
					    cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeTelos100OneXSix:
	  cid_monitors.back()->
	    setBusDriver(new Telos100Driver(Telos100Driver::OneXSixType,
					    cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeGentnerTs612:
	  cid_monitors.back()->
	    setBusDriver(new Ts612Driver(cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeVirtual:
	  cid_monitors.back()->
	    setBusDriver(new VirtualDriver(cid_monitors.size()-1,this));
	  break;

	case ConnectionData::TypeUnknown:
	  break;
    }
    cid_monitors.back()->busDriver()->
      setClientType(ClientData::TypeCidMonitor);
    connect(cid_monitors.back()->busDriver(),
	    SIGNAL(currentLineState(unsigned,BusDriver::LineState,
				    unsigned,int,int)),
	    this,SLOT(currentLineStateData(unsigned,BusDriver::LineState,
					   unsigned,int,int)));
    connect(cid_monitors.back()->busDriver(),
	    SIGNAL(receivedString(unsigned,BusDriver::Element,unsigned,
				  const QString &)),
	    this,
	    SLOT(receivedStringData(unsigned,BusDriver::Element,unsigned,
				    const QString &)));
    connect(cid_monitors.back()->busDriver(),
	    SIGNAL(connectionClosed(unsigned)),
	    this,SLOT(connectionClosedData(unsigned)));
    connect(cid_monitors.back()->busDriver(),
	    SIGNAL(watchdogStateChanged(unsigned,bool)),
	    this,SLOT(watchdogData(unsigned,bool)));

    //
    // Associate the Trap
    //
    cid_monitors.back()->setCidTrap(NULL);
    for(unsigned i=0;i<cid_traps.size();i++) {
      if(cid_traps[i]->id()==q->value(4).toInt()) {
	cid_monitors.back()->setCidTrap(cid_traps[i]);
	sql=QString().sprintf("select SOURCE_LINE,TARGET_LINE from CID_MAPS\
                               where SHOW_CODE=\"%s\"",
			      (const char *)cid_monitors.back()->showCode());
	q1=new QSqlQuery(sql);
	while(q1->next()) {
	  cid_monitors.back()->
	    setCidLineMap(q1->value(0).toInt()-1,q1->value(1).toInt()-1);
	}
	delete q1;
      }
    }

    //
    // Load the Timeslots
    //
    cid_monitors.back()->
      setSlotList(new MlTimeslotList(q->value(5).toString()));
    cid_monitors.back()->slotList()->load();
    sql=QString().sprintf("select START_TIME,END_TIME from SHOW_SLOTS \
                           where SHOW_CODE=\"%s\"",
			  (const char *)q->value(5).toString());
    q1=new QSqlQuery(sql);
    while(q1->next()) {
      cid_start_engine->addEvent(next_eid,q1->value(0).toTime());
      cid_stop_engine->addEvent(next_eid,q1->value(1).toTime());
      cid_eid_index.push_back(cid_monitors.size()-1);
      next_eid++;
    }
    delete q1;

    //
    // Fire Up the Connection (if necessary)
    //
    if(cid_monitors.back()->slotList()->slotsActive(current_datetime)) {
      StartConnection(cid_monitors.back());
    }
  }
  delete q;

  //
  // Start the strobe timer
  //
  cid_strobe_timer->start(1000);
}


void MainObject::Release()
{
  //
  // Stop the strobe timer
  //
  cid_strobe_timer->stop();

  //
  // Clear the Engines
  //
  cid_start_engine->clear();
  cid_stop_engine->clear();
  cid_eid_index.clear();

  //
  // Shut Down the Traps
  //
  for(unsigned i=0;i<cid_traps.size();i++) {
    cid_traps[i]->close();
  }

  //
  // Free the Award Contexts
  //
  for(unsigned i=0;i<cid_awards.size();i++) {
    delete cid_awards[i];
  }
  cid_awards.clear();

  //
  // Tear Down the Connections
  //
  for(unsigned i=0;i<cid_monitors.size();i++) {
    if(cid_monitors[i]->connected()) {
      syslog(LOG_DAEMON|LOG_NOTICE,"connection \"%s[%u]\" closed",
	     (const char *)cid_monitors[i]->connection()->name(),i);
    }
    delete cid_monitors[i]->busDriver();
  }
  cid_monitors.clear();

  //
  // Delete the Traps
  //
  for(unsigned i=0;i<cid_traps.size();i++) {
    delete cid_traps[i];
  }
  cid_traps.clear();

  //
  // Close Database Connection
  //
  QSqlDatabase::removeDatabase(cid_config->mysqlDbname());

  //
  // Close Config File
  //
  delete cid_config;
  cid_config=NULL;
}


void MainObject::StartConnection(CidMonitor *mon)
{
  switch(mon->connection()->systemType()) {
      case ConnectionData::TypeTelos2101:
      case ConnectionData::TypeVirtual:
      case ConnectionData::TypeAsterisk:
	mon->busDriver()->
	  connect(mon->connection()->hostName(),
		  mon->connection()->userName(),
		  mon->connection()->userPassword(),
		  mon->connection()->tcpPort(),
		  mon->connection()->showName(),
		  mon->connection()->showPassword(),
		  mon->connection()->location(),
		  mon->connection()->console(),
		  mon->connection()->consoleName());
	break;
	
      case ConnectionData::TypeTelos100Key:
      case ConnectionData::TypeTelos100Dim:
      case ConnectionData::TypeTelos100OneXSix:
      case ConnectionData::TypeGentnerTs612:
	mon->busDriver()->
	  connect(mon->connection()->ttyPort(),
		  mon->connection()->showName(),
		  mon->connection()->showPassword(),
		  mon->connection()->location(),
		  mon->connection()->console(),
		  mon->connection()->consoleName());
	break;
	
      case ConnectionData::TypeUnknown:
	break;
  }
  mon->busDriver()->requestLineState();
}


void MainObject::StopConnection(CidMonitor *mon)
{
  mon->busDriver()->disconnect();
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
  return (username==cid_config->mysqlUsername())&&
    (password==cid_config->mysqlPassword());
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
