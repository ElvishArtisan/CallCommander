// mlplaymeter.h
//
//   A playback audio meter widget.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlplaymeter.h,v 1.3 2010/11/22 13:11:40 cvs Exp $
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

#ifndef MLPLAYMETER_H
#define MLPLAYMETER_H

#include <QtGui/QWidget>
#include <QtGui/QColor>
#include <QtGui/QPainter>
#include <QtGui/QSizePolicy>
#include <QtCore/QString>
#include <QtGui/QFont>
#include <QtGui/QPaintEvent>

#include <mlsegmeter.h>

/**
 * @short An audio level meter
 * @author Fred Gleason <fredg@wava.com>
 *
 * This class implements an audio level meter, similar to
 * that found on many pieces of audio gear.  A meter
 * label is included.
 **/
class MLPlayMeter : public QWidget
{
 Q_OBJECT
 public:

 /**
  * Instantiates the widget.
  **/
  MLPlayMeter(MLSegMeter::Orientation orient,QWidget *parent=0,
	      const char *name=0);

 /**
  * Returns a recommended size for the widget.
  **/
  QSize sizeHint() const;

  /**
   * Returns the sizing policy for the widget.
   **/
  QSizePolicy sizePolicy() const;

  /** 
   * Set the range of the audio meter.
   * @param min The minimum value which will register on the meter.
   * @param max The "full scale" value of the meter.
   **/
  void setRange(int min,int max);

  /**
   * Sets the color of the "dark" (i.e. unlit) segments in the first level
   * range of the meter.
   * @param color The color to assign to the dark segments
   **/
  void setDarkLowColor(QColor color);

  /**
   * Sets the color of the "dark" (i.e. unlit) segments in the second level
   * range of the meter.
   * @param color The color to assign to the dark segments
   **/
  void setDarkHighColor(QColor color);

  /**
   * Sets the color of the "dark" (i.e. unlit) segments in the highest level
   * range of the meter.
   * @param color The color to assign to the dark segments
   **/
  void setDarkClipColor(QColor color);

  /**
   * Sets the color of the first level range of segments in the meter.
   * @param color The color to assign to the first level range of segments
   **/
  void setLowColor(QColor color);

  /** Sets the color of the second level range of segments in the meter.
   * @param color The color to assign to the second level range of segments
   **/
  void setHighColor(QColor color);

  /**
   * Sets the color of the third level range of segments in the meter.
   * @param color The color to assign to the third level range of segments
   **/
  void setClipColor(QColor color);

  /**
   * Sets the threshold level for the transition point between the first
   * and second level range of segments
   * @param level The threshold point between the first and second level
   * range of segment
   **/
  void setHighThreshold(int level);

  /**
   * Sets the threshold level for the transition point between the second
   * and third level range of segments
   * @param level The threshold point between the second and third level
   * range of segment
   **/
  void setClipThreshold(int level);

  /**
   * Sets the width of the segments in the meters
   * @param size Width of segments in the meters
   **/
  void setSegmentSize(int size);

  /**
   * Sets the width of the gap between segments in the meters
   * @param size Width of gap between segments in the meters
   **/
  void setSegmentGap(int gap);

  /**
   * Get the current meter mode
   **/
  MLSegMeter::Mode mode() const;

  /**
   * Set the meter mode.
   * @param mode = The mode to set
   **/
  void setMode(MLSegMeter::Mode mode);

  /**
   * Set the labels text attached to the widget
   * @param label Text of label
   **/
  void setLabel(QString label);

 public slots:
  /**
   * Reimplemented for internal reasons, the API is unaffected
   **/
  void setGeometry(int x,int y,int w,int h);

  /**
   * Reimplemented for internal reasons, the API is unaffected
   **/
  void setGeometry(QRect &rect);

  /**
   * Set the level of the solid bar display
   * @param level Level to set
   **/
  void setSolidBar(int level);

  /**
   * Set the level of the floating bar display
   * @param level Level to set
   **/
  void setFloatingBar(int level);

   /**
    * Set the level of the peak display on the meter.  This is only valid
    * when the meter is in "peak" mode.
    * @param level Level of "floating" display on the meter.
    **/
  void setPeakBar(int level);

 protected:
  void paintEvent(QPaintEvent *);

 private:
  void makeFont();
  MLSegMeter *meter;
  QString meter_label;
  QFont label_font;
  MLSegMeter::Orientation orientation;
  int meter_label_x;
};


#endif  // MLPLAYMETER_H
