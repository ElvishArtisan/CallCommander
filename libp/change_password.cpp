// change_password.cpp
//
// Set Password widget for Rivendell.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
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

#include "change_password.h"

ChangePassword::ChangePassword(QString *password,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Create Fonts
  //
  QFont button_font("helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  passwd_password=password;
  setCaption("Change Password");

  //
  // OK Button
  //
  QPushButton *ok_button=new QPushButton(this,"ok_button");
  ok_button->setGeometry(10,60,100,55);
  ok_button->setFont(button_font);
  ok_button->setText("&OK");
  ok_button->setDefault(true);
  connect(ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  QPushButton *cancel_button=new QPushButton(this,"cancel_button");
  cancel_button->setGeometry(120,60,100,55);
  cancel_button->setFont(button_font);
  cancel_button->setText("&Cancel");
  connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Password
  //
  passwd_password_1_edit=new QLineEdit(this,"passwd_password_1_edit");
  passwd_password_1_edit->setGeometry(90,11,100,20);
  passwd_password_1_edit->setMaxLength(16);
  passwd_password_1_edit->setEchoMode(QLineEdit::Password);
  passwd_password_1_edit->setFocus();
  QLabel *passwd_password_1_label=new QLabel(passwd_password_1_edit,"&Password:",this,
				       "passwd_password_1_label");
  passwd_password_1_label->setGeometry(10,11,75,20);
  passwd_password_1_label->setFont(button_font);
  passwd_password_1_label->
    setAlignment(Qt::AlignVCenter|Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Confirm Password
  //
  passwd_password_2_edit=new QLineEdit(this,"passwd_password_2_edit");
  passwd_password_2_edit->setGeometry(90,32,100,20);
  passwd_password_2_edit->setMaxLength(16);
  passwd_password_2_edit->setEchoMode(QLineEdit::Password);
  QLabel *passwd_password_2_label=new QLabel(passwd_password_2_edit,"C&onfirm:",this,
				       "passwd_password_2_label");
  passwd_password_2_label->setGeometry(10,32,75,20);
  passwd_password_2_label->setFont(button_font);
  passwd_password_2_label->
    setAlignment(Qt::AlignVCenter|Qt::AlignRight|Qt::TextShowMnemonic);

}


ChangePassword::~ChangePassword()
{
  delete passwd_password_1_edit;
  delete passwd_password_2_edit;
}


QSize ChangePassword::sizeHint() const
{
  return QSize(230,125);
} 


QSizePolicy ChangePassword::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ChangePassword::okData()
{
  if(passwd_password_1_edit->text()==passwd_password_2_edit->text()) {
    *passwd_password=passwd_password_1_edit->text();
    done(0);
  }
  else {
    QMessageBox::warning(this,"Password Mismatch","The passwords don't match,\nplease try again!","OK",0);
  }
}


void ChangePassword::cancelData()
{
  done(-1);
}
