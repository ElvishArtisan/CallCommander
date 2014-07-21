// mld_socket.h
//
// A telephone services server for Mithlond
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MLD_SOCKET_H
#define MLD_SOCKET_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <Qt3Support/q3serversocket.h>
#include <QtNetwork/QHostAddress>

class MldSocket : public Q3ServerSocket
{
  Q_OBJECT
  public:
   MldSocket(Q_UINT16 port,int backlog=0,QObject *parent=0,
	     const char *name=0);
   MldSocket(const QHostAddress &address,Q_UINT16 port,int backlog=0,
	     QObject *parent=0,const char *name=0);
   void newConnection(int socket);

  signals:
   void connection(int);

  private:
   Q3ServerSocket *socket;
};


#endif 
