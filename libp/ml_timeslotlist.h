// ml_timeslotlist.h
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

#ifndef ML_TIMESLOTLIST_H
#define ML_TIMESLOTLIST_H

#include <vector>

#include <ml_timeslot.h>

class MlTimeslotList
{
 public:
  MlTimeslotList(const QString &show_code);
  unsigned size() const;
  MlTimeslot *timeslot(unsigned n);
  MlTimeslot *timeslot(QDateTime &datetime);
  void load();
  bool slotsActive(const QDateTime &datetime);
  bool dowActive(int dow);
  void clear();

 private:
  QString list_showcode;
  std::vector<MlTimeslot *> list_timeslots;
};


#endif  // ML_TIMESLOTLIST_H
