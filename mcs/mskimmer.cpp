// mcs.cpp
//
// A Qt-based application for recording telephone calls
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <QtGui/QApplication>
#include <QtCore/QSignalMapper>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <globals.h>
#include <askbox.h>
#include <mskimmer.h>
#include <list_connections.h>

//
// Global Resources
//
MlConfig *ml_config;

//
// Icons
//
#include "../icons/callcommander-22x22.xpm"


void SigHandler(int signo)
{
  pid_t pLocalPid;

  switch(signo) {
  case SIGCHLD:
    pLocalPid=waitpid(-1,NULL,WNOHANG);
    while(pLocalPid>0) {
      pLocalPid=waitpid(-1,NULL,WNOHANG);
    }
    signal(SIGCHLD,SigHandler);
    return;
  }
}


MainWidget::MainWidget(QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  struct stat conf_stat;
  bool file_found=false;
  char temp[256];
  bool ask_for_profile=true;
  QString filename;

  setCaption("Call Skimmer");

  mcs_connection=new ConnectionData(ConnectionData::StorageFilesystem);

  //
  // Determine Which Configuration File to Use
  //
  // First, the command line argument
  //
  if(qApp->argc()>1) {
    memset(&conf_stat,0,sizeof(struct stat));
    if(stat(qApp->argv()[qApp->argc()-1],&conf_stat)==0) {
      mcs_config=qApp->argv()[qApp->argc()-1];
      file_found=true;
    }
  }

  //
  // Next, the .mlrc File
  //
  if(!file_found) {
    if(strlen(getenv("HOME"))<259) {
      sprintf(temp,"%s/.mlrc",getenv("HOME"));
      if(stat(temp,&conf_stat)==0) {
	mcs_config=temp;
	file_found=true;
      }
    }
  }

  //
  // Finally, the global configuration
  //
  if(!file_found) {
    mcs_config=DEFAULT_CONFIG_FILE;
  }

  //
  // Create And Set Icon
  //
  mcs_callcommander_map=new QPixmap(callcommander_xpm);
  setIcon(*mcs_callcommander_map);

  //
  // Global Section
  //
  ml_config=new MlConfig(mcs_config);
  ml_config->load();

  //
  // Try to load the specified connection profile
  //
  if(!ml_config->connectionName().isEmpty()) {
    mcs_connection->setName(ml_config->connectionName());
    ask_for_profile=!mcs_connection->load();
  }

  //
  // Connection Dialog
  //
  if(ask_for_profile) {
    ListConnections *list=
      new ListConnections(mcs_connection,tr("Open\nConnection"),
			  ml_config,this);
    if(list->exec()<0) {
      exit(0);
    }
  }

  //
  // The Bus Driver
  //
  switch(mcs_connection->systemType()) {
      case ConnectionData::TypeAsterisk:
	mcs_driver=new AsteriskDriver(0,this,"mcs_driver");
	break;

      case ConnectionData::TypeTelos2101:
	mcs_driver=new Telos2101Driver(0,this,"mcs_driver");
	break;

      case ConnectionData::TypeTelos100Key:
	mcs_driver=
	  new Telos100Driver(Telos100Driver::KeyType,0,this,"mcs_driver");
	break;

      case ConnectionData::TypeTelos100Dim:
	mcs_driver=
	  new Telos100Driver(Telos100Driver::DimType,0,this,"mcs_driver");
	break;

      case ConnectionData::TypeTelos100OneXSix:
	mcs_driver=
	  new Telos100Driver(Telos100Driver::OneXSixType,0,this,
			     "mcs_driver");
	break;

      case ConnectionData::TypeGentnerTs612:
	mcs_driver=new Ts612Driver(0,this,"mcs_driver");
	break;

      case ConnectionData::TypeVirtual:
	mcs_driver=new VirtualDriver(0,this,"mcs_driver");
	break;

      case ConnectionData::TypeUnknown:
	QMessageBox::warning(this,tr("System Error"),
			     tr("Unknown System Type!"));
	exit(256);
  }
  mcs_driver->setClientType(ClientData::TypeAudioRecorder);
  connect(mcs_driver,SIGNAL(connected(unsigned,bool)),
	  this,SLOT(connectedData(unsigned,bool)));

  switch(mcs_connection->systemType()) {
      case ConnectionData::TypeVirtual:
      case ConnectionData::TypeTelos2101:
      case ConnectionData::TypeAsterisk:
	mcs_driver->connect(mcs_connection->hostName(),
			    mcs_connection->userName(),
			    mcs_connection->userPassword(),
			    mcs_connection->tcpPort(),
			    mcs_connection->showName(),
			    mcs_connection->showPassword(),
			    mcs_connection->location(),
			    mcs_connection->console(),
			    mcs_connection->consoleName());
	break;

      case ConnectionData::TypeTelos100Key:
      case ConnectionData::TypeTelos100Dim:
      case ConnectionData::TypeTelos100OneXSix:
      case ConnectionData::TypeGentnerTs612:
	mcs_driver->connect(mcs_connection->ttyPort(),
			    mcs_connection->showName(),
			    mcs_connection->showPassword(),
			    mcs_connection->location(),
			    mcs_connection->console(),
			    mcs_connection->consoleName());
	break;

      case ConnectionData::TypeUnknown:
	QMessageBox::warning(this,tr("System Error"),
			     tr("Unknown System Type!"));
	exit(256);
  }

  //
  // Setup Signal Handling
  //
  signal(SIGCHLD,SigHandler);

  //
  // Generate Title Font
  //
  QFont font("Helvetica",18,QFont::Bold);
  font.setPixelSize(18);

  //
  // Record Widgets
  //
  QLabel *label=new QLabel("Main Bank",this,"main_bank_label");
  label->setGeometry(10,4,sizeHint().width()-20,20);
  label->setFont(font);
  label->show();
  
  main_widget=new McsWidget(0,mcs_connection,mcs_driver,
			    this,"main_widget");
  main_widget->setGeometry(0,20,main_widget->sizeHint().width(),
			   main_widget->sizeHint().height());
  
  label=new QLabel("Conference Bank",this,"conf_bank_label");
  label->setGeometry(10,main_widget->sizeHint().height()+30,
		     sizeHint().width()-20,20);
  label->setFont(font);
  label->show();
  
  conf_widget=new McsWidget(1,mcs_connection,mcs_driver,
			    this,"conf_widget");
  conf_widget->setGeometry(0,50+main_widget->sizeHint().height(),
			   conf_widget->sizeHint().width(),
			   conf_widget->sizeHint().height());
  main_widget->show();
  conf_widget->show();
  
  //
  // Generate Button Font
  //
  font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  
  //
  // The Close Button
  //
  QPushButton *close_button=new QPushButton(this,"close_button");
  close_button->setGeometry(200,main_widget->sizeHint().height()+
			    conf_widget->sizeHint().height()+55,70,40);
  close_button->setFont(font);
  close_button->setText("&Close");
  connect(close_button,SIGNAL(clicked()),this,SLOT(closeData()));
  close_button->show();

  mcs_driver->requestLineState();
}


QSize MainWidget::sizeHint() const
{
  return QSize(285,450);
  return QSize(main_widget->sizeHint().width(),
	       main_widget->sizeHint().height()+
	       conf_widget->sizeHint().height()+100);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::connectedData(unsigned id,bool state)
{
  if(state) {
    mcs_driver->connectToShow(mcs_connection->showName(),
			      mcs_connection->showPassword(),
			      mcs_connection->location());
  }
  else {
    QMessageBox::warning(this,"Login Failed","Login failed!");
    qApp->quit();
  }
}


void MainWidget::closeData()
{
  qApp->quit();
}


int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
#ifndef WIN32
  for(int i=0;i<argc;i++) {
    if((!strcmp(argv[i],"-v"))||(!strcmp(argv[i],"--version"))) {
      printf("CallCommander Call Skimmer - version %s\n",VERSION);
      exit(0);
    }
  }
#endif  // WIN32

  MainWidget *w=new MainWidget(NULL,"main");
  a.setMainWidget(w);
  w->show();
  return a.exec();
}



