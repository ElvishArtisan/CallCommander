// login.cpp
//
// Database Connection Dialog for CallCommander.
//
//   (C) Copyright 2002-2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WIN32
#include <unistd.h>
#endif  // WIN32
#include <sys/types.h>

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>

#include "dbdialog.h"

DbDialog::DbDialog(QWidget *parent) 
  : QDialog(parent)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption("DB Connection Parameters");

  //
  // Open Configuration
  //
  db_config=new MldConfig();
  db_config->load();

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Hostname
  //
  db_hostname_edit=new QLineEdit(this);
  db_hostname_edit->setGeometry(120,10,sizeHint().width()-130,19);
  db_hostname_edit->setFocus();
  QLabel *label=new QLabel(tr("Hostname:"),this);
  label->setGeometry(10,10,105,19);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Username
  //
  db_username_edit=new QLineEdit(this);
  db_username_edit->setGeometry(120,32,sizeHint().width()-130,19);
  db_username_edit->setFocus();
  label=new QLabel(tr("Username:"),this);
  label->setGeometry(10,32,105,19);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Password
  //
  db_password_edit=new QLineEdit(this);
  db_password_edit->setGeometry(120,54,sizeHint().width()-130,19);
  db_password_edit->setEchoMode(QLineEdit::Password);
  db_password_edit->setFocus();
  label=new QLabel(tr("Password:"),this);
  label->setGeometry(10,54,105,19);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Database Name
  //
  db_dbname_edit=new QLineEdit(this);
  db_dbname_edit->setGeometry(120,76,sizeHint().width()-130,19);
  db_dbname_edit->setFocus();
  label=new QLabel(tr("Database Name:"),this);
  label->setGeometry(10,76,105,19);
  label->setFont(font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // OK Button
  //
  QPushButton *ok_button=new QPushButton(this,"ok_button");
  ok_button->setGeometry(sizeHint().width()-160,sizeHint().height()-40,70,30);
  ok_button->setFont(font);
  ok_button->setText("&OK");
  ok_button->setDefault(true);
  connect(ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  QPushButton *cancel_button=new QPushButton(this,"cancel_button");
  cancel_button->setGeometry(sizeHint().width()-80,sizeHint().height()-40,
			     70,30);
  cancel_button->setFont(font);
  cancel_button->setText("&Cancel");
  connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  db_username_edit->setText(db_config->mysqlUsername());
  db_password_edit->setText(db_config->mysqlPassword());
  db_hostname_edit->setText(db_config->mysqlHostname());
  db_dbname_edit->setText(db_config->mysqlDbname());
}


DbDialog::~DbDialog()
{
  delete db_config;
}


QSize DbDialog::sizeHint() const
{
  return QSize(300,155);
} 


QSizePolicy DbDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void DbDialog::okData()
{
  db_config->setMysqlHostname(db_hostname_edit->text());
  db_config->setMysqlUsername(db_username_edit->text());
  db_config->setMysqlPassword(db_password_edit->text());
  db_config->setMysqlDbname(db_dbname_edit->text());
  if(!db_config->save()) {
    QMessageBox::warning(this,tr("Call Administrator"),
			 tr("Unable to save database settings!"));
  }
  done(0);
}


void DbDialog::cancelData()
{
  done(1);
}
