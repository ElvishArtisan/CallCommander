// calleridtrap.h
//
// Serial Driver for CallerID.com devices.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CALLERIDTRAP_H
#define CALLERIDTRAP_H

#define CALLERIDTRAP_SCAN_INTERVAL 100

#include <QtCore/QObject>
#include <QtCore/QTimer>

#include <mlttydevice.h>
#include <calleriddetail.h>

class CallerIdTrap : public QObject
{
 Q_OBJECT;
 public:
  CallerIdTrap(int id,QObject *parent=0,const char *name=0);
  ~CallerIdTrap();
  int id() const;
  void setDeviceName(QString name);
  bool simplifyNumber() const;
  void setSimplifyNumber(bool state);
  int lineMap(int line) const;
  void setLineMap(int line,int dest_line);
  bool open();
  void close();

 signals:
  void idReceived(int id,CallerIdDetail *detail);

 private slots:
  void scanTtyData();

 private:
  void ParseBuffer(char *buf);
  int id_id;
  MLTTYDevice *id_device;
  QTimer *id_timer;
  bool id_simplify_number;
  char id_buffer[256];
  int id_ptr;
  int id_line_map[CALLERID_MAX_LINES];
};


#endif  // CALLERIDTRAP_H
