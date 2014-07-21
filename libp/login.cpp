// login.cpp
//
// Login widget for CallCommander.
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

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif  // WIN32
#include <sys/types.h>

#include <QtGui/QDialog>
#include <QtCore/QString>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3textedit.h>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3buttongroup.h>
#include <math.h>

#include "dbdialog.h"
#include "login.h"

Login::Login(QString *username,QString *password,bool show_db_button,
	     QWidget *parent) 
  : QDialog(parent)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption("Login");
  login_name=username;
  login_password=password;

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Login Name
  //
  login_name_edit=new QLineEdit(this,"login_name_edit");
  login_name_edit->setGeometry(100,10,sizeHint().width()-110,19);
  login_name_edit->setMaxLength(32);
  login_name_edit->setFocus();
  QLabel *login_name_label=new QLabel(login_name_edit,"User &Name:",this,
				       "login_name_label");
  login_name_label->setGeometry(10,10,85,19);
  login_name_label->setFont(font);
  login_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Login Password
  //
  login_password_edit=new QLineEdit(this,"login_password_edit");
  login_password_edit->setGeometry(100,31,sizeHint().width()-110,19);
  login_password_edit->setMaxLength(16);
  login_password_edit->setEchoMode(QLineEdit::Password);
  QLabel *login_password_label=new QLabel(login_password_edit,"&Password:",this,
				       "login_password_label");
  login_password_label->setGeometry(10,31,85,19);
  login_password_label->setFont(font);
  login_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Database Configuration Button
  //
  login_db_button=new QPushButton(this);
  login_db_button->setGeometry(10,sizeHint().height()-40,110,30);
  login_db_button->setFont(font);
  login_db_button->setText("Configure DB");
  connect(login_db_button,SIGNAL(clicked()),this,SLOT(dbData()));
  login_db_button->setEnabled(CanEditConfig());
  if(!show_db_button) {
    login_db_button->hide();
  }

  //
  // OK Button
  //
  QPushButton *ok_button=new QPushButton(this,"ok_button");
  ok_button->setGeometry(sizeHint().width()-150,sizeHint().height()-40,60,30);
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
}


Login::~Login()
{
  delete login_name_edit;
  delete login_password_edit;
}


QSize Login::sizeHint() const
{
  return QSize(300,105);
} 


QSizePolicy Login::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void Login::dbData()
{
  DbDialog *d=new DbDialog(this);
  d->exec();
  delete d;
}


void Login::okData()
{
  *login_name=login_name_edit->text();
  *login_password=login_password_edit->text();
  done(0);
}


void Login::cancelData()
{
  done(1);
}


bool Login::CanEditConfig() const
{
#ifdef LINUX
  return geteuid()==0;
#endif  // LINUX

#ifdef OS_X
  return true;
#endif  // OS_X

#ifdef WIN32
  BOOL ret;
  SID_IDENTIFIER_AUTHORITY NtAuthority=SECURITY_NT_AUTHORITY;
  PSID AdministratorsGroup;
  
  ret=AllocateAndInitializeSid(&NtAuthority,2,SECURITY_BUILTIN_DOMAIN_RID,
			       DOMAIN_ALIAS_RID_ADMINS,
			       0,0,0,0,0,0,&AdministratorsGroup);
  if(ret) {
    if(!CheckTokenMembership(NULL,AdministratorsGroup,&ret)) {
      ret=FALSE;
    }
    FreeSid(AdministratorsGroup);
  }
  return ret;
#endif  // WIN32
}
