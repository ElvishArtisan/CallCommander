// message_data.h
//
// A Container Class for CallCommander MessageBox Events.
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

#ifndef MESSAGE_DATA_H
#define MESSAGE_DATA_H

#include <QtCore/QString>

#include <bus_driver.h>

class MessageData
{
 public:
  MessageData();
  QString nick() const;
  void setNick(const QString &nick);
  QString message() const;
  void setMessage(const QString msg);
  bool update(BusDriver::Element elem,const QString &packet);
  void sendData(BusDriver *driver) const;
  void clear();
  
 private:
  QString msg_nick;
  QString msg_message;
};

#endif  // MESSAGE_DATA_H
