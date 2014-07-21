// mld_connection.cpp
//
// A container class for mld(8) server connections.
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

#include <mld_connection.h>

MldConnection::MldConnection(Q3Socket *sock)
{
  clear();
  conn_socket=sock;
}


MldConnection::~MldConnection()
{
  if(conn_socket!=NULL) {
    delete conn_socket;
  }
}


Q3Socket *MldConnection::socket() const
{
  return conn_socket;
}


bool MldConnection::authenticated() const
{
  return conn_authenticated;
}


void MldConnection::setAuthenticated(bool state)
{
  conn_authenticated=state;
}


QString MldConnection::userName() const
{
  return conn_user_name;
}


void MldConnection::setUserName(const QString &username)
{
  conn_user_name=username;
}


unsigned MldConnection::systemId() const
{
  return conn_system_id;
}


void MldConnection::setSystemId(unsigned id)
{
  conn_system_id=id;
}


int MldConnection::console() const
{
  return conn_console;
}


void MldConnection::setConsole(int console)
{
  conn_console=console;
}


bool MldConnection::zombie() const
{
  return conn_zombie;
}


void MldConnection::setZombie(bool state)
{
  conn_zombie=state;
}


void MldConnection::clear()
{
  buffer[0]=0;
  ptr=0;
  conn_socket=NULL;
  conn_user_name="";
  conn_system_id=0xFFFFFFFF;
  conn_console=-1;
  conn_authenticated=false;
  conn_zombie=false;
};
