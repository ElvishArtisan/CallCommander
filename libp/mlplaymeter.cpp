// mlplaymeter.cpp
//
// This implements a widget that represents a stereo audio level meter,
// complete with labels and scale.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlplaymeter.cpp,v 1.4 2011/07/20 17:12:52 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QFontMetrics>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QPushButton>
#include <QtGui/QPaintEvent>

#include <stdio.h>
#include <QtGui/QSlider>
#include <Qt3Support/q3buttongroup.h>
#include <QtGui/QSizePolicy>
#include <QtGui/QMessageBox>

#include "mlplaymeter.h"

MLPlayMeter::MLPlayMeter(MLSegMeter::Orientation orient,QWidget *parent,const char *name)
  : QWidget(parent,name)
{
  meter_label=QString("");
  setBackgroundColor(Qt::black);
  orientation=orient;
  makeFont();
  meter=new MLSegMeter(orientation,this,"meter");
  meter->setSegmentSize(5);
  meter->setSegmentGap(1);
}


QSize MLPlayMeter::sizeHint() const
{
  if(meter_label==QString("")) {
    return QSize(335,60);
  }
  else {
    return QSize(335,80);
  }
}


QSizePolicy MLPlayMeter::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}


void MLPlayMeter::setRange(int min,int max)
{
  meter->setRange(min,max);
}


void MLPlayMeter::setDarkLowColor(QColor color)
{
  meter->setDarkLowColor(color);
}


void MLPlayMeter::setDarkHighColor(QColor color)
{
  meter->setDarkHighColor(color);
}


void MLPlayMeter::setDarkClipColor(QColor color)
{
  meter->setDarkClipColor(color);
}


void MLPlayMeter::setLowColor(QColor color)
{
  meter->setLowColor(color);
}


void MLPlayMeter::setHighColor(QColor color)
{
  meter->setHighColor(color);
}


void MLPlayMeter::setClipColor(QColor color)
{
  meter->setClipColor(color);
}


void MLPlayMeter::setHighThreshold(int level)
{
  meter->setHighThreshold(level);
}


void MLPlayMeter::setClipThreshold(int level)
{
  meter->setClipThreshold(level);
}


void MLPlayMeter::setLabel(QString label)
{
  meter_label=label;
  makeFont();
  setGeometry(geometry().left(),geometry().top(),
	      geometry().width(),geometry().height());
}


void MLPlayMeter::setGeometry(int x,int y,int w,int h)
{
  QWidget::setGeometry(x,y,w,h);
  if(meter_label.isEmpty()) {
    meter->setGeometry(2,2,w-4,h-4);
  }
  else {
    switch(orientation) {
	case MLSegMeter::Left:
	  meter->setGeometry(2,2,w-4-h,h-4);
	  label_font=QFont("helvetica",height()-2,QFont::Bold);
	  label_font.setPixelSize(height()-2);
	  break;
	case MLSegMeter::Right:
	  meter->setGeometry(2+h,2,w-4-h,h-4);
	  label_font=QFont("helvetica",height()-2,QFont::Bold);
	  label_font.setPixelSize(height()-2);
	  break;
	case MLSegMeter::Up:
	  meter->setGeometry(2,2,w-4,h-4-w);
	  label_font=QFont("helvetica",width()-2,QFont::Bold);
	  label_font.setPixelSize(width()-2);
	  break;
	case MLSegMeter::Down:
	  meter->setGeometry(2,2+width(),w-4,h-4-w);
	  label_font=QFont("helvetica",width()-2,QFont::Bold);
	  label_font.setPixelSize(width()-2);
	  break;
    }
    makeFont();
  }
}


void MLPlayMeter::setGeometry(QRect &rect)
{
  setGeometry(rect.left(),rect.top(),rect.width(),rect.height());
}


void MLPlayMeter::setSolidBar(int level)
{
  meter->setSolidBar(level);
}


void MLPlayMeter::setPeakBar(int level)
{
  meter->setPeakBar(level);
}


void MLPlayMeter::setFloatingBar(int level)
{
  meter->setFloatingBar(level);
}


void MLPlayMeter::setSegmentSize(int size)
{
  meter->setSegmentSize(size);
}


void MLPlayMeter::setSegmentGap(int gap)
{
  meter->setSegmentGap(gap);
}


MLSegMeter::Mode MLPlayMeter::mode() const
{
  return meter->mode();
}


void MLPlayMeter::setMode(MLSegMeter::Mode mode)
{
  meter->setMode(mode);
}


void MLPlayMeter::paintEvent(QPaintEvent *paintEvent)
{
  //
  // Setup
  //
  QPainter *p=new QPainter(this);
  p->setFont(label_font);
  p->setPen(Qt::white);
  if(!meter_label.isEmpty()) {
    switch(orientation) {
	case MLSegMeter::Left:
	  p->drawText(width()-height()+meter_label_x,height()-2,meter_label);
	  break;
	case MLSegMeter::Right:
	  p->drawText(meter_label_x,height()-2,meter_label);
	  break;
	case MLSegMeter::Up:
	  p->drawText(meter_label_x,height()-3,meter_label);
	  break;
	case MLSegMeter::Down:
	  p->drawText(meter_label_x,width()-1,meter_label);
	  break;
    }
  }
  p->end();
}



void MLPlayMeter::makeFont()
{
  switch(orientation) {
      case MLSegMeter::Left:
	label_font=QFont("helvetica",height()-2,QFont::Bold);
	label_font.setPixelSize(height()-2);
	meter_label_x=(height()-QFontMetrics(label_font).
		       width(meter_label))/2;
	break;
      case MLSegMeter::Right:
	label_font=QFont("helvetica",height()-2,QFont::Bold);
	label_font.setPixelSize(height()-2);
	meter_label_x=(height()-QFontMetrics(label_font).
		       width(meter_label))/2;
	break;
      case MLSegMeter::Up:
	label_font=QFont("helvetica",width()-2,QFont::Bold);
	label_font.setPixelSize(width()-2);
	meter_label_x=(width()-QFontMetrics(label_font).
		       width(meter_label))/2;
	break;
      case MLSegMeter::Down:
	label_font=QFont("helvetica",width()-2,QFont::Bold);
	label_font.setPixelSize(width()-2);
	meter_label_x=(width()-QFontMetrics(label_font).
		       width(meter_label))/2;
	break;
  }
}
