// line_button.cpp
//
// Line button for CallCommander
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

#include <QtGui/QPainter>
#include <Qt3Support/Q3PointArray>

#include "line_button.h"

LineButton::LineButton(int line,int bank,MlConfig *config,QWidget *parent)
  : QPushButton(parent)
{
  line_line=line;
  line_bank=bank;
  line_config=config;
  line_index=10;
  line_state=BusDriver::LineInactive;
  line_frame=0;
  line_font=QFont("hevetica",26,QFont::Bold);
  line_font.setPixelSize(26);
#ifndef LINUX
  setAutoFillBackground(true);
#endif  // LINUX
  InitIcons();
  UpdateCap();
}


void LineButton::setState(BusDriver::LineState state)
{
  line_state=state;
  UpdateCap();
}


void LineButton::setFrame(int frame)
{
  setPixmap(*line_icons[line_index][frame]);
  line_frame=frame;
}


void LineButton::UpdateCap()
{
  QString state="Unknown";
  MlConfig::ColorRole color;
  QPalette palette;
  QString style;

  switch(line_state) {
  case BusDriver::LineInactive:
    state="Inactive";
    line_index=10;
    color=MlConfig::IdleColor;
    break;

  case BusDriver::LineIdle:
    state="Idle";
    line_index=0;
    color=MlConfig::IdleColor;
    break;

  case BusDriver::LineElsewhere:
    state="Elsewhere";
    line_index=4;
    color=MlConfig::OffHookColor;
    break;

  case BusDriver::LineHandset:
    state="Handset";
    line_index=11;
    color=MlConfig::OffHookColor;
    break;

  case BusDriver::LineSpeaker:
    state="Speaker";
    line_index=12;
    color=MlConfig::OffHookColor;
    break;

  case BusDriver::LineHold:
    state="Hold";
    line_index=5;
    color=MlConfig::OnHoldColor;
    break;

  case BusDriver::LineScreenedHold:
    state="Screened";
    line_index=6;
    color=MlConfig::OnHoldColor;
    break;

  case BusDriver::LineOnAir:
    state="OnAir";
    line_index=1+line_bank%2;
    color=MlConfig::OnAirColor;
    break;

  case BusDriver::LineRinging:
    state="Ringing";
    line_index=8;
    color=MlConfig::RingingColor;
    break;

  case BusDriver::LineLocked:
    state="Locked";
    line_index=3;
    color=MlConfig::OnAirColor;
    break;

  case BusDriver::LineBusied:
    state="Busied";
    line_index=9;
    color=MlConfig::OffHookColor;
    break;

  case BusDriver::LineNext:
    state="Next";
    line_index=7;
    color=MlConfig::OnHoldColor;
    break;

  default:
    state="Last State";
    line_index=-1;
    color=MlConfig::IdleColor;
    break;
  }
  if(line_index<0) {
    setText(QString().sprintf("Unknown\nState %d",line_state));
  }
  else {
    setPixmap(*line_icons[line_index][line_frame]);
  }
  setPalette(line_config->palette(color));
#ifndef LINUX
  setStyleSheet(QString("background-color: ")+
		line_config->color(color).name());
#endif  // LINUX
}


void LineButton::InitIcons()
{
  QPainter *p=new QPainter();
  Q3PointArray triangle=Q3PointArray(3);
  QBitmap map(50,50);

  //
  // Line Idle/Unavailable
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(24,24,3,3,QColor(Qt::color1));
  p->end();
  MakeIcon(p,&map,0,0,line_config->color(MlConfig::IdleColor));
  line_icons[0][1]=line_icons[0][0];
  line_icons[0][2]=line_icons[0][0];

  //
  // Line On Main
  //
  p->begin(&map);
  triangle.setPoint(0,20,5);
  triangle.setPoint(1,0,25);
  triangle.setPoint(2,20,45);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(20,15,20,20,QColor(Qt::color1));
  p->drawPolygon(triangle);
  p->end();
  MakeIcon(p,&map,1,0,line_config->color(MlConfig::OnAirColor));
  line_icons[1][1]=line_icons[1][0];
  line_icons[1][2]=line_icons[1][0];

  //
  // Line On Conf
  //
  p->begin(&map);
  triangle.setPoint(0,20,5);
  triangle.setPoint(1,40,25);
  triangle.setPoint(2,20,45);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(0,15,20,20,QColor(Qt::color1));
  p->drawPolygon(triangle);
  p->end();
  MakeIcon(p,&map,2,0,line_config->color(MlConfig::OnAirColor));
  line_icons[2][1]=line_icons[2][0];
  line_icons[2][2]=line_icons[2][0];

  //
  // Line Locked
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(10,25,25,20,QColor(Qt::color1));
  p->drawArc(15,10,14,15,0,2880);
  p->drawLine(15,18,15,25);
  p->drawLine(29,18,29,25);
  p->end();
  MakeIcon(p,&map,3,0,line_config->color(MlConfig::OnAirColor));
  line_icons[3][1]=line_icons[3][0];
  line_icons[3][2]=line_icons[3][0];

  //
  // Line Off Hook
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(24,24,3,3,QColor(Qt::color1));
  p->fillRect(18,18,3,3,QColor(Qt::color1));
  p->fillRect(18,30,3,3,QColor(Qt::color1));
  p->fillRect(30,18,3,3,QColor(Qt::color1));
  p->fillRect(30,30,3,3,QColor(Qt::color1));
  p->end();
  MakeIcon(p,&map,4,0,line_config->color(MlConfig::OffHookColor));
  line_icons[4][1]=line_icons[4][0];
  line_icons[4][2]=line_icons[4][0];

  //
  // Line On Hold
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(15,15,20,20,QColor(Qt::color1));
  p->drawLine(20,10,10,10);
  p->drawLine(10,10,10,40);
  p->drawLine(10,40,20,40);
  p->drawLine(30,10,40,10);
  p->drawLine(40,10,40,40);
  p->drawLine(40,40,30,40);
  p->end();
  MakeIcon(p,&map,5,0,line_config->color(MlConfig::OnHoldColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(15,15,20,20,QColor(Qt::color1));
  p->drawLine(20,10,10,10);
  p->drawLine(10,10,10,40);
  p->drawLine(10,40,20,40);
  p->drawLine(30,10,40,10);
  p->drawLine(40,10,40,40);
  p->drawLine(40,40,30,40);
  p->end();
  MakeIcon(p,&map,5,1,line_config->color(MlConfig::OnHoldColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(15,15,20,20,QColor(Qt::color1));
  p->end();
  MakeIcon(p,&map,5,2,line_config->color(MlConfig::OnHoldColor));

  //
  // Line On Screened Hold
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawLine(10,25,20,39);
  p->drawLine(20,39,20,38);
  p->drawLine(20,38,10,24);
  p->drawLine(10,24,10,26);
  p->drawLine(10,26,20,40);
  p->drawLine(20,40,39,10);
  p->drawLine(39,10,39,9);
  p->drawLine(39,9,20,39);
  p->drawLine(20,39,20,41);
  p->drawLine(20,41,39,11);
  p->end();
  MakeIcon(p,&map,6,0,line_config->color(MlConfig::OnHoldColor));
  line_icons[6][1]=line_icons[6][0];
  line_icons[6][2]=line_icons[6][0];

  //
  // Line Ready/Next
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawLine(10,25,20,39);
  p->drawLine(20,39,20,38);
  p->drawLine(20,38,10,24);
  p->drawLine(10,24,10,26);
  p->drawLine(10,26,20,40);
  p->drawLine(20,40,39,10);
  p->drawLine(39,10,39,9);
  p->drawLine(39,9,20,39);
  p->drawLine(20,39,20,41);
  p->drawLine(20,41,39,11);
  p->end();
  MakeIcon(p,&map,7,0,line_config->color(MlConfig::OnHoldColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawLine(10,25,20,39);
  p->drawLine(20,39,20,38);
  p->drawLine(20,38,10,24);
  p->drawLine(10,24,10,26);
  p->drawLine(10,26,20,40);
  p->drawLine(20,40,39,10);
  p->drawLine(39,10,39,9);
  p->drawLine(39,9,20,39);
  p->drawLine(20,39,20,41);
  p->drawLine(20,41,39,11);
  p->end();
  MakeIcon(p,&map,7,1,line_config->color(MlConfig::OnHoldColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->end();
  MakeIcon(p,&map,7,2,line_config->color(MlConfig::OnHoldColor));

  //
  // Line Ringing
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawRect(14,14,22,22);
  p->drawRect(15,15,20,20);
  p->drawRect(16,16,18,18);
  p->end();
  MakeIcon(p,&map,8,0,line_config->color(MlConfig::RingingColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(15,15,20,20,QColor(Qt::color1));
  p->end();
  MakeIcon(p,&map,8,1,line_config->color(MlConfig::RingingColor));

  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawRect(14,14,22,22);
  p->drawRect(15,15,20,20);
  p->drawRect(16,16,18,18);
  p->drawRect(17,17,16,16);
  p->drawRect(18,18,14,14);
  p->drawRect(19,19,12,12);
  p->end();
  MakeIcon(p,&map,8,2,line_config->color(MlConfig::RingingColor));

  //
  // Line Busied Out
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->drawLine(10,10,39,39);
  p->drawLine(39,39,39,38);
  p->drawLine(39,38,10,9);
  p->drawLine(10,9,10,11);
  p->drawLine(10,11,39,40);
  p->drawLine(10,39,39,10);
  p->drawLine(39,10,39,9);
  p->drawLine(39,9,10,38);
  p->drawLine(10,38,10,40);
  p->drawLine(10,40,39,11);
  p->end();
  MakeIcon(p,&map,9,0,line_config->color(MlConfig::OffHookColor));
  line_icons[9][1]=line_icons[9][0];
  line_icons[9][2]=line_icons[9][0];

  //
  // Line Unavailable
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->end();
  MakeIcon(p,&map,10,0,line_config->color(MlConfig::IdleColor));
  line_icons[10][1]=line_icons[10][0];
  line_icons[10][2]=line_icons[10][0];

  //
  // Line on Handset
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->setBrush(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->translate(25,25);
  p->drawPie(-20,-20,40,40,0,2881);
  p->fillRect(-20,5,40,3,QColor(Qt::color1));
  p->setPen(QColor(Qt::color0));
  p->setBrush(QColor(Qt::color0));
  p->drawPie(-5,-5,10,10,0,2881);
  p->fillRect(-25,-25,50,11,QColor(Qt::color0));
  p->end();
  MakeIcon(p,&map,11,0,line_config->color(MlConfig::OffHookColor));
  line_icons[11][1]=line_icons[11][0];
  line_icons[11][2]=line_icons[11][0];

  //
  // Line on Speakerphone
  //
  p->begin(&map);
  p->setPen(QColor(Qt::color1));
  p->eraseRect(0,0,50,50);
  p->fillRect(10,15,10,15,QColor(Qt::color1));
  p->drawLine(20,15,30,5);
  p->drawLine(20,16,30,6);
  p->drawLine(20,17,30,7);
  p->drawLine(30,40,20,30);
  p->drawLine(30,39,20,29);
  p->drawLine(30,38,20,28);
  p->drawLine(30,5,30,40);
  p->drawLine(29,6,29,39);
  p->drawLine(28,7,28,38);
  p->fillRect(40,5,2,5,QColor(Qt::color1));
  p->fillRect(40,20,2,5,QColor(Qt::color1));
  p->fillRect(40,35,2,5,QColor(Qt::color1));
  p->end();
  MakeIcon(p,&map,12,0,line_config->color(MlConfig::OffHookColor));
  line_icons[12][1]=line_icons[12][0];
  line_icons[12][2]=line_icons[12][0];
}


void LineButton::MakeIcon(QPainter *p,QBitmap *map,int offset,int frame,
			  const QColor &color)
{
  line_icons[offset][frame]=new QPixmap(75,75);
  //line_icons[offset][frame]=new QPixmap(65,65);
  p->begin(line_icons[offset][frame]);
  p->setPen(QColor(Qt::color1));
  p->setFont(line_font);
  p->fillRect(0,0,75,75,color);
  //  p->fillRect(0,0,65,65,color);
  p->drawPixmap(20,-5,50,50,*map);
  p->drawText(10,65,QString().sprintf("%d",line_line+1));
  p->end();
}
