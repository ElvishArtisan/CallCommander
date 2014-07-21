// edit_system.cpp
//
// Edit a CallCommander Virtual System.
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

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QPainter>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <ml_reload_packet.h>

#include "globals.h"
#include "list_connections.h"
#include "edit_system.h"

EditSystem::EditSystem(QString showname,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Virtual System");

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
  // Showname
  //
  edit_showname_edit=new QLineEdit(this,"edit_showname_edit");
  edit_showname_edit->setGeometry(130,10,sizeHint().width()-140,20);
  edit_showname_edit->setFont(font);
  edit_showname_edit->setReadOnly(true);
  edit_showname_edit->setText(showname);
  QLabel *label=
    new QLabel(edit_showname_edit,tr("Show Name:"),this,"edit_showname_label");
  label->setGeometry(10,10,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Show Password
  //
  edit_showpasswd1_edit=new QLineEdit(this,"edit_showpasswd1_edit");
  edit_showpasswd1_edit->setGeometry(130,32,150,20);
  edit_showpasswd1_edit->setFont(font);
  edit_showpasswd1_edit->setEchoMode(QLineEdit::Password);
  edit_showpasswd1_edit->setMaxLength(32);
  edit_showpasswd2_edit=new QLineEdit(this,"edit_showpasswd2_edit");
  edit_showpasswd2_edit->setGeometry(130,54,150,20);
  edit_showpasswd2_edit->setFont(font);
  edit_showpasswd2_edit->setEchoMode(QLineEdit::Password);
  edit_showpasswd2_edit->setMaxLength(32);
  label=new QLabel(edit_showname_edit,tr("Show Password:"),
		   this,"edit_showname_label");
  label->setGeometry(10,32,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Location
  //
  edit_location_edit=new QLineEdit(this,"edit_location_edit");
  edit_location_edit->setGeometry(130,76,sizeHint().width()-140,20);
  edit_location_edit->setFont(font);
  edit_location_edit->setMaxLength(255);
  label=new QLabel(edit_location_edit,tr("Studio:"),
		   this,"edit_location_label");
  label->setGeometry(10,76,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Hostname
  //
  edit_hostname_edit=new QLineEdit(this,"edit_hostname_edit");
  edit_hostname_edit->setGeometry(130,98,sizeHint().width()-140,20);
  edit_hostname_edit->setFont(font);
  edit_hostname_edit->setMaxLength(255);
  label=new QLabel(edit_hostname_edit,tr("Server Hostname:"),
		   this,"edit_hostname_label");
  label->setGeometry(10,98,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Connection
  //
  edit_connection_edit=new QLineEdit(this,"edit_connection_edit");
  edit_connection_edit->setGeometry(130,125,sizeHint().width()-220,20);
  edit_connection_edit->setFont(font);
  edit_connection_edit->setMaxLength(255);
  label=new QLabel(edit_connection_edit,tr("Connection:"),
		   this,"edit_connection_label");
  label->setGeometry(10,125,115,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  QPushButton *button=new QPushButton(this,"connection_button");
  button->setGeometry(sizeHint().width()-80,120,70,30);
  button->setFont(label_font);
  button->setText("&Select");
  connect(button,SIGNAL(clicked()),this,SLOT(selectedData()));

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
  sql=QString().sprintf("select SHOW_PASSWORD,HOSTNAME,LOCATION,CONNECTION \
                         from VIRTUAL_SYSTEMS where SHOW_NAME=\"%s\"",
			(const char *)edit_showname_edit->text());
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_showpasswd1_edit->setText(q->value(0).toString());
    edit_showpasswd2_edit->setText(q->value(0).toString());
    edit_hostname_edit->setText(q->value(1).toString());
    edit_location_edit->setText(q->value(2).toString());
    edit_connection_edit->setText(q->value(3).toString());
  }
  delete q;
}


EditSystem::~EditSystem()
{
}


QSize EditSystem::sizeHint() const
{
  return QSize(450,226);
} 


QSizePolicy EditSystem::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditSystem::selectedData()
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


void EditSystem::okData()
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

  if(edit_showpasswd1_edit->text()!=edit_showpasswd2_edit->text()) {
    QMessageBox::warning(this,tr("Password Mismatch"),
			 tr("The passwords don't match!"));
    return;
  }
  sql=QString().sprintf("update VIRTUAL_SYSTEMS set HOSTNAME=\"%s\",\
                         LOCATION=\"%s\",CONNECTION=\"%s\",\
                         SHOW_PASSWORD=\"%s\" \
                         where SHOW_NAME=\"%s\"",
			(const char *)edit_hostname_edit->text(),
			(const char *)edit_location_edit->text(),
			(const char *)edit_connection_edit->text(),
			(const char *)edit_showpasswd1_edit->text(),
			(const char *)edit_showname_edit->text());
  q=new QSqlQuery(sql);
  delete q;

  if(!addr.isNull()) {
    MlSendReloadPacket(admin_config,addr,ML_MLD_RELOAD_PORT,
		       admin_reload_socket);
  }

  done(0);
}


void EditSystem::cancelData()
{
  done(-1);
}
