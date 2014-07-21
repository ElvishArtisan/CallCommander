// vguest.h
//
// Communications Layer for the Logitek vGuest Protocol
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#ifndef VGUEST_H
#define VGUEST_H

#include <Qt3Support/q3socketdevice.h>
#include <QtCore/QTimer>

#define VGUEST_RECONNECT_MIN_INTERVAL 5000
#define VGUEST_RECONNECT_MAX_INTERVAL 15000.0
#define VGUEST_MAX_COMMAND_LENGTH 255
#define VGUEST_ID_BYTE 0x0C;
#define VGUEST_DEFAULT_SURFACE_NUMBER 1
#define VGUEST_DEFAULT_PORT 10212
#define VGUEST_MAX_TEXT_LENGTH 60

class VGuest
{
 public:
  VGuest(int id);
  int id() const;
  bool connect(const QHostAddress &addr,Q_UINT16 port,const QString &user,
		 const QString &password);
  void close();
  void sendPulse(int engine,int device,int surface,int relay,int length);
  void sendTurnOn(int engine,int device,int surface,int bus);
  void sendTurnOff(int engine,int device,int surface,int bus);
  void sendSetInputAssign(int dest_engine,int dest_device,int surface,
		       int src_engine,int src_device);
  void sendSetInputMode(int engine,int device,int surface,int mode);
  void sendSetFaderLevel(int engine,int device,int surface,int level);
  void sendWriteText(int engine,int device,int surface,int dline,int dcol,
		     int attr,const QString &msg);
  void readyRead();

 private:
  void SendCommand(char *str,int len);
  void DispatchCommand(char *cmd,int len);
  int GetHoldoff();
  QString RenderCommand(char *cmd,int len);
  QString PadString(QString str,unsigned len);
  Q3SocketDevice *vguest_socket;
  int vguest_istate;
  int vguest_cmd_length;
  int vguest_cmd_ptr;
  char vguest_cmd_buffer[VGUEST_MAX_COMMAND_LENGTH];
  int vguest_id;
  bool vguest_connecting;
  bool vguest_connected;
  QHostAddress vguest_address;
  Q_UINT16 vguest_port;
  QString vguest_profile;
  QString vguest_password;
};


#endif  // VGUEST_H
