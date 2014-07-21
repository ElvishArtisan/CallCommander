// msg_textedit.cpp
//
// A MessageBox Widget for CallCommander
//
//   (C) Copyright 2007,2011 Fred Gleason <fredg@paravelsystems.com>
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

#include <Qt3Support/Q3PopupMenu>

#include "change_nickname.h"
#include "msg_textedit.h"

MsgTextEdit::MsgTextEdit(QWidget *parent)
  : QTextEdit(parent)
{
  setWordWrapMode(QTextOption::WordWrap);
  setReadOnly(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setTextInteractionFlags(Qt::NoTextInteraction);

  msg_popup_menu=new QMenu(this);
  msg_popup_menu->insertItem("&Change Screen Name",this,
			     SLOT(changeNicknameData()),8,-1);
  msg_popup_menu->insertItem("C&lear",this,
			     SLOT(clearData()),8,-1);
}


void MsgTextEdit::setNickname(const QString &name)
{
  msg_nickname=name;
}


void MsgTextEdit::clear()
{
  QTextEdit::clear();
}


void MsgTextEdit::changeNicknameData()
{
  ChangeNickname *dialog=new ChangeNickname(&msg_nickname,this);
  if(dialog->exec()==0) {
    emit nicknameChanged(msg_nickname);
  }
  delete dialog;
}


void MsgTextEdit::clearData()
{
  clear();
  emit clearClicked();
}


void MsgTextEdit::mousePressEvent(QMouseEvent *e)
{
  if(e->button()==Qt::RightButton) {
    msg_popup_menu->popup(e->globalPos());
    e->accept();
  }
  else {
    e->ignore();
  }
}


void MsgTextEdit::mouseDoubleClickEvent(QMouseEvent *e)
{
  e->accept();
  emit doubleClicked();
}
