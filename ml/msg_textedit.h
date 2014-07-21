// msg_textedit.h
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

#ifndef MSG_TEXTEDIT_H
#define MSG_TEXTEDIT_H

#include <QtGui/QTextEdit>
#include <QtGui/QMenu>

class MsgTextEdit : public QTextEdit
{
  Q_OBJECT
 public:
  MsgTextEdit(QWidget *parent=0);
  void setNickname(const QString &name);
  void clear();

 signals:
  void nicknameChanged(const QString &name);
  void clearClicked();
  void doubleClicked();

 private slots:
  void changeNicknameData();
  void clearData();

 protected:
  void mousePressEvent(QMouseEvent *e);
  void mouseDoubleClickEvent(QMouseEvent *e);

 private:
  QString msg_nickname;
  QMenu *msg_popup_menu;
};


#endif  // MSG_TEXTEDIT_H
