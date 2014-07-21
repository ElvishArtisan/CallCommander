// change_nickname.cpp
//
// A dialog to select a redial number for CallCommander
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include "change_nickname.h"
#include "globals.h"

ChangeNickname::ChangeNickname(QString *nickname,
			       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  change_nickname=nickname;
  setCaption(tr("Change Screen Name"));

  //
  // Create Fonts
  //
  QFont button_font=QFont("helvetica",16,QFont::Bold);
  button_font.setPixelSize(16);
  QFont list_font=QFont("helvetica",16,QFont::Normal);
  list_font.setPixelSize(16);

  //
  // Nickname
  //
  change_nickname_edit=new QLineEdit(this,"change_nickname_edit");
  change_nickname_edit->setGeometry(100,10,sizeHint().width()-115,20);
  change_nickname_edit->setText(*nickname);
  QLabel *label=new QLabel(change_nickname_edit,"Screen Name:",
			   this,"change_nickname_label");
  label->setGeometry(10,10,85,20);
  label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);

  //
  // OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize ChangeNickname::sizeHint() const
{
  return QSize(200,100);
}


QSizePolicy ChangeNickname::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ChangeNickname::okData()
{
  *change_nickname=change_nickname_edit->text();
  done(0);
}


void ChangeNickname::cancelData()
{
  done(-1);
}
