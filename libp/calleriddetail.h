// calleriddetail.h
//
// Caller ID Data Container Class
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

#ifndef CALLERIDDETAIL_H
#define CALLERIDDETAIL_H

#include <QtCore/QString>
#include <QtCore/QDateTime>

#define CALLERID_MAX_LINES 96

class CallerIdDetail
{
 public:
  enum CallDirection {CallInbound='I',CallOutbound='O'};
  enum CallState {CallStart='S',CallEnd='E'};
  enum Protocol {Native=0,CallerIdCom=1};
  CallerIdDetail(int lineno,CallDirection dir,CallState state,int length,
		  char ring_type,int ring_quan,QDateTime datetime,
		  QString num,QString name);
  int lineNumber() const;
  CallDirection direction() const;
  CallState state() const;
  int length() const;
  char ringType() const;
  int ringQuantity() const;
  QDateTime datetime() const;
  QString number() const;
  void setNumber(const QString &pnum);
  QString name() const;
  bool numberIsValid() const;
  static QString simplifiedNumber(QString pnum);
  static QString formattedNumber(QString pnum);

 private:
  int id_line_number;
  CallDirection id_direction;
  CallState id_state;
  int id_length;
  char id_ring_type;
  int id_ring_quantity;
  QDateTime id_datetime;
  QString id_number;
  QString id_name;
  bool id_number_is_valid;
};


#endif
