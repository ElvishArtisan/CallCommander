// purge_directory.cpp
//
// Purge a CallCommander Phone Number Directory Entry.
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

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <math.h>

#include "purge_directory.h"

PurgeDirectory::PurgeDirectory(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Purge Directory");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Data Catagory Selector
  //
  QLabel *label=new QLabel("Purge the following data types:",
			   this,"selector_label");
  label->setGeometry(10,10,sizeHint().width()-20,16);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignCenter);

  purge_nanpa_box=new QCheckBox(this,"purge_nanpa_box");
  purge_nanpa_box->setGeometry(45,35,15,15);
  purge_nanpa_box->setChecked(true);
  label=new QLabel(purge_nanpa_box,"Generic NANPA Data",
		   this,"purge_nanpa_label");
  label->setGeometry(65,34,sizeHint().width()-45,16);
  label->setFont(font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  purge_custom_box=new QCheckBox(this,"purge_custom_box");
  purge_custom_box->setGeometry(45,55,15,15);
  label=new QLabel(purge_custom_box,"Custom Directory Entries",
		   this,"purge_custom_label");
  label->setGeometry(65,54,sizeHint().width()-45,16);
  label->setFont(font);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

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

  //
  // Load Data
  //
}


PurgeDirectory::~PurgeDirectory()
{
}


QSize PurgeDirectory::sizeHint() const
{
  return QSize(290,150);
} 


QSizePolicy PurgeDirectory::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void PurgeDirectory::okData()
{
  QString sql;
  QSqlQuery *q;

  if(purge_nanpa_box->isChecked()) {
    sql="delete from DIRECTORY where CUSTOM=\"N\"";
    q=new QSqlQuery(sql);
    delete q;
  }
  if(purge_custom_box->isChecked()) {
    sql="delete from DIRECTORY where CUSTOM=\"Y\"";
    q=new QSqlQuery(sql);
    delete q;
  }
  done(0);
}


void PurgeDirectory::cancelData()
{
  done(-1);
}
