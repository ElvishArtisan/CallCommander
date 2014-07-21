// askbox.cpp
//
// Start-up dialog for the Mithlond Project.
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

#include <QtGui/QPushButton>
#include <Qt3Support/q3multilineedit.h>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QComboBox>
#include <QtGui/QPaintEvent>
#include <math.h>

#include "mldconfig.h"
#include "askbox.h"
#include "askdata.h"

AskBox::AskBox(AskData *data,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  QFont label_font;
  QFont button_font;

  ask_data=data;
  setCaption("Connect");

  //
  // Fonts
  //
  label_font=QFont("Helvetica",12,QFont::Normal);
  label_font.setPixelSize(12);

  button_font=QFont("Helvetica",18,QFont::Bold);
  button_font.setPixelSize(18);

  //
  // Host Name
  //
  ask_hostname_edit=new QLineEdit(this,"ask_hostname_edit");
  ask_hostname_edit->setFont(label_font);
  ask_hostname_edit->setGeometry(110,12,130,19);
  ask_hostname_edit->setMaxLength(254);
  ask_hostname_edit->setText(ask_data->hostname());
  ask_hostname_edit->setFocus();
  QLabel *ask_hostname_label=new QLabel(ask_hostname_edit,"&Hostname:",this,
				       "ask_hostname_label");
  ask_hostname_label->setFont(label_font);
  ask_hostname_label->setGeometry(10,12,95,19);
  ask_hostname_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Password
  //
  ask_password_edit=new QLineEdit(this,"ask_password_edit");
  ask_password_edit->setEchoMode(QLineEdit::Password);
  ask_password_edit->setFont(label_font);
  ask_password_edit->setGeometry(110,33,130,19);
  ask_password_edit->setMaxLength(254);
  ask_password_edit->setText(ask_data->password());
  ask_password_edit->setFocus();
  QLabel *ask_password_label=new QLabel(ask_password_edit,"&Password:",this,
				       "ask_password_label");
  ask_password_label->setFont(label_font);
  ask_password_label->setGeometry(10,33,95,19);
  ask_password_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Host Port
  //
  ask_port_spin=new QSpinBox(this,"ask_port_spin");
  ask_port_spin->setFont(label_font);
  ask_port_spin->setGeometry(110,54,60,19);
  ask_port_spin->setRange(0,65535);
  ask_port_spin->setValue(ask_data->port());
  QLabel *ask_port_label=new QLabel(ask_port_spin,"Po&rt:",this,
				       "ask_port_label");
  ask_port_label->setFont(label_font);
  ask_port_label->setGeometry(10,54,95,19);
  ask_port_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Studio Group
  //
  ask_group_box=new QComboBox(this,"ask_group_box");
  ask_group_box->setFont(label_font);
  ask_group_box->setGeometry(110,75,40,19);
  for(int i=0;i<MAX_GROUPS;i++) {
    ask_group_box->insertItem(QString().sprintf("%d",i));
  }
  ask_group_box->setCurrentItem(ask_data->group());
  QLabel *ask_group_label=new QLabel(ask_group_box,"&Studio Group:",this,
				       "ask_group_label");
  ask_group_label->setFont(label_font);
  ask_group_label->setGeometry(10,75,95,19);
  ask_group_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  //
  // Console
  //
  ask_console_box=new QComboBox(this,"ask_console_box");
  ask_console_box->setFont(label_font);
  ask_console_box->setGeometry(110,96,70,19);
  for(int i=0;i<MAX_DESKTOP_DIRECTORS;i++) {
    ask_console_box->insertItem(QString().sprintf("DD%d",i+1));
  }
  ask_console_box->insertItem("Virtual");
  if(ask_data->console()>=MAX_DESKTOP_DIRECTORS) {
    ask_console_box->setCurrentItem(MAX_DESKTOP_DIRECTORS);
  }
  else {
    ask_console_box->setCurrentItem(ask_data->console());
  }
  QLabel *ask_console_label=new QLabel(ask_console_box,"&Director:",this,
				       "ask_console_label");
  ask_console_label->setFont(label_font);
  ask_console_label->setGeometry(10,96,95,19);
  ask_console_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter|Qt::TextShowMnemonic);

  ask_hostname_edit->setFocus();

  //
  // OK Button
  //
  QPushButton *ok_button=new QPushButton(this,"ok_button");
  ok_button->setGeometry(sizeHint().width()/2-105,sizeHint().height()-65,
			 100,55);
  ok_button->setFont(button_font);
  ok_button->setText("&OK");
  ok_button->setDefault(true);
  connect(ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  QPushButton *cancel_button=new QPushButton(this,"cancel_button");
  cancel_button->setGeometry(sizeHint().width()/2+5,sizeHint().height()-65,
			     100,55);
  cancel_button->setFont(button_font);
  cancel_button->setText("&Cancel");
  connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


AskBox::~AskBox()
{
  delete ask_hostname_edit;
  delete ask_password_edit;
  delete ask_port_spin;
  delete ask_group_box;
  delete ask_console_box;
}


QSize AskBox::sizeHint() const
{
  return QSize(260,196);
} 


QSizePolicy AskBox::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AskBox::cancelData()
{
  done(1);
}


void AskBox::okData()
{
  ask_data->setHostname(ask_hostname_edit->text());
  ask_data->setPassword(ask_password_edit->text());
  ask_data->setPort(ask_port_spin->value());
  ask_data->setGroup(ask_group_box->currentItem());
  ask_data->setConsole(ask_console_box->currentItem());

  done(0);
}


void AskBox::paintEvent(QPaintEvent *paintevent)
{
  QPainter *p=new QPainter(this);
    
  p->end();
}




