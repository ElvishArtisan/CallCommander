// add_callerid_source.cpp
//
// Add a CallCommander CallerId Source.
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

#include <ml.h>

#include "add_callerid_source.h"

AddCallerIdSource::AddCallerIdSource(QString *hostname,int *host_id,
		 QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_hostname=hostname;
  add_host_id=host_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Add CallerID Source");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Host Name
  //
  add_hostname_edit=new QLineEdit(this,"add_hostname_edit");
  add_hostname_edit->setGeometry(80,10,sizeHint().width()-90,20);
  add_hostname_edit->setFont(font);
  add_hostname_edit->setMaxLength(255);
  QLabel *label=
    new QLabel(add_hostname_edit,"&Hostname:",this,"add_hostname_label");
  label->setGeometry(10,10,65,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Host ID
  //
  add_hostid_spin=new QSpinBox(this,"add_hostid_spin");
  add_hostid_spin->setGeometry(80,32,40,20);
  add_hostid_spin->setRange(1,ML_MAX_CALLERID_SOURCES);
  label=new QLabel(add_hostid_spin,"&Source #:",this,"add_hostid_label");
  label->setGeometry(10,32,65,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
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
}


AddCallerIdSource::~AddCallerIdSource()
{
}


QSize AddCallerIdSource::sizeHint() const
{
  return QSize(250,130);
} 


QSizePolicy AddCallerIdSource::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddCallerIdSource::okData()
{
  QString sql;
  QSqlQuery *q;

  if(add_hostname_edit->text().isEmpty()) {
    QMessageBox::warning(this,tr("Invalid Hostname"),
			 tr("You must provide a valid hostname!"));
    return;
  }
  sql=QString().sprintf("select HOSTNAME from CID_SOURCES\
                         where (HOSTNAME=\"%s\")&&(HOST_ID=%d)",
			(const char *)add_hostname_edit->text(),
			add_hostid_spin->value());
  q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,tr("Source Exists"),
			 tr("A source already exists on that Host/ID!"));
    delete q;
    return;
  }
  delete q;
  *add_hostname=add_hostname_edit->text();
  *add_host_id=add_hostid_spin->value();
  done(0);
}


void AddCallerIdSource::cancelData()
{
  done(-1);
}
