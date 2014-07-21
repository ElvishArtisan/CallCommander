// mlmld.cpp
//
// The CallCommander Logic Module Host Launcher
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
#include <sys/types.h>
#include <sys/wait.h>

#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtSql/QSqlQuery>

#include <mlconf.h>
#include <ml.h>
#include <mldconfig.h>
#include <cmdswitch.h>

#include <mlmld.h>

//
// Global Variables
//
MldConfig *mld_config;
volatile bool restart=false;

void SigHandler(int signum)
{
  pid_t local_pid;

  switch(signum) {
      case SIGCHLD:
	local_pid=waitpid(-1,NULL,WNOHANG);
	while(local_pid>0) {
	  local_pid=waitpid(-1,NULL,WNOHANG);
	}
	signal(SIGCHLD,SigHandler);
	return;

      case SIGHUP:
	restart=true;
	break;

      case SIGINT:
      case SIGTERM:
	syslog(LOG_DAEMON|LOG_NOTICE,"stopping all logic modules");
	system("killall mlmh");
	syslog(LOG_DAEMON|LOG_NOTICE,"shutting down normally");
	closelog();
	unlink(MLMLD_PID_FILE);
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
  openlog("mlmld",0,LOG_DAEMON);

  //
  // Read Command Options
  //
  mlm_debug=false;
  CmdSwitch *cmd=
    new CmdSwitch(qApp->argc(),qApp->argv(),"mlmld",MLMLD_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="-d") {
      mlm_debug=true;
    }
  }
  delete cmd;

  //
  // Reload Socket
  //
  mlm_reload_socket=new Q3SocketDevice(Q3SocketDevice::Datagram);
  mlm_reload_socket->bind(QHostAddress(),ML_MLMLD_RELOAD_PORT);
  mlm_reload_socket->setBlocking(false);

  //
  // Initialize Data Structures
  //
  syslog(LOG_DAEMON|LOG_NOTICE,"starting up");
  Init(true);

  //
  // Detach
  //
  if(!mlm_debug) {
    if(!MLDetach()) {
      fprintf(stderr,"mlmld: unable to detach\n");
      exit(256);
    }
  }

  //
  // Log PID
  //
  FILE *pidfile;
  if((pidfile=fopen(MLMLD_PID_FILE,"w"))==NULL) {
    perror("mlmld");
  }
  else {
    fprintf(pidfile,"%d",getpid());
    fclose(pidfile);
  }

  signal(SIGCHLD,SigHandler);
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


void MainObject::restartTimerData()
{
  char buffer[1024];

  int n=mlm_reload_socket->readBlock(buffer,1024);
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
  QDateTime current_datetime(QDate::currentDate(),QTime::currentTime());

  //
  //
  // Load Configuration
  //
  hostname=GetCanonicalName();
  mlm_config=new MldConfig();
  mlm_config->load();

  //
  // Start the Database Connection
  //
  if(mlm_config->mysqlHostname().isEmpty()) {
    if(initial_startup||mlm_debug) {
      fprintf(stderr,"mlmld: invalid database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,"invalid database server, exiting");
    exit(256);
  }
  mlm_db=QSqlDatabase::addDatabase(mlm_config->mysqlDbtype());
  /*
  if(!mlm_db) {
    if(initial_startup||mlm_debug) {
      fprintf(stderr,"mlmld: unable to connect to database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,
	   "unable to connect to database server, exiting");
    exit(256);
  }
  */
  mlm_db.setDatabaseName(mlm_config->mysqlDbname());
  mlm_db.setUserName(mlm_config->mysqlUsername());
  mlm_db.setPassword(mlm_config->mysqlPassword());
  mlm_db.setHostName(mlm_config->mysqlHostname());
  if(!mlm_db.open()) {
    mlm_db.removeDatabase(mlm_config->mysqlDbname());
    if(initial_startup||mlm_debug) {
      fprintf(stderr,"mlmld: unable to login to database server\n");
    }
    syslog(LOG_DAEMON|LOG_CRIT,"unable to login to database server, exiting");
    exit(256);
  }

  //
  // Start the Host Instances
  //
  QString cmd;
  QString use_realtime="";

  sql=QString().sprintf("select CONNECTION_NAME,MODULE_PATH,LOG_PATH,\
                         MODULE_ARGS,USE_REALTIME \
                         from LOGIC_MODULES where HOSTNAME=\"%s\"",
			(const char *)hostname);
  q=new QSqlQuery(sql);
  while(q->next()) {
    if(q->value(4).toString()=="Y") {
      use_realtime="--use-realtime";
    }
    cmd=QString().sprintf("mlmh %s \"%s\" \"%s\"",
			  (const char *)use_realtime,
			  (const char *)q->value(0).toString(),
			  (const char *)q->value(1).toString());
    if(!q->value(3).toString().isEmpty()) {
      cmd+=QString().sprintf(" \"%s\"",(const char *)q->value(3).toString());
    }
    if(q->value(2).toString().isEmpty()) {
      cmd+=" > /dev/null 2> /dev/null";
    }
    else {
      cmd+=QString().sprintf(" >> %s 2>> %s",
			     (const char *)q->value(2).toString(),
			     (const char *)q->value(2).toString());
    }
    cmd+=" &";
    syslog(LOG_DAEMON|LOG_NOTICE,
	   "starting logic module \"%s\"",(const char *)q->value(1).toString());
    // printf("CMD: %s\n",(const char *)cmd);
    if(fork()==0) {
      system(cmd);
      exit(0);
    }
 }
  delete q;
}


void MainObject::Release()
{
  //
  // Kill Host Instances
  //
  system("killall mlmh");
  syslog(LOG_DAEMON|LOG_NOTICE,"stopping all logic modules");

  //
  // Close Database Connection
  //
  mlm_db.removeDatabase(mlm_config->mysqlDbname());

  //
  // Close Config File
  //
  delete mlm_config;
  mlm_config=NULL;
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
  return (username==mlm_config->mysqlUsername())&&
    (password==mlm_config->mysqlPassword());
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  new MainObject(NULL,"main");
  return a.exec();
}
