// mldshow.h
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

#ifndef MLDSHOW_H
#define MLDSHOW_H

#include <QtCore/QDateTime>

class MldShow
{
 public:
  MldShow();
  QString showCode() const;
  void setShowCode(QString showcode);
  int startDay() const;
  void setStartDay(int day);
  int endDay() const;
  void setEndDay(int time);
  QTime startTime() const;
  void setStartTime(QTime time);
  QTime endTime() const;
  void setEndTime(QTime time);
  bool isActive(QTime time,int dow);
  int id() const;
  void setId(int id);
  int killDeviceConsole() const;
  void setKillDeviceConsole(int console);
  void clear();

 private:
  QString show_showcode;
  int show_start_day;
  int show_end_day;
  QTime show_start_time;
  QTime show_end_time;
  int show_id;
  int show_kill_device_console;
};


#endif  // MLDSHOW_H
