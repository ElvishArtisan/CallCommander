//   mltimenegine.h
//
//   An event timer engine.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mltimeengine.h,v 1.3 2010/11/22 13:11:40 cvs Exp $
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
//

#ifndef MLTIMENEGINE_H
#define MLTIMENEGINE_H


#include <vector>

#include <QtGui/QWidget>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>

#include <mltimeevent.h>

using namespace std;

/**
 * @short A precision event time list engine.
 * @author Fred Gleason <fredg@wava.com>
 *
 * This class implements an "engine" component into which time events can
 * be loaded.  When the time arrives, a signal will be sent.
 **/

class MLTimeEngine : public QObject
{
  Q_OBJECT
  public:
  /**
   * Instantiates the object.
   **/
    MLTimeEngine(QObject *parent=0,const char *name=0);

  /**
   * Destroys the object
   **/
    ~MLTimeEngine();

  /**
   * Clears all events.
   **/
    void clear();

  /**
   * Get an event by id.
   * @param id = The id of the event.
   * Returns: the scheduled time of the event.
   **/
    QTime event(int id) const;


   /**
    * Get the current time offset.
    * Returns: the current time offset.
    **/
    int timeOffset() const;

   /**
    * Set the time offset.
    * @param msecs = The time offset in milliseconds.
    **/
    void setTimeOffset(int msecs);

  /**
   * Add a time event.  
   * @param id = The id to be sent by the 'timeout(int)' signal for this event.
   * @param time = The time at which to emit the 'timeout(int)' signal.
   **/     
    void addEvent(int id,QTime time);

  /**
   * Remove a time event.  
   * @param id = The id of the event to be removed.
   **/     
    void removeEvent(int id);

  /**
   * Get the id of the next event.
   * Returns: the id of the next scheduled event.
   **/
    int next() const;

 signals:
  /**
   * Emitted when an event occurs.
   * @param id = The id of the event.
   **/
  void timeout(int id);

 private slots:
  void timerData();

 private:
  void EmitEvents(int offset);
  void SetTimer();
  int GetNextDiff(QTime time,int *pending_id);
  QTimer *engine_timer;
  vector<MLTimeEvent> engine_events;
  int engine_pending_id;
  int engine_time_offset;
};

#endif  // MLTIMENEGINE_H
