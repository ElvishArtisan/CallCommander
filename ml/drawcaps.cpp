// drawcaps.cpp
//
// A Qt-based application for controlling broadcast telephone interfaces.
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QPainter>
#include <Qt3Support/Q3PointArray>
#include <QtGui/QFontMetrics>

#include "globals.h"
#include "mithlond.h"

void MainWidget::DrawCaps()
{
  QPainter *p=new QPainter();
  Q3PointArray triangle=Q3PointArray(3);
  QFont font;

  //
  // Screen Button
  //
  telos_screen_icon=QPixmap(70,70);
  p->begin(&telos_screen_icon);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  p->drawLine(25,44,35,54);
  p->drawLine(35,54,55,24);
  p->drawLine(25,45,35,55);
  p->drawLine(35,55,55,25);
  p->drawLine(25,46,35,56);
  p->drawLine(35,56,55,26);
  p->drawLine(30,15,15,15);
  p->drawLine(15,15,15,65);
  p->drawLine(15,65,30,65);
  p->drawLine(50,15,65,15);
  p->drawLine(65,15,65,65);
  p->drawLine(65,65,50,65);
  p->end();

  //
  // Hold Button
  //
  telos_hold_icon=QPixmap(80,80);
  p->begin(&telos_hold_icon);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,80,80,palette().color(QPalette::Button));
  p->fillRect(25,25,30,30,QColor(Qt::color1));
  p->drawLine(30,15,15,15);
  p->drawLine(15,15,15,65);
  p->drawLine(15,65,30,65);
  p->drawLine(50,15,65,15);
  p->drawLine(65,15,65,65);
  p->drawLine(65,65,50,65);
  p->end();

  //
  // Drop Button
  //
  telos_drop_icon=QPixmap(80,80);
  p->begin(&telos_drop_icon);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,80,80,palette().color(QPalette::Button));
  p->drawRect(15,15,50,50);
  p->fillRect(38,38,4,4,QColor(Qt::color1));
  p->end();

  //
  // Busy Button
  //
  telos_busy_icon[0]=QPixmap(70,70);
  p->begin(&telos_busy_icon[0]);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  p->drawLine(15,14,54,53);
  p->drawLine(15,15,54,54);
  p->drawLine(15,16,54,55);
  p->drawLine(54,14,15,53);
  p->drawLine(54,15,15,54);
  p->drawLine(54,16,15,55);
  p->end();

  telos_busy_icon[1]=QPixmap(70,70);
  p->begin(&telos_busy_icon[1]);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,70,70,ml_config->color(MlConfig::InUseColor));
  p->drawLine(15,14,54,53);
  p->drawLine(15,15,54,54);
  p->drawLine(15,16,54,55);
  p->drawLine(54,14,15,53);
  p->drawLine(54,15,15,54);
  p->drawLine(54,16,15,55);
  p->end();

  //
  // Dump Button
  //
  telos_dump_icon=QPixmap(70,70);
  p->begin(&telos_dump_icon);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  font=QFont("Helvetica",22,QFont::Bold);
  font.setPixelSize(22);
  QFontMetrics fm(font);
  p->setFont(font);
  p->drawText((70-fm.width("?#@!"))/2,42,"?#@!");
  p->end();

  //
  // Record Button
  //
  telos_record_modulo=8;
  telos_record_icon[0]=DrawRecordCap(p,105);
  telos_record_icon[1]=DrawRecordCap(p,90);
  telos_record_icon[2]=DrawRecordCap(p,75);
  telos_record_icon[3]=DrawRecordCap(p,60);
  telos_record_icon[4]=DrawRecordCap(p,45);
  telos_record_icon[5]=DrawRecordCap(p,30);
  telos_record_icon[6]=DrawRecordCap(p,15);
  telos_record_icon[7]=DrawRecordCap(p,0);

  //
  // Next Button
  //
  telos_next_icon=QPixmap(70,70);
  p->begin(&telos_next_icon);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  triangle.setPoint(0,15,15);
  triangle.setPoint(1,35,35);
  triangle.setPoint(2,15,55);
  p->drawPolygon(triangle);
  triangle.setPoint(0,32,15);
  triangle.setPoint(1,52,35);
  triangle.setPoint(2,32,55);
  p->drawPolygon(triangle);
  p->fillRect(49,15,7,40,QColor(Qt::color1));
  p->end();

  //
  // Keypad Button
  //
  telos_keypad_icon=QPixmap(70,70);
  p->begin(&telos_keypad_icon);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  p->fillRect(15,7,12,12,QColor(Qt::color1));
  p->fillRect(29,7,12,12,QColor(Qt::color1));
  p->fillRect(43,7,12,12,QColor(Qt::color1));
  p->fillRect(15,21,12,12,QColor(Qt::color1));
  p->fillRect(29,21,12,12,QColor(Qt::color1));
  p->fillRect(43,21,12,12,QColor(Qt::color1));
  p->fillRect(15,35,12,12,QColor(Qt::color1));
  p->fillRect(29,35,12,12,QColor(Qt::color1));
  p->fillRect(43,35,12,12,QColor(Qt::color1));
  p->fillRect(29,49,12,12,QColor(Qt::color1));
  p->end();

  //
  // Start Clock
  //
  if(ml_config->animateIcons()) {
    clock=new QTimer(this,"clock");
    connect(clock,SIGNAL(timeout()),this,SLOT(TickClock()));
    clock->start(FRAME_RATE);
  }
}


void MainWidget::UpdateDelayButton(int offset)
{
  QString counter=QString().sprintf("%3.1f",(float)offset/1000.0);

  QPainter *p=new QPainter();
  telos_dump_icon=QPixmap(70,70);
  p->begin(&telos_dump_icon);
  p->setPen(QColor(Qt::color1));
  p->fillRect(0,0,70,70,ml_config->color(MlConfig::IdleColor));
  QFont font=QFont("Helvetica",22,QFont::Bold);
  font.setPixelSize(22);
  QFontMetrics fm(font);
  p->setFont(font);
  p->drawText((70-fm.width("?#@!"))/2,34,"?#@!");
  p->drawText((70-fm.width(counter))/2,62,counter);
  p->end();
  delete p;
  telos_dump_button->setPixmap(telos_dump_icon);
}


QPixmap MainWidget::DrawRecordCap(QPainter *p,int angle)
{
  QPixmap pix(70,70);
  p->begin(&pix);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->fillRect(0,0,70,70,palette().color(QPalette::Button));
  p->drawArc(2,20,30,30,0,5760);
  p->drawPie(14,32,6,6,0,5760);
  p->translate(17,35);
  p->rotate(angle);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->rotate(-angle);

  p->translate(-17,-35);
  p->drawArc(36,20,30,30,0,5760);
  p->drawPie(48,32,6,6,0,5760);
  p->translate(51,35);
  p->rotate(angle);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->fillRect(-2,-12,4,7,QColor(Qt::color1));
  p->rotate(120);
  p->translate(-56,-40);
  p->end();

  return pix;
}
