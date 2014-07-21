// ml_reload_packet.cpp
//
// Reload Packet Functions for CallCommander
//
//   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifdef WIN32
#include <winsock2.h>
#else
#include <netdb.h>
#endif  // WIN32

#include <QtGui/QMessageBox>

#include "ml_reload_packet.h"

QHostAddress MlResolveIpAddress(const QString &hostname)
{
  struct hostent *hostent;
  QHostAddress addr;

  if((hostent=gethostbyname(hostname))==NULL) {
    return addr;
  }
  addr.setAddress(QString().sprintf("%d.%d.%d.%d",0xFF&hostent->h_addr[0],
				    0xFF&hostent->h_addr[1],
				    0xFF&hostent->h_addr[2],
				    0xFF&hostent->h_addr[3]));
  return addr;
}


void MlSendReloadPacket(MldConfig *config,const QHostAddress &addr,
			Q_UINT16 port,Q3SocketDevice *socket)
{
  QString packet=QString().sprintf("%s\x0A%s\x0A",
				   (const char *)config->mysqlUsername(),
				   (const char *)config->mysqlPassword());
  socket->writeBlock(packet,packet.length(),addr,port);
}
