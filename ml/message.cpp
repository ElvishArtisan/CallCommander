// message.cpp
//
// Message Passing Widget for the Mithlond Project.
//
//   (C) Copyright 2002-2015 Fred Gleason <fredg@paravelsystems.com>
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
#include <Qt3Support/q3textedit.h>
#include <QtGui/QLabel>
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QPixmap>
#include <QtGui/QPaintEvent>
#include <QtGui/QPalette>

#include <math.h>

#include <ml.h>

#include "message.h"

Message::Message(QColor base,unsigned hang_time,
		 QWidget *parent,const char *name)
  : QWidget(parent,name)
{
  msg_base_color=base;
  msg_hang_time=hang_time;
  msg_hanging=false;
  msg_send_lines=2;

  //
  // The Send Box
  //
  msg_send_box=new SendMsgTextEdit(this);
  connect(msg_send_box,SIGNAL(messageSent()),this,SLOT(sendData()));

  //
  // The Receive Box
  //
  msg_rcv_box=new MsgTextEdit(this);
  connect(msg_rcv_box,SIGNAL(doubleClicked()),
	  this,SLOT(doubleClickedData()));
  connect(msg_rcv_box,SIGNAL(nicknameChanged(const QString &)),
	  this,SLOT(nicknameChangedData(const QString &)));
  connect(msg_rcv_box,SIGNAL(clearClicked()),this,SLOT(clearClickedData()));

  //
  // The Backdrop
  //
  //  msg_backdrop_label=new QLabel(this);
  //DrawBackdrop();
  //msg_backdrop_label->hide();

  //
  // The Hang Timer
  //
  msg_hang_timer=new QTimer(this);
  connect(msg_hang_timer,SIGNAL(timeout()),this,SLOT(timerData()));
}


QSize Message::sizeHint() const
{
  return QSize(420,183);
} 


QSizePolicy Message::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


QString Message::nickname() const
{
  return msg_nickname;
}


void Message::setNickname(const QString &name)
{
  msg_nickname=name;
  msg_rcv_box->setNickname(name);
}


void Message::setFont(const QFont &font)
{
  msg_send_box->setFont(font);
  msg_rcv_box->setFont(font);
  QWidget::setFont(font);
  setGeometry(geometry());
}


void Message::setSendLines(int lines)
{
  msg_send_lines=lines;
  setGeometry(geometry());
}


void Message::setScrollback(int lines)
{
  msg_rcv_box->document()->setMaximumBlockCount(lines);
}


void Message::setPreserveText(bool state)
{
  msg_preserve_text=state;
}


void Message::setMessage(QString msg)
{
  //msg_rcv_box->clear();
  msg_rcv_box->insertPlainText(msg);
  msg_rcv_box->ensureCursorVisible();
}


void Message::appendMessage(QString msg)
{
  msg_rcv_box->insertPlainText(msg);
  msg_rcv_box->ensureCursorVisible();
}


QString Message::messageBuffer() const
{
  return msg_rcv_box->text();
}


void Message::setMessageBuffer(QString buffer)
{
  msg_rcv_box->clear();
  msg_rcv_box->setText(buffer);
}


void Message::doubleClickedData()
{
  if(!msg_hanging) {
    msg_send_box->setStyleSheet("background-color: red;color: yellow;");
    msg_rcv_box->setStyleSheet("background-color: red;color: yellow;");
    if(!msg_preserve_text) {
      msg_hang_text=msg_rcv_box->text();
      msg_rcv_box->setText("<center><font size=\"36\"><br>Look Up!</font></center>");
    }
    //    msg_backdrop_label->show();
    msg_hang_timer->start(msg_hang_time,true);
    msg_hanging=true;
    emit touched();
  }
}


void Message::paintEvent(QPaintEvent *)
{
  QPainter *p=new QPainter(this);
  p->setPen(QColor(Qt::black));
  p->fillRect(10,10,10,10,QColor(Qt::white));
  p->end();
}


int Message::numLines() const
{
  return msg_rcv_box->document()->blockCount();
}


void Message::clear()
{
  msg_rcv_box->clear();
}


void Message::setGeometry(int x,int y,int w,int h)
{
  QFontMetrics fm(msg_send_box->font());
  int send_height=fm.lineSpacing()+msg_send_lines*msg_send_box->frameWidth();
  msg_send_box->setGeometry(0,h-msg_send_lines*send_height,
			    w,msg_send_lines*send_height);
  msg_rcv_box->setGeometry(0,0,w,h-msg_send_lines*send_height);
  //  msg_backdrop_label->setGeometry(0,0,w,h);
  //  DrawBackdrop();
  QWidget::setGeometry(x,y,w,h);
}


void Message::setGeometry(const QRect &rect)
{
  setGeometry(rect.x(),rect.y(),rect.width(),rect.height());
}


void Message::sendData()
{
  emit message(msg_send_box->text());
  msg_send_box->clear();
  msg_rcv_box->textCursor().movePosition(QTextCursor::EndOfBlock,
					 QTextCursor::MoveAnchor,
					 msg_rcv_box->document()->blockCount());
}


void Message::timerData()
{
  msg_send_box->setStyleSheet("");
  msg_rcv_box->setStyleSheet("");
  if(!msg_preserve_text) {
    msg_rcv_box->setText(msg_hang_text);
  }
  //msg_backdrop_label->hide();
  msg_hanging=false;
}


void Message::nicknameChangedData(const QString &name)
{
  msg_nickname=name;
  msg_rcv_box->setNickname(name);
}


void Message::clearClickedData()
{
  emit cleared();
}

/*
void Message::DrawBackdrop()
{
  QFont font("helvetica",36,QFont::Bold);
  font.setPixelSize(36);
  QFontMetrics fm(font);
  QPixmap pix(geometry().size());
  QPainter *p=new QPainter(&pix);
  p->setPen(Qt::yellow);
  p->setFont(font);
  p->fillRect(0,0,pix.size().width(),pix.size().height(),Qt::red);
  p->drawText((pix.size().width()-fm.width(tr("Look Up!")))/2,
	      pix.size().height()/2,tr("Look Up!"));
  p->end();
  delete p;

  msg_backdrop_label->setPixmap(pix);
}
*/
