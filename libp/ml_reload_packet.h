// ml_reload_packet.h
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

#ifndef ML_RELOAD_PACKET_H
#define ML_RELOAD_PACKET_H

#include <QtCore/QString>
#include <Qt3Support/q3socketdevice.h>
#include <QtNetwork/QHostAddress>

#include <mldconfig.h>

QHostAddress MlResolveIpAddress(const QString &hostname);
void MlSendReloadPacket(MldConfig *config,const QHostAddress &addr,
			Q_UINT16 port,Q3SocketDevice *socket);


#endif  // ML_RELOAD_PACKET_H
