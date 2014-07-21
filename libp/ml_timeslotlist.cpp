// ml_timeslotlist.cpp
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

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "mlconf.h"
#include "ml_timeslotlist.h"


MlTimeslotList::MlTimeslotList(const QString &show_code)
{
  list_showcode=show_code;
}


unsigned MlTimeslotList::size() const
{
  return list_timeslots.size();
}


MlTimeslot *MlTimeslotList::timeslot(unsigned n)
{
  return list_timeslots[n];
}


MlTimeslot *MlTimeslotList::timeslot(QDateTime &datetime)
{
  for(unsigned i=0;i<list_timeslots.size();i++) {
    if(list_timeslots[i]->slotActive(datetime)) {
      return list_timeslots[i];
    }
  }
  return NULL;
}


void MlTimeslotList::load()
{
  QString sql;
  QSqlQuery *q;
  QSqlQuery *q1;

  sql=QString().sprintf("select START_TIME,END_TIME,\
                         MON,TUE,WED,THU,FRI,SAT,SUN,FILTER_POLICY,ID \
                         from SHOW_SLOTS where SHOW_CODE=\"%s\"",
			(const char *)list_showcode);
  q=new QSqlQuery(sql);
  while(q->next()) {
    list_timeslots.push_back(new MlTimeslot(q->value(10).toInt()));
    list_timeslots.back()->setStartTime(q->value(0).toTime());
    list_timeslots.back()->setEndTime(q->value(1).toTime());
    for(int i=0;i<7;i++) {
      list_timeslots.back()->setDowActive(i+1,MLBool(q->value(i+2).toString()));
    }
    list_timeslots.back()->
      setFilterPolicy((MlTimeslot::FilterPolicy)q->value(9).toInt());
    sql=QString().sprintf("select FILTER from CID_FILTERS where SLOT_ID=%d",
			  q->value(10).toInt());
    q1=new QSqlQuery(sql);
    while(q1->next()) {
      list_timeslots.back()->addFilter(q1->value(0).toString());
    }
    delete q1;
  }
  delete q;
}


bool MlTimeslotList::slotsActive(const QDateTime &datetime)
{
  for(unsigned i=0;i<list_timeslots.size();i++) {
    if(list_timeslots[i]->slotActive(datetime)) {
      return true;
    }
  }
  return false;
}


bool MlTimeslotList::dowActive(int dow)
{
  for(unsigned i=0;i<list_timeslots.size();i++) {
    if(list_timeslots[i]->dowActive(dow)) {
      return true;
    }
  }
  return false;
}


void MlTimeslotList::clear()
{
  for(unsigned i=0;i<list_timeslots.size();i++) {
    delete list_timeslots[i];
  }
  list_timeslots.clear();
}
