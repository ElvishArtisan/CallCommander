// mld_connection.h
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


#ifndef MLD_CONNECTION_H
#define MLD_CONNECTION_H

#include <Qt3Support/q3socket.h>

#include <ml.h>

class MldConnection
{
 public:
  MldConnection(Q3Socket *);
  ~MldConnection();
  Q3Socket *socket() const;
  char buffer[ML_VIRTUAL_MAX_MSG_LENGTH];
  unsigned ptr;
  bool authenticated() const;
  void setAuthenticated(bool state);
  QString userName() const;
  void setUserName(const QString &username);
  unsigned systemId() const;
  void setSystemId(unsigned id);
  int console() const;
  void setConsole(int console);
  bool zombie() const;
  void setZombie(bool state);
  void clear();

 private:
  Q3Socket *conn_socket;
  bool conn_authenticated;
  QString conn_user_name;
  unsigned conn_system_id;
  int conn_console;
  bool conn_zombie;
};


#endif  // MLD_CONNECTION_H
