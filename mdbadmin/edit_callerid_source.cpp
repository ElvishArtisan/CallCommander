// edit_callerid_source.cpp
//
// Edit a CallCommander CallerId Source.
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
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <mlttydevice.h>
#include <change_password.h>

#include "globals.h"
#include "edit_callerid_source.h"

EditCallerIdSource::EditCallerIdSource(const QString &hostname,int host_id,
				       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QString sql;
  QSqlQuery *q;

  edit_hostname=hostname;
  edit_host_id=host_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit User");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // TTY Port
  //
  edit_ttyport_edit=new QLineEdit(this,"edit_ttyport_edit");
  edit_ttyport_edit->setGeometry(80,10,sizeHint().width()-90,20);
  edit_ttyport_edit->setFont(font);
  QLabel *label=
    new QLabel(edit_ttyport_edit,"TTY Port:",this,"edit_ttyport_label");
  label->setGeometry(10,10,65,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // TTY Settings Button
  //
  QPushButton *button=new QPushButton(this,"tty_settings_button");
  button->setGeometry(10,35,sizeHint().width()-20,50);
  button->setFont(label_font);
  button->setText("Serial Port &Settings");
  connect(button,SIGNAL(clicked()),this,SLOT(settingsData()));

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
  sql=QString().sprintf("select TTY_PORT,BAUD_RATE,DATA_BITS,STOP_BITS,PARITY\
                         from CID_SOURCES\
                         where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
			(const char *)edit_hostname,edit_host_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_ttyport_edit->setText(q->value(0).toString());
    edit_settings.setSpeed(q->value(1).toInt());
    edit_settings.setWordLength(q->value(2).toInt());
    edit_settings.setStopBits(q->value(3).toInt());
    edit_settings.setParity((MLTTYDevice::Parity)q->value(4).toInt());
  }
  delete q;
}


EditCallerIdSource::~EditCallerIdSource()
{
}


QSize EditCallerIdSource::sizeHint() const
{
  return QSize(250,160);
} 


QSizePolicy EditCallerIdSource::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditCallerIdSource::settingsData()
{
  EditTtySettings *edit=new EditTtySettings(&edit_settings,this);
  edit->exec();
  delete edit;
}


void EditCallerIdSource::okData()
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("update CID_SOURCES set TTY_PORT=\"%s\",BAUD_RATE=%d,\
                         DATA_BITS=%d,STOP_BITS=%d,PARITY=%d \
                         where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
			(const char *)edit_ttyport_edit->text(),
			edit_settings.speed(),edit_settings.wordLength(),
			edit_settings.stopBits(),edit_settings.parity(),
			(const char *)edit_hostname,edit_host_id);
  q=new QSqlQuery(sql);
  delete q;
  done(0);
}


void EditCallerIdSource::cancelData()
{
  done(-1);
}
