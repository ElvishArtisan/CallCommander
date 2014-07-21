// call_meta_data.h
//
// A Container Class for CallCommander Call Metadata.
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

#ifndef CALL_META_DATA_H
#define CALL_META_DATA_H

#include <QtCore/QVariant>

#include <bus_driver.h>

class CallMetaData
{
 public:
  CallMetaData();
  int version(BusDriver::Element elem) const;
  QVariant element(BusDriver::Element elem) const;
  static int priorityOffset(const QString &packet);
  void setElement(BusDriver::Element elem,int value);
  void setElement(BusDriver::Element elem,unsigned value);
  void setElement(BusDriver::Element elem,bool value);
  void setElement(BusDriver::Element elem,const QString &value);
  void setElement(BusDriver::Element elem,const char *value);
  bool update(BusDriver::Element elem,const QString &packet);
  void sendData(BusDriver *driver,unsigned line,int priority_offset=0) const;
  void clear(bool reset_version=false);
  
 private:
  bool ElementValid(BusDriver::Element elem) const;
  unsigned ElementOffset(BusDriver::Element elem) const;
  int meta_version[BusDriver::FirstNameAwardElement-BusDriver::NameElement];
  QVariant meta_element[BusDriver::FirstNameAwardElement-BusDriver::NameElement];
  /*
  int meta_version[BusDriver::LastElement-BusDriver::NameElement];
  QVariant meta_element[BusDriver::LastElement-BusDriver::NameElement];
  */
};

#endif  // CALL_META_DATA_H
