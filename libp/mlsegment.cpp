// mlsegment.cpp
//
//   A segment container class for MLSegClock.
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlsegment.cpp,v 1.1 2011/07/27 19:40:54 cvs Exp $
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

#include <QtCore/QStringList>

#include "mlsegment.h"

MLSegment::MLSegment()
{
}


MLSegment::MLSegment(const QTime &start,const QTime &end,const QString &label)
{
  seg_start_time=start;
  seg_end_time=end;
  seg_label=label;
}


QTime MLSegment::startTime() const
{
  return seg_start_time;
}


void MLSegment::setStartTime(const QTime &time)
{
  seg_start_time=time;
}


QTime MLSegment::endTime() const
{
  return seg_end_time;
}


void MLSegment::setEndTime(const QTime &time)
{
  seg_end_time=time;
}


QString MLSegment::label() const
{
  return seg_label;
}


void MLSegment::setLabel(const QString &str)
{
  seg_label=str;
}


QString MLSegment::toString() const
{
  return seg_start_time.toString("hh:mm:ss")+","+
    seg_end_time.toString("hh:mm:ss")+","+
    seg_label;
}


bool MLSegment::setFromString(const QString &str)
{
  QStringList fields=str.split(",");
  if(fields.size()>=3) {
    seg_start_time=QTime::fromString(fields[0],"hh:mm:ss");
    seg_end_time=QTime::fromString(fields[1],"hh:mm:ss");
    if(seg_start_time.isValid()&&seg_end_time.isValid()&&
       (seg_start_time<seg_end_time)) {
      seg_label=fields[2];
      for(int i=3;i<fields.size();i++) {
	seg_label+=(","+fields[i]);
      }
      return true;
    }
  }
  return false;
}
