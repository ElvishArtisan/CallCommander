// mld_system.h
//
// A container class for mld(8) server systems.
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef MLD_SYSTEM_H
#define MLD_SYSTEM_H

#include <bus_driver.h>
#include <connection_data.h>

#include <ml.h>

class MldSystem
{
 public:
  MldSystem(int id);
  ~MldSystem();
  int id() const;
  BusDriver *busDriver() const;
  void setBusDriver(BusDriver *driver);
  ConnectionData *connectionData() const;
  void setConnectionData(ConnectionData *conn);
  bool connected() const;
  void setConnected(bool state);
  QString showName() const;
  void setShowName(const QString &showname);
  QString showPassword() const;
  void setShowPassword(const QString &passwd);
  QString location() const;
  void setLocation(const QString &loc);
  void clear();

 private:
  int system_id;
  BusDriver *system_bus_driver;
  ConnectionData *system_connection_data;
  bool system_connected;
  QString system_show_name;
  QString system_show_password;
  QString system_location;
};


#endif  // MLD_SYSTEM_H
