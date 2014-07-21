// ml_timeslot.cpp
//
// A Container Class for CallCommander Time Slot Entries.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtCore/QRegExp>

#include "ml_timeslot.h"

MlTimeslot::MlTimeslot(int id)
{
  clear();
  slot_id=id;
}


int MlTimeslot::id() const
{
  return slot_id;
}


QTime MlTimeslot::startTime() const
{
  return slot_start_time;
}


void MlTimeslot::setStartTime(const QTime &time)
{
  slot_start_time=time;
}


void MlTimeslot::setStartTime(const QString &time)
{
  slot_start_time=
    QTime(time.left(2).toInt(),time.mid(3,2).toInt(),time.right(2).toInt());
}


QTime MlTimeslot::endTime() const
{
  return slot_end_time;
}


void MlTimeslot::setEndTime(const QTime &time)
{
  slot_end_time=time;
}


void MlTimeslot::setEndTime(const QString &time)
{
  slot_end_time=
    QTime(time.left(2).toInt(),time.mid(3,2).toInt(),time.right(2).toInt());
}


bool MlTimeslot::dowActive(int dow) const
{
  return slot_dow_active[dow-1];
}


void MlTimeslot::setDowActive(int dow,bool state)
{
  slot_dow_active[dow-1]=state;
}


bool MlTimeslot::slotActive(const QDateTime &datetime)
{
  if(!slot_dow_active[datetime.date().dayOfWeek()-1]) {
    return false;
  }
  if((datetime.time()<slot_start_time)||(datetime.time()>slot_end_time)) {
    return false;
  }
/*
  printf("%s active, START: %s  END: %s\n",
	 (const char *)datetime.toString("yyyy-MM-dd hh:mm:ss"),
	 (const char *)slot_start_time.toString("hh:mm:ss"),
	 (const char *)slot_end_time.toString("hh:mm:ss"));
*/
  return true;
}


MlTimeslot::FilterPolicy MlTimeslot::filterPolicy() const
{
  return slot_filter_policy;
}


void MlTimeslot::setFilterPolicy(FilterPolicy policy)
{
  slot_filter_policy=policy;
}


unsigned MlTimeslot::filters() const
{
  return slot_filters.size();
}


QString MlTimeslot::filter(unsigned n) const
{
  return slot_filters[n];
}


void MlTimeslot::addFilter(const QString &str)
{
  slot_filters.push_back(str);
}


void MlTimeslot::clearFilters()
{
  slot_filters.clear();
}


bool MlTimeslot::filterMatch(const QString &str)
{
  bool ret=false;
  QRegExp *regex=new QRegExp();

  regex->setWildcard(true);
  for(unsigned i=0;i<slot_filters.size();i++) {
    regex->setPattern(slot_filters[i]);
    ret|=regex->exactMatch(str);
  }
  delete regex;

  return ret;
}


void MlTimeslot::clear()
{
  slot_start_time=QTime();
  slot_end_time=QTime(23,59,59);
  slot_filter_policy=MlTimeslot::AcceptPolicy;
  for(unsigned i=0;i<7;i++) {
    slot_dow_active[i]=true;
  }
  slot_filters.clear();
}
