// mysql_login.cpp
//
// mySQL Administrative Login widget for CallCommander.
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

#include <QtCore/QString>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <Qt3Support/q3textedit.h>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3buttongroup.h>

#include <mllabel.h>
#include <math.h>

#include "mysql_login.h"

MySqlLogin::MySqlLogin(QString msg,QString *username,QString *password,
		       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  setCaption("Call Administrator - mySQL Admin");
  login_name=username;
  login_password=password;

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",10,QFont::Normal);
  font.setPixelSize(10);

  //
  // Message Label
  //
  MLLabel *label=new MLLabel(msg,this,"message_label");
  label->setFont(font);
  label->setGeometry(10,10,sizeHint().width()-20,sizeHint().height()-130);
  label->setAlignment(Qt::AlignCenter);
  

  //
  // MySql Login Name
  //
  login_name_edit=new QLineEdit(this,"login_name_edit");
  login_name_edit->setFont(font);
  login_name_edit->setGeometry(100,sizeHint().height()-110,100,19);
  login_name_edit->setMaxLength(16);
  login_name_edit->setFocus();
  QLabel *login_name_label=new QLabel(login_name_edit,"User &Name:",this,
				       "login_name_label");
  login_name_label->setFont(font);
  login_name_label->setGeometry(10,sizeHint().height()-109,85,19);
  login_name_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // MySql Login Password
  //
  login_password_edit=new QLineEdit(this,"login_password_edit");
  login_password_edit->setFont(font);
  login_password_edit->setGeometry(100,sizeHint().height()-90,100,19);
  login_password_edit->setMaxLength(16);
  login_password_edit->setEchoMode(QLineEdit::Password);
  QLabel *login_password_label=new QLabel(login_password_edit,"&Password:",
					  this,"login_password_label");
  login_password_label->setFont(font);
  login_password_label->setGeometry(10,sizeHint().height()-88,85,19);
  login_password_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // OK Button
  //
  QPushButton *ok_button=new QPushButton(this,"ok_button");
  ok_button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  ok_button->setFont(font);
  ok_button->setText("&OK");
  ok_button->setDefault(true);
  connect(ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // CANCEL Button
  //
  QPushButton *cancel_button=new QPushButton(this,"cancel_button");
  cancel_button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,
			     80,50);
  cancel_button->setFont(font);
  cancel_button->setText("&Cancel");
  connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


MySqlLogin::~MySqlLogin()
{
  delete login_name_edit;
  delete login_password_edit;
}


QSize MySqlLogin::sizeHint() const
{
  return QSize(360,210);
} 


QSizePolicy MySqlLogin::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MySqlLogin::okData()
{
  *login_name=login_name_edit->text();
  *login_password=login_password_edit->text();
  done(0);
}


void MySqlLogin::cancelData()
{
  done(1);
}


void MySqlLogin::paintEvent(QPaintEvent *paintevent)
{
  QPainter *p=new QPainter(this);
    
  p->end();
}
