// mld_system.cpp
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

#include <mld_system.h>

MldSystem::MldSystem(int id)
{
  clear();
  system_id=id;
}


MldSystem::~MldSystem()
{
  if(system_bus_driver!=NULL) {
    delete system_bus_driver;
  }
  if(system_connection_data!=NULL) {
    delete system_connection_data;
  }
}


int MldSystem::id() const
{
  return system_id;
}


BusDriver *MldSystem::busDriver() const
{
  return system_bus_driver;
}


void MldSystem::setBusDriver(BusDriver *driver)
{
  system_bus_driver=driver;
}


ConnectionData *MldSystem::connectionData() const
{
  return system_connection_data;
}


void MldSystem::setConnectionData(ConnectionData *conn)
{
  system_connection_data=conn;
}


bool MldSystem::connected() const
{
  return system_connected;
}


void MldSystem::setConnected(bool state)
{
  system_connected=state;
}


QString MldSystem::showName() const
{
  return system_show_name;
}


void MldSystem::setShowName(const QString &showname)
{
  system_show_name=showname;
}


QString MldSystem::showPassword() const
{
  return system_show_password;
}


void MldSystem::setShowPassword(const QString &passwd)
{
  system_show_password=passwd;
}


QString MldSystem::location() const
{
  return system_location;
}


void MldSystem::setLocation(const QString &loc)
{
  system_location=loc;
}


void MldSystem::clear()
{
  system_id=-1;
  system_bus_driver=NULL;
  system_connection_data=NULL;
  system_connected=false;
  system_show_name="";
  system_location="";
}
