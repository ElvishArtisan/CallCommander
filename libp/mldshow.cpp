// mldshow.cpp
//
// A container class for CallCommander mld(8) Configuration
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include "ml.h"
#include "mldshow.h"

MldShow::MldShow()
{
  clear();
}


QString MldShow::showCode() const
{
  return show_showcode;
}

void MldShow::setShowCode(QString showcode)
{
  show_showcode=showcode;
}


int MldShow::startDay() const
{
  return show_start_day;
}


void MldShow::setStartDay(int day)
{
  show_start_day=day;
}


int MldShow::endDay() const
{
  return show_end_day;
}


void MldShow::setEndDay(int day)
{
  show_end_day=day;
}


QTime MldShow::startTime() const
{
  return show_start_time;
}


void MldShow::setStartTime(QTime time)
{
  show_start_time=time;
}


QTime MldShow::endTime() const
{
  return show_end_time;
}


void MldShow::setEndTime(QTime time)
{
  show_end_time=time;
}


bool MldShow::isActive(QTime time,int dow)
{
  if((time>=show_start_time)&&(time<show_end_time)) {
    if(show_start_day<=show_end_day) {
      if((dow>=show_start_day)&&(dow<=show_end_day)) {
	return true;
      }
      return false;
    }
    if((dow<=show_start_day)&&(dow>show_end_day)) {
      return true;
    }
    return false;
  }
  return false;
}


int MldShow::id() const
{
  return show_id;
}


void MldShow::setId(int id)
{
  show_id=id;
}


int MldShow::killDeviceConsole() const
{
  return show_kill_device_console;
}


void MldShow::setKillDeviceConsole(int console)
{
  show_kill_device_console=console;
}


void MldShow::clear()
{
  show_showcode="";
  show_start_day=0;
  show_end_day=0;
  show_start_time=QTime();
  show_end_time=QTime();
  show_kill_device_console=ML_DEFAULT_KILL_DEVICE_CONSOLE;
}
