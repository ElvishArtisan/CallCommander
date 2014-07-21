//   mltimeevent.cpp
//
//   A Container Class for MLTimeEngine events.
//
//   (C) Copyright 2003-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mltimeevent.cpp,v 1.3 2011/07/20 17:12:52 cvs Exp $
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
///

#include "mltimeevent.h"

MLTimeEvent::MLTimeEvent()
{
  clear();
}


QTime MLTimeEvent::time() const
{
  return event_time;
}


void MLTimeEvent::setTime(QTime time)
{
  event_time=time;
}


int MLTimeEvent::id(int num) const
{
  return event_id[num];
}


void MLTimeEvent::setId(int num,int id)
{
  event_id[num]=id;
}


void MLTimeEvent::addId(int id)
{
  event_id.push_back(id);
}


void MLTimeEvent::removeId(int num)
{
  std::vector<int>::iterator it=event_id.begin()+num;
  event_id.erase(it,it+1);
}


int MLTimeEvent::size() const
{
  return event_id.size();
}


void MLTimeEvent::clear()
{
  event_time=QTime();
  event_id.clear();
}
