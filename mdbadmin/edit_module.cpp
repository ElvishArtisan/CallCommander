// edit_module.cpp
//
// Edit a CallCommander Logic Module Configuration.
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

#ifdef WIN32
#include <windows.h>
#else
#include <netdb.h>
#endif  // WIN32

#ifdef LINUX
#include <mlpaths.h>
#endif  // LINUX

#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtGui/QFileDialog>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <mlconf.h>
#include <ml_reload_packet.h>

#include "globals.h"
#include "list_connections.h"
#include "edit_module.h"

EditModule::EditModule(int id,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_id=id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Logic Module");

  //
  // Get Local Hostname
  //
#ifndef WIN32
  edit_local_hostname=GetCanonicalName();
#endif

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Description
  //
  edit_description_edit=new QLineEdit(this,"edit_description_edit");
  edit_description_edit->setGeometry(130,10,sizeHint().width()-140,20);
  edit_description_edit->setFont(font);
  QLabel *label=
    new QLabel(edit_description_edit,tr("Description:"),this,
	       "edit_description_label");
  label->setGeometry(10,10,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Hostname
  //
  edit_hostname_edit=new QLineEdit(this,"edit_hostname_edit");
  edit_hostname_edit->setGeometry(130,32,sizeHint().width()-140,20);
  edit_hostname_edit->setFont(font);
  edit_hostname_edit->setMaxLength(255);
  connect(edit_hostname_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(hostnameTextChangedData(const QString &)));
  label=new QLabel(edit_hostname_edit,tr("Server Hostname:"),
		   this,"edit_hostname_label");
  label->setGeometry(10,32,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Realtime Permissions
  //
  edit_realtime_check=new QCheckBox(this,"edit_realtime_check");
  edit_realtime_check->setGeometry(135,55,16,16);
  label=new QLabel(edit_realtime_check,tr("Use Realtime Permissions"),
		   this,"edit_realtime_label");
#ifdef OS_X
  label->setGeometry(155,58,sizeHint().width()-165,16);
#else
  label->setGeometry(155,52,sizeHint().width()-165,16);
#endif  // OS_X
  label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  label->setFont(label_font);

  //
  // Connection
  //
  edit_connection_edit=new QLineEdit(this,"edit_connection_edit");
  edit_connection_edit->setGeometry(130,82,sizeHint().width()-220,20);
  edit_connection_edit->setFont(font);
  edit_connection_edit->setMaxLength(255);
  label=new QLabel(edit_connection_edit,tr("Connection:"),
		   this,"edit_connection_label");
  label->setGeometry(10,82,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  QPushButton *button=new QPushButton(this,"connection_button");
  button->setGeometry(sizeHint().width()-80,78,70,30);
  button->setFont(label_font);
  button->setText("&Select");
  connect(button,SIGNAL(clicked()),this,SLOT(connectionSelectedData()));

  //
  // Module Path
  //
  edit_module_path_edit=new QLineEdit(this,"edit_module_path_edit");
  edit_module_path_edit->setGeometry(130,116,sizeHint().width()-220,20);
  edit_module_path_edit->setFont(font);
  edit_module_path_edit->setMaxLength(255);
  label=new QLabel(edit_module_path_edit,tr("Module Path:"),
		   this,"edit_module_path_label");
  label->setGeometry(10,116,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_module_path_button=new QPushButton(this,"module_path_button");
  edit_module_path_button->setGeometry(sizeHint().width()-80,111,70,30);
  edit_module_path_button->setFont(label_font);
  edit_module_path_button->setText("&Select");
  connect(edit_module_path_button,SIGNAL(clicked()),
	  this,SLOT(modulePathSelectedData()));

  //
  // Module Arguments
  //
  edit_module_args_edit=new QLineEdit(this,"edit_module_args_edit");
  edit_module_args_edit->setGeometry(130,150,sizeHint().width()-220,20);
  edit_module_args_edit->setFont(font);
  edit_module_args_edit->setMaxLength(255);
  label=new QLabel(edit_module_args_edit,tr("Module Args:"),
		   this,"edit_module_args_label");
  label->setGeometry(10,150,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Log Path
  //
  edit_log_path_edit=new QLineEdit(this,"edit_log_path_edit");
  edit_log_path_edit->setGeometry(130,184,sizeHint().width()-220,20);
  edit_log_path_edit->setFont(font);
  edit_log_path_edit->setMaxLength(255);
  label=new QLabel(edit_log_path_edit,tr("Log Path:"),
		   this,"edit_log_path_label");
  label->setGeometry(10,184,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_log_path_button=new QPushButton(this,"log_path_button");
  edit_log_path_button->setGeometry(sizeHint().width()-80,179,70,30);
  edit_log_path_button->setFont(label_font);
  edit_log_path_button->setText("&Select");
  connect(edit_log_path_button,SIGNAL(clicked()),
	  this,SLOT(logPathSelectedData()));

  //
  //  OK Button
  //
  button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  QString sql;
  QSqlQuery *q;
  sql=QString().sprintf("select DESCRIPTION,CONNECTION_NAME,HOSTNAME,\
                         MODULE_PATH,MODULE_ARGS,LOG_PATH,USE_REALTIME from \
                         LOGIC_MODULES where ID=%d",
			id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_description_edit->setText(q->value(0).toString());
    edit_connection_edit->setText(q->value(1).toString());
    edit_hostname_edit->setText(q->value(2).toString());
    edit_module_path_edit->setText(q->value(3).toString());
    edit_module_args_edit->setText(q->value(4).toString());
    edit_log_path_edit->setText(q->value(5).toString());
    edit_realtime_check->setChecked(q->value(6).toString()=="Y");
  }
  delete q;

  hostnameTextChangedData(edit_hostname_edit->text());
}


EditModule::~EditModule()
{
}


QSize EditModule::sizeHint() const
{
  return QSize(450,290);
} 


QSizePolicy EditModule::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditModule::hostnameTextChangedData(const QString &str)
{
  edit_module_path_button->setEnabled(str==edit_local_hostname);
  edit_log_path_button->setEnabled(str==edit_local_hostname);
}


void EditModule::connectionSelectedData()
{
  ConnectionData *conn=new ConnectionData(ConnectionData::StorageDatabase);
  conn->setName(edit_connection_edit->text());
  conn->setOfferStartConnection(false);
  ListConnections *list_conn=new ListConnections(conn,tr("OK"),NULL,this);
  if(list_conn->exec()<0) {
    delete list_conn;
    return;
  }
  delete list_conn;
  edit_connection_edit->setText(conn->name());
  delete conn;
}


void EditModule::modulePathSelectedData()
{
#ifdef LINUX
  QString module_path=ML_MODULES_DIR;
  if(!edit_module_path_edit->text().isEmpty()) {
    module_path=edit_module_path_edit->text();
  }
  module_path=QFileDialog::getOpenFileName(module_path,
     QString().sprintf("CallCommander Logic Modules (*.%s)",ML_MLM_EXTENSION),
			       this,"Choose a Logic Module");
  if(module_path.isNull()) {
    return;
  }
  edit_module_path_edit->setText(module_path);
#endif  // LINUX
}


void EditModule::logPathSelectedData()
{
  QString log_path=
    QFileDialog::getSaveFileName(edit_log_path_edit->text(),
				 "Text Files (*.txt)",
				 this,tr("Choose a Log File"));
  if(log_path.isNull()) {
    return;
  }
  edit_log_path_edit->setText(log_path);
}


void EditModule::okData()
{
  QSqlQuery *q;
  QString sql;
  QHostAddress addr=MlResolveIpAddress(edit_hostname_edit->text());

  //
  // Check for valid Virtual Hostname
  //
  if(addr.isNull()) {
    if(QMessageBox::question(this,tr("Address Error"),
			     tr("The Server Hostname is invalid.\nDo you still want to save?"),QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
      return;
    }
  }

  sql=QString().sprintf("update LOGIC_MODULES set HOSTNAME=\"%s\",\
                         DESCRIPTION=\"%s\",CONNECTION_NAME=\"%s\",\
                         MODULE_PATH=\"%s\",MODULE_ARGS=\"%s\",\
                         LOG_PATH=\"%s\",USE_REALTIME=\"%s\" where ID=%d",
			(const char *)edit_hostname_edit->text(),
			(const char *)edit_description_edit->text(),
			(const char *)edit_connection_edit->text(),
			(const char *)edit_module_path_edit->text(),
			(const char *)edit_module_args_edit->text(),
			(const char *)edit_log_path_edit->text(),
			(const char *)MLYesNo(edit_realtime_check->isChecked()),
			edit_id);
  q=new QSqlQuery(sql);
  delete q;

  if(!addr.isNull()) {
    MlSendReloadPacket(admin_config,addr,ML_MLMLD_RELOAD_PORT,
		       admin_reload_socket);
  }
  done(0);
}


void EditModule::cancelData()
{
  done(-1);
}


QString EditModule::GetCanonicalName() const
{
#ifndef HOST_NAME_MAX
#define HOST_NAME_MAX 1024
#endif  // WIN32
  char hostname[HOST_NAME_MAX+1];
  struct hostent *hostent;

  gethostname(hostname,HOST_NAME_MAX);
  if((hostent=gethostbyname(hostname))==NULL) {
    return hostname;
  }
  return QString(hostent->h_name);
}


