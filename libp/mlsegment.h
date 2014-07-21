// mlsegment.h
//
//   A segment container class for MLSegClock.
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlsegment.h,v 1.1 2011/07/27 19:40:54 cvs Exp $
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

#ifndef MLSEGMENT_H
#define MLSEGMENT_H

#include <QtCore/QDateTime>

class MLSegment
{
  public:
    MLSegment();
    MLSegment(const QTime &start,const QTime &end,const QString &label);
    QTime startTime() const;
    void setStartTime(const QTime &time);
    QTime endTime() const;
    void setEndTime(const QTime &time);
    QString label() const;
    void setLabel(const QString &str);
    QString toString() const;
    bool setFromString(const QString &str);

  private:
    QTime seg_start_time;
    QTime seg_end_time;
    QString seg_label;
};


#endif  // MLSEGMENT_H
