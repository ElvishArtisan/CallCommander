// mlmh.cpp
//
// The CallCommander Logic Module Host
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
#include <limits.h>
#include <time.h>
#include <netdb.h>
#include <dlfcn.h>
#include <syslog.h>
#include <pthread.h>
#include <errno.h>

#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtCore/QSignalMapper>

#include <ml.h>
#include <mldconfig.h>
#include <mlconf.h>
#include <cmdswitch.h>
#include <telos2101_driver.h>
#include <telos100_driver.h>
#include <ts612_driver.h>
#include <virtual_driver.h>
#include <asterisk_driver.h>
#include <ml_timeslotlist.h>

#include <globals.h>
#include <mlmh.h>

#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 256
#endif

//
// Global Variables
//
MldConfig *mld_config;
BusDriver *global_bus_driver;
bool global_shutdown_flag=false;
Q3SocketDevice *global_send_socket;
std::vector<Q3SocketDevice *> global_receive_sockets;
std::vector<MLTTYDevice *> global_tty_devices;
std::vector<VGuest *> global_vguests;
QTimer *global_callback_timers[MLM_MAX_TIMERS];
QTimer *global_receive_timer;
char global_module_args[256];
QString global_show;
int global_show_state;
int global_console;
int global_console_mode;
int global_lstate[MAX_LINES];
int global_bank[MAX_LINES];
int global_line_console[MAX_LINES];
int global_watchdog_state;
int global_dump_state;
int global_busy_all_state;
int global_record_state;
int global_screen_state;
int global_mute_state;
int global_conference_state;
int global_vip_state;
int global_delay_offset=0;
bool global_init_complete;
bool global_hard_shutdown;
void (*mlm_startup_sym)(const char *);
void (*mlm_shutdown_sym)();
void (*mlm_metadata_update_requested_sym)();
void (*mlm_watchdog_state_changed_sym)(int);
void (*mlm_show_changed_sym)(const char *,int);
void (*mlm_console_changed_sym)(int,int);
void (*mlm_line_state_changed_sym)(int,int,int,int);
void (*mlm_dump_state_changed_sym)(int);
void (*mlm_record_state_changed_sym)(int);
void (*mlm_busy_all_state_changed_sym)(int);
void (*mlm_screen_state_changed_sym)(int);
void (*mlm_mute_state_changed_sym)(int);
void (*mlm_conference_state_changed_sym)(int);
void (*mlm_vip_state_changed_sym)(int);
void (*mlm_delay_offset_changed_sym)(int);
void (*mlm_received_string_sym)(int,int,const char *);
void (*mlm_caller_id_received_sym)(int,const char *);
void (*mlm_chat_message_received_sym)(const char *,const char *);
void (*mlm_caller_element_received_sym)(int,int,const char *);
void (*mlm_udp_received_sym)(int,const char *,int);
void (*mlm_serial_received_sym)(int,const char *,int);
void (*mlm_timer_expired_sym)(int);
void (*mlm_vguest_pulse_received_sym)(int,int,int,int,int,int);
void (*mlm_vguest_turn_on_received_sym)(int,int,int,int,int);
void (*mlm_vguest_turn_off_received_sym)(int,int,int,int,int);
void (*mlm_vguest_set_input_assign_received_sym)(int,int,int,int,int,int);
void (*mlm_vguest_set_input_mode_received_sym)(int,int,int,int,int);
void (*mlm_vguest_set_fader_level_received_sym)(int,int,int,int,int);

MainObject *global_host=NULL;


void SigHandler(int signum)
{
  switch(signum) {
      case SIGINT:
      case SIGTERM:
	if(global_hard_shutdown) {
	  syslog(LOG_DAEMON|LOG_NOTICE,"shutting down normally");
	  closelog();
	  exit(0);
	}
	else {
	  global_shutdown_flag=true;
	}
	break;
  }
}


int GetVGuestIndex(int handle)
{
  for(unsigned i=0;i<global_vguests.size();i++) {
    if(handle==global_vguests[i]->id()) {
      return i;
    }
  }
  return -1;
}


MainObject::MainObject(QObject *parent,const char *name)
  :QObject(parent,name)
{
  global_host=this;
  mlm_use_realtime=false;
  global_init_complete=false;
  global_hard_shutdown=false;
  mlm_metadata_timer=NULL;
  int offset_argc=qApp->argc();

  //
  // Read Command Options
  //
  CmdSwitch *cmd=
    new CmdSwitch(qApp->argc(),qApp->argv(),"mlmh",MLMH_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--use-realtime") {
      mlm_use_realtime=true;
      offset_argc=qApp->argc()-1;
      cmd->setProcessed(i,true);
    }
  }
  delete cmd;

  if((offset_argc<3)||(offset_argc>4)) {
    fprintf(stderr,"mlmh: invalid command parameters\n");
    exit(256);
  }

  QString connection;
  QString module_path;
  QString module_args="";
  switch(offset_argc) {
      case 3:
	connection=qApp->argv()[qApp->argc()-2];
	module_path=qApp->argv()[qApp->argc()-1];
	break;

      case 4:
	connection=qApp->argv()[qApp->argc()-3];
	module_path=qApp->argv()[qApp->argc()-2];
	module_args=qApp->argv()[qApp->argc()-1];
	break;
  }

  //
  // Open Syslog Connection
  //
  QString logname=MLGetBasePart(module_path);
  logname=logname.left(logname.findRev("."));
  openlog(logname,0,LOG_DAEMON);

  //
  // Load Configuration
  //
  mlm_config=new MldConfig(MLD_CONF_FILE);
  mlm_config->load();

  //
  // UDP Send Socket
  //
  global_send_socket=new Q3SocketDevice(Q3SocketDevice::Datagram);

  //
  // Receive Timer
  //
  global_receive_timer=new QTimer(this,"global_receive_timer");
  connect(global_receive_timer,SIGNAL(timeout()),
	  this,SLOT(receiveTimerData()));

  //
  // Callback Timers
  //
  QSignalMapper *mapper=new QSignalMapper(this,"callback_timer_mapper");
  connect(mapper,SIGNAL(mapped(int)),
	  this,SLOT(callbackTimerData(int)));
  for(int i=0;i<MLM_MAX_TIMERS;i++) {
    global_callback_timers[i]=new QTimer(this);
    mapper->setMapping(global_callback_timers[i],i);
    connect(global_callback_timers[i],SIGNAL(timeout()),mapper,SLOT(map()));
  }

  //
  // Start the Database Connection
  //
  if(mlm_config->mysqlHostname().isEmpty()) {
    syslog(LOG_DAEMON|LOG_ERR,"invalid database server, exiting");
    exit(256);
  }
  mlm_db=QSqlDatabase::addDatabase(mlm_config->mysqlDbtype());
  /*
  if(!mlm_db) {
    syslog(LOG_DAEMON|LOG_ERR,"unable to connect to database server, exiting");
    exit(256);
  }
  */
  mlm_db.setDatabaseName(mlm_config->mysqlDbname());
  mlm_db.setUserName(mlm_config->mysqlUsername());
  mlm_db.setPassword(mlm_config->mysqlPassword());
  mlm_db.setHostName(mlm_config->mysqlHostname());
  if(!mlm_db.open()) {
    mlm_db.removeDatabase(mlm_config->mysqlDbname());
    syslog(LOG_DAEMON|LOG_ERR,"unable to login to database server");
    exit(256);
  }

  //
  // Load Module Arguments
  //
  strncpy(global_module_args,module_args,256);

  //
  // Load Connection
  //
  mlm_connection=new ConnectionData(ConnectionData::StorageDatabase);
  mlm_connection->setName(connection);
  if(!mlm_connection->load()) {
    syslog(LOG_DAEMON|LOG_ERR,"connection \"%s\" is invalid, exiting",
	   (const char *)connection);
    exit(256);
  }

  //
  // Load Plugin
  //
  if((mlm_plugin_handle=dlopen(module_path,RTLD_LAZY))==NULL) {
    syslog(LOG_DAEMON|LOG_ERR,"%s, exiting",dlerror());
    exit(256);
  }
  *(void **)(&mlm_startup_sym)=dlsym(mlm_plugin_handle,"MLMStartup");
  *(void **)(&mlm_shutdown_sym)=dlsym(mlm_plugin_handle,"MLMShutdown");
  *(void **)(&mlm_metadata_update_requested_sym)=
    dlsym(mlm_plugin_handle,"MLMMetadataUpdateRequested");
  *(void **)(&mlm_watchdog_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMWatchdogStateChanged");
  *(void **)(&mlm_show_changed_sym)=dlsym(mlm_plugin_handle,"MLMShowChanged");
  *(void **)(&mlm_console_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMConsoleChanged");
  *(void **)(&mlm_line_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMLineStateChanged");
  *(void **)(&mlm_dump_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMDumpStateChanged");
  *(void **)(&mlm_record_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMREcordStateChanged");
  *(void **)(&mlm_busy_all_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMBusyAllStateChanged");
  *(void **)(&mlm_screen_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMScreenStateChanged");
  *(void **)(&mlm_mute_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMMuteStateChanged");
  *(void **)(&mlm_conference_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMConferenceStateChanged");
  *(void **)(&mlm_vip_state_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMVipStateChanged");
  *(void **)(&mlm_delay_offset_changed_sym)=
    dlsym(mlm_plugin_handle,"MLMDelayOffsetChanged");
  *(void **)(&mlm_received_string_sym)=
    dlsym(mlm_plugin_handle,"MLMReceivedString");
  *(void **)(&mlm_caller_id_received_sym)=
    dlsym(mlm_plugin_handle,"MLMCallerIdReceived");
  *(void **)(&mlm_chat_message_received_sym)=
    dlsym(mlm_plugin_handle,"MLMChatMessageReceived");
  *(void **)(&mlm_caller_element_received_sym)=
    dlsym(mlm_plugin_handle,"MLMCallerElementReceived");
  *(void **)(&mlm_udp_received_sym)=
    dlsym(mlm_plugin_handle,"MLMUdpReceived");
  *(void **)(&mlm_serial_received_sym)=
    dlsym(mlm_plugin_handle,"MLMSerialReceived");
  *(void **)(&mlm_vguest_pulse_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestPulseReceived");
  *(void **)(&mlm_vguest_turn_on_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestTurnOnReceived");
  *(void **)(&mlm_vguest_turn_off_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestTurnOffReceived");
  *(void **)(&mlm_vguest_set_input_assign_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestSetInputAssignReceived");
  *(void **)(&mlm_vguest_set_input_mode_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestSetInputModeReceived");
  *(void **)(&mlm_vguest_set_fader_level_received_sym)=
    dlsym(mlm_plugin_handle,"MLMVGuestSetFaderLevelReceived");
  *(void **)(&mlm_timer_expired_sym)=
    dlsym(mlm_plugin_handle,"MLMTimerExpired");

  //
  // Load Driver
  //
  switch(mlm_connection->systemType()) {
	case ConnectionData::TypeAsterisk:
	  global_bus_driver=new AsteriskDriver(0,this);
	  break;

	case ConnectionData::TypeTelos2101:
	  global_bus_driver=new Telos2101Driver(0,this);
	  break;

	case ConnectionData::TypeTelos100Key:
	  global_bus_driver=new Telos100Driver(Telos100Driver::KeyType,0,this);
	  break;

	case ConnectionData::TypeTelos100Dim:
	  global_bus_driver=new Telos100Driver(Telos100Driver::DimType,0,this);
	  break;

	case ConnectionData::TypeTelos100OneXSix:
	  global_bus_driver=
	    new Telos100Driver(Telos100Driver::OneXSixType,0,this);
	  break;

	case ConnectionData::TypeGentnerTs612:
	  global_bus_driver=new Ts612Driver(0,this);
	  break;

	case ConnectionData::TypeVirtual:
	  global_bus_driver=new VirtualDriver(0,this);
	  break;

	case ConnectionData::TypeUnknown:
	  break;
  }
  global_bus_driver->setClientType(ClientData::TypeLogicModule);
  global_bus_driver->setPromiscuousMode(true);
  connect(global_bus_driver,SIGNAL(watchdogStateChanged(unsigned,bool)),
	  this,SLOT(watchdogStateChangedData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentShow(unsigned,const QString &,bool)),
	  this,SLOT(currentShowData(unsigned,const QString &,bool)));
  connect(global_bus_driver,
	  SIGNAL(currentConsole(unsigned,int,ConsoleData::Mode)),
	  SLOT(currentConsoleData(unsigned,int,ConsoleData::Mode)));
  connect(global_bus_driver,
	  SIGNAL(currentLineState(unsigned,BusDriver::LineState,
				  unsigned,int,int)),
	  this,SLOT(currentLineStateData(unsigned,BusDriver::LineState,
					 unsigned,int,int)));
  connect(global_bus_driver,SIGNAL(currentDumpDelayState(unsigned,bool)),
	  this,SLOT(currentDumpDelayStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentRecordState(unsigned,bool)),
	  this,SLOT(currentRecordStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentBusyAllState(unsigned,bool)),
	  this,SLOT(currentBusyAllStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentScreenState(unsigned,bool)),
	  this,SLOT(currentScreenStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentMuteState(unsigned,bool)),
	  this,SLOT(currentMuteStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentConferenceState(unsigned,bool)),
	  this,SLOT(currentConferenceStateData(unsigned,bool)));
  connect(global_bus_driver,SIGNAL(currentVipState(unsigned,bool)),
	  this,SLOT(currentVipStateData(unsigned,bool)));
  connect(global_bus_driver,
	  SIGNAL(receivedString(unsigned,BusDriver::Element,unsigned,
				const QString &)),
	  this,
	  SLOT(receivedStringData(unsigned,BusDriver::Element,unsigned,
				  const QString &)));
  connect(global_bus_driver,
	  SIGNAL(metadataUpdateRequested(unsigned,ClientData::ClientType)),
	  this,
	  SLOT(metadataUpdateRequestedData(unsigned,ClientData::ClientType)));
  connect(global_bus_driver,SIGNAL(connected(unsigned,bool)),
	  this,SLOT(connectedData(unsigned,bool)));

  //
  // Connect to System
  //
  switch(mlm_connection->systemType()) {
      case ConnectionData::TypeTelos2101:
      case ConnectionData::TypeVirtual:
      case ConnectionData::TypeAsterisk:
	global_bus_driver->connectToHost(mlm_connection->hostName(),
					 mlm_connection->userName(),
					 mlm_connection->userPassword(),
					 mlm_connection->tcpPort());
	break;
	
      case ConnectionData::TypeTelos100Key:
      case ConnectionData::TypeTelos100Dim:
      case ConnectionData::TypeTelos100OneXSix:
      case ConnectionData::TypeGentnerTs612:
	global_bus_driver->connectToSerial(mlm_connection->ttyPort());
	break;
	
      case ConnectionData::TypeUnknown:
	break;
  }

  //
  // Shutdown Poll Timer
  //
  mlm_shutdown_timer=new QTimer(this,"mlm_shutdown_timer");
  connect(mlm_shutdown_timer,SIGNAL(timeout()),this,SLOT(shutdownTimerData()));
  mlm_shutdown_timer->start(MLMH_SHUTDOWN_POLL_INTERVAL);

  //
  // Set Signal Handling
  //
  ::signal(SIGINT,SigHandler);
  ::signal(SIGTERM,SigHandler);

  syslog(LOG_DAEMON|LOG_INFO,
	 "starting up from \"%s\" using connection \"%s\"",
	 (const char *)module_path,
	 (const char *)connection);
}


void MainObject::watchdogStateChangedData(unsigned id,bool state)
{
  global_watchdog_state=state;
  if((mlm_watchdog_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_watchdog_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentShowData(unsigned id,const QString &show,bool state)
{
  global_show=show;
  global_show_state=state;

  if(state) {
    global_bus_driver->connectToConsole(mlm_connection->console(),
				   mlm_connection->consoleName());
  }
  else {
    fprintf(stderr,"mlmh: unable to connect to console\n");
    exit(256);
  }

  if((mlm_show_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_show_changed_sym)((const char *)show,(int)state);
    fflush(stdout);
  }
  if((mlm_startup_sym!=NULL)&&(!global_init_complete)) {
    (*mlm_startup_sym)(global_module_args);
    fflush(stdout);
  }
  global_init_complete=true;
  syslog(LOG_DAEMON|LOG_NOTICE,"startup complete");
}


void MainObject::currentConsoleData(unsigned id,int console,
				    ConsoleData::Mode mode)
{
  if(mlm_metadata_timer==NULL) {
    mlm_metadata_timer=new QTimer(this,"mlm_metadata_timer");
    connect(mlm_metadata_timer,SIGNAL(timeout()),
	    this,SLOT(requestMetadataData()));
    mlm_metadata_timer->start(MLMH_REQUEST_METADATA_DELAY,true);
  }
  if((mlm_console_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_console_changed_sym)(console,(int)mode);
    fflush(stdout);
  }
}


void MainObject::currentLineStateData(unsigned id,BusDriver::LineState lstate,
				      unsigned line,int bank,int console)
{
  global_lstate[line]=(int)lstate;
  global_bank[line]=bank;
  global_line_console[line]=console;
  if((mlm_line_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_line_state_changed_sym)((int)lstate,(int)line,bank,console);
    fflush(stdout);
  }
}


void MainObject::currentDumpDelayStateData(unsigned id,bool state)
{
  global_dump_state=state;
  if((mlm_dump_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_dump_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentRecordStateData(unsigned id,bool state)
{
  global_record_state=state;
  if((mlm_record_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_record_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentBusyAllStateData(unsigned id,bool state)
{
  global_busy_all_state=state;
  if((mlm_busy_all_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_busy_all_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentScreenStateData(unsigned id,bool state)
{
  global_screen_state=state;
  if((mlm_screen_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_screen_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentMuteStateData(unsigned id,bool state)
{
  global_mute_state=state;
  if((mlm_mute_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_mute_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentConferenceStateData(unsigned id,bool state)
{
  global_conference_state=state;
  if((mlm_conference_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_conference_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::currentVipStateData(unsigned id,bool state)
{
  global_vip_state=state;
  if((mlm_vip_state_changed_sym!=NULL)&&global_init_complete) {
    (*mlm_vip_state_changed_sym)((int)state);
    fflush(stdout);
  }
}


void MainObject::receivedStringData(unsigned id,BusDriver::Element elem,
				    unsigned line,const QString &str)
{
  int n=0;
  bool ok=false;

  switch(elem) {
      case BusDriver::DelayOffsetGlobalElement:
	if((mlm_delay_offset_changed_sym!=NULL)&&global_init_complete) {
	  n=str.toInt(&ok);
	  if(ok) {
	    (*mlm_delay_offset_changed_sym)(n);
	    fflush(stdout);
	    global_delay_offset=n;
	  }
	}
	break;

      case BusDriver::NameElement:
      case BusDriver::Addr1Element:
      case BusDriver::Addr2Element:
      case BusDriver::CityElement:
      case BusDriver::StateElement:
      case BusDriver::ZipElement:
      case BusDriver::EmailElement:
      case BusDriver::GenderElement:
      case BusDriver::StationElement:
      case BusDriver::CommentElement:
      case BusDriver::CellphoneElement:
      case BusDriver::QualityElement:
      case BusDriver::CounterElement:
      case BusDriver::AgeElement:
      case BusDriver::RingTimerElement:
      case BusDriver::AirTimerElement:
      case BusDriver::HoldTimerElement:
	if((mlm_caller_element_received_sym!=NULL)&&global_init_complete) {
	  n=str.findRev("|");
	  (*mlm_caller_element_received_sym)((int)line,(int)elem,
			     (const char *)str.right(str.length()-n-1));
	  fflush(stdout);
	}
	break;

      case BusDriver::PhoneElement:
	if((mlm_caller_id_received_sym!=NULL)&&global_init_complete) {
	  n=str.findRev("|");
	  (*mlm_caller_id_received_sym)((int)line,
			     (const char *)str.right(str.length()-n-1));
	  fflush(stdout);
	}
	break;

      case BusDriver::MessageGlobalElement:
	if((mlm_chat_message_received_sym!=NULL)&&global_init_complete) {
	  n=str.find("|");
	  (*mlm_chat_message_received_sym)((const char *)str.left(n),
					   str.right(str.length()-n-1));
	  fflush(stdout);
	}
	break;

      default:
	if(elem>=MLM_FIRST_ELEMENT) {
	  if((mlm_received_string_sym!=NULL)&&global_init_complete) {
	    (*mlm_received_string_sym)((int)elem,(int)line,(const char *)str);
	    fflush(stdout);
	  }
	}
	break;
  }
}


void MainObject::metadataUpdateRequestedData(unsigned id,
					     ClientData::ClientType type)
{
  if((mlm_metadata_update_requested_sym!=NULL)&&global_init_complete) {
    (*mlm_metadata_update_requested_sym)();
  }
}


void MainObject::connectedData(unsigned id,bool state)
{
  if(state) {
    global_bus_driver->connectToShow(mlm_connection->showName(),
				     mlm_connection->showPassword(),
				     mlm_connection->location());
    //
    // Set Realtime Priority
    //
    if(mlm_use_realtime) {
      sched_param attr;
      memset(&attr,0,sizeof(attr));
      attr.sched_priority=5;
      if(pthread_setschedparam(pthread_self(),SCHED_RR,&attr)==0) {
	syslog(LOG_DAEMON|LOG_INFO,"using realtime permissions");
      }
      else {
	syslog(LOG_DAEMON|LOG_WARNING,"failed to set realtime priority: %s",
	       strerror(errno));
      }
    }
  }
  else {
    fprintf(stderr,"mlmh: unable to connect to system\n");
    exit(256);
  }
}


void MainObject::callbackTimerData(int timernum)
{
  if((mlm_timer_expired_sym!=NULL)&&global_init_complete) {
    (*mlm_timer_expired_sym)(timernum);
    fflush(stdout);
  }
}


void MainObject::shutdownTimerData()
{
  if(global_shutdown_flag) {
    mlm_shutdown_timer->stop();
    for(int i=0;i<MLM_MAX_TIMERS;i++) {
      global_callback_timers[i]->stop();
    }
    if((mlm_shutdown_sym!=NULL)&&(global_init_complete)) {
      (*mlm_shutdown_sym)();
    }
    QTimer *timer=new QTimer(this,"exit_timer");
    connect(timer,SIGNAL(timeout()),this,SLOT(exitTimerData()));
    timer->start(500,true);
  }
}


void MainObject::exitTimerData()
{
  syslog(LOG_DAEMON|LOG_NOTICE,"shutting down normally");
  closelog();
  exit(0);
}


void MainObject::receiveTimerData()
{
  char data[1024];
  int n=0;

  for(unsigned i=0;i<global_receive_sockets.size();i++) {
    while((n=global_receive_sockets[i]->readBlock(data,1024))>0) {
      if((mlm_udp_received_sym!=NULL)&&global_init_complete) {
	(*mlm_udp_received_sym)(global_receive_sockets[i]->socket(),data,n);
      }
    }
  }
  for(unsigned i=0;i<global_tty_devices.size();i++) {
    if(global_tty_devices[n]!=NULL) {
      while((n=global_tty_devices[i]->readBlock(data,1024))>0) {
	if((mlm_serial_received_sym!=NULL)&&global_init_complete) {
	  (*mlm_serial_received_sym)(i,data,n);
	}
      }
    }
  }
  for(unsigned i=0;i<global_vguests.size();i++) {
    global_vguests[i]->readyRead();
  }
}


void MainObject::requestMetadataData()
{
  ClientData *data=NULL;

  //
  // Find the longest-running peer
  //
  for(unsigned i=1;i<global_bus_driver->clientList()->size();i++) {
    if(global_bus_driver->clientList()->at(i)->clientType()==
       ClientData::TypeUser) {
      if(data==NULL) {
	data=global_bus_driver->clientList()->at(i);
      }
      else {
	if(global_bus_driver->clientList()->at(i)->loginDateTime()<
	   data->loginDateTime()) {
	  data=global_bus_driver->clientList()->at(i);
	}
      }
    }
  }
  global_bus_driver->requestMetadataUpdate(data);
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
