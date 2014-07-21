// mlsegclock.h
//
//   A Segment Clock widget.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlsegclock.h,v 1.7 2012/08/25 00:07:13 cvs Exp $
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

#ifndef MLSEGCLOCK_H
#define MLSEGCLOCK_H

#include <vector>

#include <QtGui/QWidget>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtGui/QPaintEvent>
#include <QtGui/QLabel>

#include <mlseglist.h>
#include <mlsegment.h>

//
// Fonts
//
#define MLSEGCLOCK_TITLE_FONT_FACE "Helvetica"
#define MLSEGCLOCK_TITLE_FONT_SIZE 24
#define MLSEGCLOCK_TITLE_FONT_WEIGHT QFont::Normal

#define MLSEGCLOCK_DIGIT_FONT_FACE "Timesroman"
#define MLSEGCLOCK_DIGIT_FONT_SIZE 30
#define MLSEGCLOCK_DIGIT_FONT_WEIGHT QFont::Normal

#define MLSEGCLOCK_LABEL_FONT_FACE "System"
#define MLSEGCLOCK_LABEL_FONT_SIZE 16
#define MLSEGCLOCK_LABEL_FONT_WEIGHT QFont::Bold


class MLSegClock : public QWidget
{
  Q_OBJECT
  public:
  enum Orientation {Horizontal=0,Vertical=1};
    MLSegClock(Qt::Orientation layout,QWidget *parent=0,
	      const char *name=0);
    QSizePolicy sizePolicy() const;
    QSize sizeHint() const;
    bool showHour() const;
    void setShowHour(bool state);
    MLSegList *segList() const;
    void loadSegList(struct MLSegList *list,bool relay);
    void set12Hour();
    void set24Hour();
    void setOffset(int secs);

  private slots:
    void tickClock();

  signals:
    void segStarted(int index);
    void segFinished(int index);

  protected:
    void paintEvent(QPaintEvent *);

  private:
    int clock_x,clock_y;
    int clock_size_x;
    int clock_size_y; 
    int segment_x,segment_y;
    int segment_size_x;
    int segment_size_y; 
    int label_x,label_y;
    QTime current_time,previous_time;
    MLSegList *rseglist;
    bool relay_segments;
    int current_index;
    int time_style;
    int time_offset;
    QFont title_font;
    QFont digit_font;
    QFont label_font;
    bool seg_show_hour;
    Qt::Orientation seg_style;
    QLabel *time_label;
    QLabel *segment_label;
    QLabel *comment_label;
};

#endif  // MLSEGCLOCK_H
