// message.h
//
// Message Widget for CallCommander. 
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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtGui/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QPaintEvent>
#include <QtCore/QTimer>
#include <QtGui/QPalette>
#include <QtGui/QLabel>

#include "msg_textedit.h"
#include "sendmsg_textedit.h"

/*
 * Widget-wide defines
 */
#define SEND_BOX_HEIGHT 60


class Message : public QWidget
{
  Q_OBJECT
 public:
  Message(QColor base,unsigned hang_time=5000,
	  QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  QString nickname() const;
  void setNickname(const QString &name);
  void setFont(const QFont &font);
  void setSendLines(int lines);
  void setScrollback(int lines);
  void setMessage(QString msg);
  void appendMessage(QString msg);
  QString messageBuffer() const;
  void setMessageBuffer(QString buffer);
  int numLines() const;
  void clear();
  
 signals:
  void message(QString msg);
  void cleared();
  void touched();
  
 public slots:
  void setGeometry(int x,int y,int w,int h);
  void setGeometry(const QRect &rect);
  void doubleClickedData();
  
 protected:
  void paintEvent(QPaintEvent *);

 private slots:
  void sendData();
  void timerData();
  void nicknameChangedData(const QString &name);
  void clearClickedData();
  
 private:
  void DrawBackdrop();
  QString msg_nickname;
  SendMsgTextEdit *msg_send_box;
  MsgTextEdit *msg_rcv_box;
  QLabel *msg_backdrop_label;
  QTimer *msg_hang_timer;
  QColor msg_base_color;
  unsigned msg_hang_time;
  bool msg_hanging;
  int msg_send_lines;
};


#endif  // MESSAGE_H
