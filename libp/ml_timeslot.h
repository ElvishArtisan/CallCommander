// ml_timeslot.h
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

#ifndef ML_TIMESLOT_H
#define ML_TIMESLOT_H

#include <QtCore/QDateTime>
#include <QtCore/QStringList>

class MlTimeslot
{
 public:
  enum FilterPolicy {AcceptPolicy=0,DenyPolicy=1};
  MlTimeslot(int id);
  int id() const;
  QTime startTime() const;
  void setStartTime(const QTime &time);
  void setStartTime(const QString &time);
  QTime endTime() const;
  void setEndTime(const QTime &time);
  void setEndTime(const QString &time);
  bool dowActive(int dow) const;
  void setDowActive(int dow,bool state);
  bool slotActive(const QDateTime &datetime);
  FilterPolicy filterPolicy() const;
  void setFilterPolicy(FilterPolicy policy);
  unsigned filters() const;
  QString filter(unsigned n) const;
  void addFilter(const QString &str);
  void clearFilters();
  bool filterMatch(const QString &str);
  void clear();
  
 private:
  int slot_id;
  QTime slot_start_time;
  QTime slot_end_time;
  bool slot_dow_active[7];
  FilterPolicy slot_filter_policy;
  QStringList slot_filters;
};

#endif  // ML_TIMESLOT_H
