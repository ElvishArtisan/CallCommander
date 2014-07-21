// mlsegclock.cpp
//
//   A segment clock widget.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlsegclock.cpp,v 1.19 2012/08/25 00:07:13 cvs Exp $
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

#include <QtGui/QPainter>
#include <QtGui/QPixmap>
#include <QtCore/QDateTime>
#include <QtGui/QFontMetrics>
#include <QtGui/QPaintEvent>

#include <stdio.h>
#include <string.h>

#include "mlsegclock.h"

MLSegClock::MLSegClock(Qt::Orientation style,QWidget *parent,
		     const char *name)
  :QWidget(parent,name)
{
  seg_style=style;
  relay_segments=false;

  //
  // Create Fonts
  //
  title_font=QFont(MLSEGCLOCK_TITLE_FONT_FACE,
		   MLSEGCLOCK_TITLE_FONT_SIZE,
		   MLSEGCLOCK_TITLE_FONT_WEIGHT);
  title_font.setPixelSize(MLSEGCLOCK_TITLE_FONT_SIZE);

  digit_font=QFont(MLSEGCLOCK_DIGIT_FONT_FACE,
		   MLSEGCLOCK_DIGIT_FONT_SIZE,
		   MLSEGCLOCK_DIGIT_FONT_WEIGHT);
  digit_font.setPixelSize(MLSEGCLOCK_DIGIT_FONT_SIZE);
  QFontMetrics *fm=new QFontMetrics(digit_font);
  clock_size_x=fm->boundingRect("00:00:00").width();
  clock_size_y=fm->boundingRect("00:00:00").height();
  segment_size_x=fm->boundingRect("00:00").width();
  segment_size_y=fm->boundingRect("00:00").height();  
  delete fm;
  seg_show_hour=true;

  label_font=QFont(MLSEGCLOCK_LABEL_FONT_FACE,
		   MLSEGCLOCK_LABEL_FONT_SIZE,
		   MLSEGCLOCK_LABEL_FONT_WEIGHT);
  label_font.setPixelSize(MLSEGCLOCK_LABEL_FONT_SIZE);

  //
  // Setup Style Layout
  //
  switch(style) {
  case Qt::Vertical:
    clock_x=0;
    clock_y=0;
    segment_x=0;
    segment_y=76;
    label_x=4;
    label_y=166;
    break;
  case Qt::Horizontal:
    clock_x=0;
    clock_y=0;
    segment_x=176;
    segment_y=0;
    label_x=4;
    label_y=74;
    break;
 default:
    clock_x=0;
    clock_y=0;
    segment_x=0;
    segment_y=76;
    label_x=4;
    label_y=166;
    break;
  }

  //
  // Clock Stuff
  //
  time_offset=0;
  previous_time=QTime::currentTime().addSecs(time_offset);
  QTimer *timer=new QTimer(this,"timer");
  connect(timer,SIGNAL(timeout()),this,SLOT(tickClock()));
  timer->start(100);
  rseglist=0;
  current_index=-1;
  time_style=1;
  time_offset=0;

  //
  // Time Field
  //
  time_label=new QLabel(this);
  //time_label->setAlignment(Qt::AlignCenter);
  time_label->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
  time_label->setFont(digit_font);
  time_label->
    setGeometry(clock_x+1,clock_y+35,168,MLSEGCLOCK_DIGIT_FONT_SIZE);

  //
  // Segment Field
  //
  segment_label=new QLabel(this);
  segment_label->setAlignment(Qt::AlignCenter);
  segment_label->setFont(digit_font);
  segment_label->
    setGeometry(segment_x+1,segment_y+35,168,MLSEGCLOCK_DIGIT_FONT_SIZE);

  //
  // Label Field
  //
  comment_label=new QLabel(this);
  comment_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  comment_label->setFont(label_font);
  comment_label->
    setGeometry(label_x+1,label_y-15,168,MLSEGCLOCK_LABEL_FONT_SIZE);
  comment_label->setBackgroundColor(Qt::red);

  QPalette p=palette();
  p.setColor(QPalette::WindowText,Qt::green);
  comment_label->setPalette(p);

  setPalette(QPalette(QColor(0,0,0)));
}


QSizePolicy MLSegClock::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}


QSize MLSegClock::sizeHint() const
{
  switch(seg_style) {
      case Qt::Horizontal:
	return QSize(344,85);
	break;

      case Qt::Vertical:
	return QSize(172,170);
	break;
  }
  return QSize(172,170);
}


bool MLSegClock::showHour() const
{
  return seg_show_hour;
}


void MLSegClock::setShowHour(bool state)
{
  seg_show_hour=state;
}


MLSegList *MLSegClock::segList() const
{
  return rseglist;
}


void MLSegClock::loadSegList(MLSegList *list,bool relay)
{
  rseglist=list;
  relay_segments=relay;
}


void MLSegClock::tickClock()
{
  QString accum;
  QString alabel;
  QString str;

  current_time=QTime::currentTime().addSecs(time_offset);
  if(current_time.second()!=previous_time.second()) {
    previous_time=current_time;
    //
    // Clock Display
    //
    if(time_style==1) {
      if(seg_show_hour) {
	str=current_time.toString("h:mm:ss");
      }
      else {
	str=current_time.toString("--:mm:ss");
      }
    }
    else {
      if(seg_show_hour) {
	str=current_time.toString("h:mm:ss ap");
      }
      else {
	str=current_time.toString("--:mm:ss ap");
      }
      str=str.left(str.length()-3);
    }
    time_label->setText(str);

    //
    // Segment and Label Display
    //
    if(rseglist!=NULL) {
      //int index=rseglist->index(current_time.addSecs(-time_offset));
      int index=rseglist->index(current_time);
      if(index!=current_index) {
	if((current_index!=-1)&&relay_segments) {
	  emit segFinished(current_index);
	}
	if((index!=-1)&&relay_segments) {
	  emit segStarted(index);
	}
	current_index=index;
      }
      if(index>=0) {
	int secs=rseglist->secondsRemaining(index,current_time);
	accum=QString().sprintf("%02d:%02d",secs/60,secs%60);
	alabel=rseglist->segment(index)->label();
      }
      else {
	accum="00:00";
	alabel="";
      }
    }
    else {
      accum="00:00";
      alabel="";
    }
    segment_label->setText(accum);
    comment_label->setText(alabel);
  }
}


void MLSegClock::set12Hour()
{
  time_style=0;
}


void MLSegClock::set24Hour()
{
  time_style=1;
}


void MLSegClock::setOffset(int secs)
{
  time_offset=secs;
}


void MLSegClock::paintEvent(QPaintEvent *e)
{
  QPainter p(this);
  p.setPen(Qt::red);
  p.setBrush(Qt::red);

  //
  // Clock Counter
  //
  p.drawLine(clock_x,clock_y,clock_x+170,clock_y);
  p.drawLine(clock_x+170,clock_y,clock_x+170,clock_y+70);
  p.drawLine(clock_x+170,clock_y+70,clock_x,clock_y+70);
  p.drawLine(clock_x,clock_y+70,clock_x,clock_y);
  p.drawLine(clock_x,clock_y,clock_x,clock_y+30);
  p.drawLine(clock_x,clock_y+30,clock_x+170,clock_y+30);
  p.drawLine(clock_x,clock_y+12,clock_x+50,clock_y+12);
  p.drawLine(clock_x+170-50,clock_y+12,clock_x+170,clock_y+12);
  p.drawLine(clock_x,clock_y+18,clock_x+50,clock_y+18);
  p.drawLine(clock_x+170-50,clock_y+18,clock_x+170,clock_y+18);
  p.setFont(title_font);
  p.drawText(clock_x+55,clock_y+25,tr("TIME"),4);

  //
  // Segment Counter
  //
  p.drawLine(segment_x,segment_y,segment_x+170,segment_y);
  p.drawLine(segment_x+170,segment_y,segment_x+170,segment_y+70);
  p.drawLine(segment_x+170,segment_y+70,segment_x,segment_y+70);
  p.drawLine(segment_x,segment_y+70,segment_x,segment_y);
  p.drawLine(segment_x,segment_y+30,segment_x+170,segment_y+30);
  p.drawLine(segment_x,segment_y+12,segment_x+20,segment_y+12);
  p.drawLine(segment_x+170-20,segment_y+12,segment_x+170,segment_y+12);
  p.drawLine(segment_x,segment_y+18,segment_x+20,segment_y+18);
  p.drawLine(segment_x+170-20,segment_y+18,segment_x+170,segment_y+18);
  p.setFont(title_font);
  p.drawText(segment_x+25,segment_y+25,tr("SEGMENT"),7);
}
