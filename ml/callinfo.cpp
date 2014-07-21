// callinfo.cpp
//
// Caller Information widget for CallCommander. 
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#include <calleriddetail.h>
#include "callinfo.h"

CallInfo::CallInfo(QWidget *parent)
  : QWidget(parent)
{
  QPainter *p=new QPainter();

  //
  // Cellphone Icon
  //
  call_cellphone_icon=QPixmap(25,25);
  p->begin(&call_cellphone_icon);
  p->setPen(QColor(Qt::black));
  p->setBrush(QColor(Qt::black));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-2,-2,4,4,0,5760);
  p->drawLine(0,0,0,10);
  p->setPen(QColor(Qt::red));
  p->drawArc(-5,-5,10,10,-720,1440);
  p->drawArc(-8,-8,16,16,-720,1440);
  p->drawArc(-11,-11,22,22,-720,1440);
  p->drawArc(-5,-5,10,10,2160,1440);
  p->drawArc(-8,-8,16,16,2160,1440);
  p->drawArc(-11,-11,22,22,2160,1440);
  p->end();

  //
  // Call Quality Icons
  //
  call_quality_icons[0]=QPixmap(25,25);
  p->begin(&call_quality_icons[0]);
  p->setPen(QColor(Qt::black));
  p->setBrush(QColor(Qt::black));
  p->eraseRect(0,0,25,25);
  p->end();

  call_quality_icons[1]=QPixmap(25,25);
  p->begin(&call_quality_icons[1]);
  p->setPen(QColor(Qt::red));
  p->setBrush(QColor(Qt::red));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-5,-5,10,10,0,5760);
  p->drawLine(-5,-5,-9,-9);
  p->drawLine(-5,5,-9,9);
  p->drawLine(5,5,9,9);
  p->drawLine(5,-5,9,-9);
  p->drawLine(-7,0,-12,0);
  p->drawLine(7,0,12,0);
  p->drawLine(0,-7,0,-12);
  p->drawLine(0,7,0,12);
  p->end();

  call_quality_icons[2]=QPixmap(25,25);
  p->begin(&call_quality_icons[2]);
  p->setPen(QColor(Qt::red));
  p->setBrush(QColor(Qt::red));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-5,-5,10,10,0,5760);
  p->end();

  call_quality_icons[3]=QPixmap(25,25);
  p->begin(&call_quality_icons[3]);
  p->setPen(QColor(Qt::darkYellow));
  p->setBrush(QColor(Qt::darkYellow));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-5,-5,10,10,0,5760);
  p->end();

  call_quality_icons[4]=QPixmap(25,25);
  p->begin(&call_quality_icons[4]);
  p->setPen(QColor(Qt::green));
  p->setBrush(QColor(Qt::green));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-5,-5,10,10,0,5760);
  p->end();

  call_quality_icons[5]=QPixmap(25,25);
  p->begin(&call_quality_icons[5]);
  p->setPen(QColor(Qt::green));
  p->setBrush(QColor(Qt::green));
  p->eraseRect(0,0,25,25);
  p->translate(12,12);
  p->drawPie(-5,-5,10,10,0,5760);
  p->drawLine(-5,-5,-9,-9);
  p->drawLine(-5,5,-9,9);
  p->drawLine(5,5,9,9);
  p->drawLine(5,-5,9,-9);
  p->drawLine(-7,0,-12,0);
  p->drawLine(7,0,12,0);
  p->drawLine(0,-7,0,-12);
  p->drawLine(0,7,0,12);
  p->end();
  
  delete p;

  //
  // Element Labels
  //
    for(int i=0;i<MlConfig::CallerSize;i++) {
    call_label[i]=new QLabel(this);
    call_label[i]->
      setGeometry(ml_config->callerWindow((MlConfig::CallerFields)i));
    call_label[i]->setFont(ml_config->callerFont((MlConfig::CallerFields)i));
    call_label[i]->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    call_label[i]->
      setWordWrap(ml_config->callerLines((MlConfig::CallerFields)i)>1);
    if(ml_config->callerShowFrame()) {
      call_label[i]->setFrameStyle(QFrame::Box);
    }
    if(ml_config->callerShow((MlConfig::CallerFields)i)) {
      call_label[i]->show();
    }
    else {
      call_label[i]->hide();
    }
  }
  call_label[MlConfig::CallerCell]->setScaledContents(true);
  call_label[MlConfig::CallerQuality]->setScaledContents(true);

  //
  // Comment Field
  //
  call_comment=new CallComment(this);
  call_comment->
    setGeometry(ml_config->callerWindow(MlConfig::CallerComment));
  call_comment->setFont(ml_config->callerFont(MlConfig::CallerComment));
  call_comment->setReadOnly(true);
  if(ml_config->callerShowFrame()) {
    call_comment->setFrameStyle(QFrame::Box);
  }
  if(ml_config->callerAllowScrolling()) {
    call_label[MlConfig::CallerComment]->hide();
  }
  else {
    call_label[MlConfig::CallerComment]->setWordWrap(true);
    call_comment->hide();
  }
}

/*
QSize CallInfo::sizeHint() const
{
  return QSize(0,0);
}


QSizePolicy CallInfo::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}
*/

void CallInfo::setMetadata(CallMetaData *data)
{
  //
  // Name
  //
  call_label[MlConfig::CallerName]->
    setText(data->element(BusDriver::NameElement).toString());

  //
  // Age
  //
  if(data->element(BusDriver::AgeElement).toInt()>0) {
    call_label[MlConfig::CallerAge]->
      setText(QString().sprintf("%d",data->element(BusDriver::AgeElement).
				toInt()));
  }
  else {
    call_label[MlConfig::CallerAge]->setText("");
  }

  //
  // City/State
  //
  QString citystate=data->element(BusDriver::CityElement).toString();
  if((!data->element(BusDriver::CityElement).toString().isEmpty())&&
     (!data->element(BusDriver::StateElement).toString().isEmpty())) {
    citystate+=", ";
  }
  citystate+=data->element(BusDriver::StateElement).toString();
  call_label[MlConfig::CallerCityState]->setText(citystate);

  //
  // Zip Code
  //
  call_label[MlConfig::CallerZip]->
    setText(data->element(BusDriver::ZipElement).toString());

  //
  // Phone Number
  //
  call_label[MlConfig::CallerPhone]->
    setText(CallerIdDetail::formattedNumber(
	     data->element(BusDriver::PhoneElement).toString()));

  //
  // Station
  //
  call_label[MlConfig::CallerStation]->
    setText(data->element(BusDriver::StationElement).toString());

  //
  // Gender
  //
  call_label[MlConfig::CallerGender]->
    setText(data->element(BusDriver::GenderElement).toString());

  //
  // Comment
  //
  call_label[MlConfig::CallerComment]->
    setText(data->element(BusDriver::CommentElement).toString());
  call_comment->setText(data->element(BusDriver::CommentElement).toString());

  //
  // Cellphone
  //
  if(data->element(BusDriver::CellphoneElement).toBool()) {
    call_label[MlConfig::CallerCell]->setPixmap(call_cellphone_icon);
  }
  else {
    call_label[MlConfig::CallerCell]->setPixmap(QPixmap());
  }

  //
  // Call Quality
  //
  switch(data->element(BusDriver::QualityElement).toInt()) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
    call_label[MlConfig::CallerQuality]->
      setPixmap(call_quality_icons[data->element(BusDriver::QualityElement).
				   toInt()]);
    break;

  default:
    call_label[MlConfig::CallerQuality]->setText("");
    break;
  }

  //
  // Counter
  //
  call_label[MlConfig::CallerCallCounter]->
    setText(QString().sprintf("%d",data->element(BusDriver::CounterElement).
			      toInt()));
}


void CallInfo::setTimer(int value)
{
  if(value<0) {
    call_label[MlConfig::CallerTimer]->setText("");
  }
  else {
    int min=value/60;
    int sec=value-min*60;
    
    call_label[MlConfig::CallerTimer]->
      setText(QString().sprintf("%02d:%02d",min,sec));
  }
}


void CallInfo::clear()
{
  for(int i=0;i<MlConfig::CallerSize;i++) {
    call_label[i]->setText("");
  }
  call_comment->clear();
  call_label[MlConfig::CallerCell]->setPixmap(QPixmap());
  call_label[MlConfig::CallerQuality]->setPixmap(QPixmap());
}
