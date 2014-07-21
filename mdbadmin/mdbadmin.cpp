// mdbadmin.cpp
//
// The Database Administrator Utility for CallCommander.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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
#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <sys/types.h>
#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/QPixmap>
#include <QtSql/QSqlQuery>

#include <ml.h>
#include <mldconfig.h>
#include <login.h>
#include <ml_reload_packet.h>
#include <cmdswitch.h>
#include <escapestring.h>

#include "opendb.h"
#include "createdb.h"
#include "mdbadmin.h"
#include "globals.h"
#include "list_directory.h"
#include "list_show_configs.h"
#include "list_users.h"
#include "list_callerid_sources.h"
#include "list_systems.h"
#include "list_modules.h"
#include "system_info.h"

//
// Global Classes
//
MldConfig *admin_config;
QString admin_loginname;
bool exiting=false;
Q3SocketDevice *admin_reload_socket;
QString admin_admin_username;
QString admin_admin_password;
bool found_check_db;

//
// Icons
//
#include "../icons/callcommander-22x22.xpm"


MainWidget::MainWidget(QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  QString sql;
  QSqlQuery *q;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont default_font("Helvetica",12,QFont::Normal);
  default_font.setPixelSize(12);
  QFont title_font=QFont("Helvetica",16,QFont::Bold);
  title_font.setPixelSize(16);
  qApp->setFont(default_font);

  //
  // Create And Set Icon
  //
  mdb_callcommander_map=new QPixmap(callcommander_xpm);
  setIcon(*mdb_callcommander_map);

  //
  // Reload Socket
  //
  admin_reload_socket=new Q3SocketDevice(Q3SocketDevice::Datagram);

  //
  // Load Configs
  //
  admin_config=new MldConfig();
  admin_config->load();

  //
  // Open Database
  //

  /*
  QStringList drivers=QSqlDatabase::drivers();
  for(unsigned i=0;i<drivers.size();i++) {
      QMessageBox::information(this,"DRIVERS",drivers[i]);
  }
  */
  OpenDb(admin_config->mysqlDbname(),admin_config->mysqlUsername(),
	 admin_config->mysqlPassword(),admin_config->mysqlHostname(),true);

  //
  // Log In
  //
  QString password;
  Login *login=new Login(&admin_loginname,&password,true,this);
  if(login->exec()!=0) {
    exit(0);
  }
  sql=QString("select LOGIN_NAME from USERS where ")+
    "(LOGIN_NAME=\""+EscapeString(admin_loginname)+"\")&&"+
    "(PASSWORD=\""+EscapeString(password)+"\")";
  q=new QSqlQuery(sql);
  if(q->size()<=0) {
    QMessageBox::information(this,"Login Failed","Invalid Login!");
    exiting=true;
    delete q;
  }
  else {
    delete q;
    sql=
      QString().sprintf("select ADMIN_PRIV from USERS where LOGIN_NAME=\"%s\"",
			(const char *)admin_loginname);
    q=new QSqlQuery(sql);
    q->first();
    if(q->value(0).toString().lower()!="y") {
      QMessageBox::information(this,"Login Failed",
		      "This user does not have \nadministrative permissions!");
      exiting=true;
    }
    delete q;
    setCaption(QString().sprintf("Call Administrator - User: %s",
				 (const char *)admin_loginname));
  }

  //
  // Title
  //
  QLabel *label=new QLabel("CallCommander",this,"main_title_label");
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(title_font);
  label->setAlignment(Qt::AlignCenter);

  label=new QLabel("Database Administrator",this,"sub_title_label");
  label->setGeometry(10,25,sizeHint().width()-20,20);
  label->setFont(default_font);
  label->setAlignment(Qt::AlignCenter);

  //
  // Manage Users Button
  //
  QPushButton *button=new QPushButton(this,"users_button");
  button->setGeometry(10,50,120,60);
  button->setFont(font);
  button->setText("Manage\n&Users");
  connect(button,SIGNAL(clicked()),this,SLOT(manageUsersData()));

  //
  // Manage Shows Button
  //
  button=new QPushButton(this,"shows_button");
  button->setGeometry(150,50,120,60);
  button->setFont(font);
  button->setText("Manage\n&Shows");
  connect(button,SIGNAL(clicked()),this,SLOT(manageShowsData()));

  //
  // Manage Directory Button
  //
  button=new QPushButton(this,"directory_button");
  button->setGeometry(10,120,120,60);
  button->setFont(font);
  button->setText("Manage\n&Directory");
  connect(button,SIGNAL(clicked()),this,SLOT(manageDirectoryData()));

  //
  // CallerID Source Button
  //
  button=new QPushButton(this,"callerid_button");
  button->setGeometry(150,120,120,60);
  button->setFont(font);
  button->setText("Manage &CallerID\nSources");
  connect(button,SIGNAL(clicked()),this,SLOT(manageCallerIdData()));

  //
  // Manage Virtual Systems Vutton
  //
  button=new QPushButton(this,"virtual_button");
  button->setGeometry(10,190,120,60);
  button->setFont(font);
  button->setText("Manage &Virtual\nSystems");
  connect(button,SIGNAL(clicked()),this,SLOT(manageVirtualData()));

  //
  // Logic Modules Button
  //
  button=new QPushButton(this,"logic_modules_button");
  button->setGeometry(150,190,120,60);
  button->setFont(font);
  button->setText("Manage &Logic\nModules");
  connect(button,SIGNAL(clicked()),this,SLOT(manageLogicModuleData()));

  //
  // System Info Button
  //
  button=new QPushButton(this,"sysinfo_button");
  button->setGeometry(80,260,120,60);
  button->setFont(font);
  button->setText("System\n&Info");
  connect(button,SIGNAL(clicked()),this,SLOT(showInfoData()));

  //
  // Quit Button
  //
  button=new QPushButton(this,"quit_button");
  button->setGeometry(10,sizeHint().height()-70,sizeHint().width()-20,60);
  button->setFont(font);
  button->setText("&Quit");
  connect(button,SIGNAL(clicked()),this,SLOT(quitMainWidget()));
}


MainWidget::~MainWidget()
{
}


QSize MainWidget::sizeHint() const
{
  return QSize(280,410);
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::manageUsersData()
{
  ListUsers *list=new ListUsers(this,"list");
  list->exec();
  delete list;
  ReloadMonitors();
}


void MainWidget::manageShowsData()
{
  ListShowConfigs *list=new ListShowConfigs(this,"list");
  list->exec();
  delete list;
}


void MainWidget::manageDirectoryData()
{
  ListDirectory *list=new ListDirectory(this,"list");
  list->exec();
  delete list;
}


void MainWidget::manageCallerIdData()
{
  ListCallerIdSources *list=new ListCallerIdSources(this,"list");
  list->exec();
  delete list;
  ReloadMonitors();
}


void MainWidget::manageVirtualData()
{
  ListSystems *list=new ListSystems(this,"list");
  list->exec();
  delete list;
  ReloadMonitors();
}


void MainWidget::manageLogicModuleData()
{
  ListModules *list=new ListModules(this,"list");
  list->exec();
  delete list;
  ReloadModules();
}


void MainWidget::showInfoData()
{
  SystemInfo *info=new SystemInfo(this,"info");
  info->exec();
  delete info;
}


void MainWidget::quitMainWidget()
{
  exit(0);
}


void MainWidget::ReloadMonitors()
{
  QString sql;
  QSqlQuery *q;
  QHostAddress addr;

  sql="select CID_HOSTNAME from SHOWS where CID_HOSTNAME!=\"\"";
  q=new QSqlQuery(sql);
  while(q->next()) {
    addr=MlResolveIpAddress(q->value(0).toString());
    if(!addr.isNull()) {
      MlSendReloadPacket(admin_config,addr,ML_MCIDMD_RELOAD_PORT,
			 admin_reload_socket);
    }
  }
  delete q;
}


void MainWidget::ReloadModules()
{
}

/*
int main(int argc,char *argv[])
{
  QApplication a(argc,argv);
  
  MainWidget *w=new MainWidget(NULL,"main");
  if(exiting) {
      exit(0);
  }
  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}
*/


int gui_main(int argc,char *argv[])
{
  QApplication a(argc,argv);

  //
  // Start Event Loop
  //
  MainWidget *w=new MainWidget(NULL,"main");
  if(exiting) {
      exit(0);
  }
  a.setMainWidget(w);
  w->setGeometry(w->geometry().x(),w->geometry().y(),
		 w->sizeHint().width(),w->sizeHint().height());
  w->show();
  return a.exec();
}


int cmdline_main(int argc,char *argv[])
{
  QApplication a(argc,argv,false);
  
  //
  // Load Configs
  //
  admin_config=new MldConfig();
  admin_config->load();

  //
  // Open Database
  //
  if(!OpenDb(admin_config->mysqlDbname(),admin_config->mysqlUsername(),
	     admin_config->mysqlPassword(),admin_config->mysqlHostname(),
	     false)) {
    return 1;
  }

  return 0;
}


int main(int argc,char *argv[])
{
  int ret;
  bool found_check_db=false;
  CmdSwitch *cmd=new CmdSwitch(argc,argv,"rdadmin",MDBADMIN_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--check-db") {
      found_check_db=true;
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-admin-user") {
      admin_admin_username=cmd->value(i);
      cmd->setProcessed(i,true);
    }
    if(cmd->key(i)=="--mysql-admin-password") {
      admin_admin_password=cmd->value(i);
      cmd->setProcessed(i,true);
    }
  }
  if(found_check_db) {
    ret=cmdline_main(argc,argv);
  }
  else {
    ret=gui_main(argc,argv);
  }
  return ret;
}

