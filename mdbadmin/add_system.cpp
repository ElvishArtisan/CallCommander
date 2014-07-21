// add_system.cpp
//
// Add a CallCommander Virtual System.
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
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>
#include <QtGui/QLabel>

#include <math.h>

#include <ml.h>

#include "add_system.h"

AddSystem::AddSystem(QString *showname,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_showname=showname;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Add Virtual System");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Show Name
  //
  add_showname_edit=new QLineEdit(this,"add_showname_edit");
  add_showname_edit->setGeometry(90,10,sizeHint().width()-100,20);
  add_showname_edit->setFont(font);
  add_showname_edit->setMaxLength(64);
  QLabel *label=
    new QLabel(add_showname_edit,"&Show Name:",this,"add_showname_label");
  label->setGeometry(10,10,75,20);
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


AddSystem::~AddSystem()
{
}


QSize AddSystem::sizeHint() const
{
  return QSize(250,108);
} 


QSizePolicy AddSystem::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddSystem::okData()
{
  QString sql;
  QSqlQuery *q;

  if(add_showname_edit->text().isEmpty()) {
    QMessageBox::warning(this,tr("Invalid Showname"),
			 tr("You must provide a valid showname!"));
    return;
  }
  sql=QString().sprintf("select SHOW_NAME from VIRTUAL_SYSTEMS\
                         where SHOW_NAME=\"%s\"",
			(const char *)add_showname_edit->text());
  q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,tr("System Exists"),
		 tr("A virtual system already exists with that show name!"));
    delete q;
    return;
  }
  delete q;
  *add_showname=add_showname_edit->text();
  done(0);
}


void AddSystem::cancelData()
{
  done(-1);
}
