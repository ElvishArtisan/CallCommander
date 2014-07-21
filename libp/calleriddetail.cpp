// calleriddetail.cpp
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

#include "calleriddetail.h"

CallerIdDetail::CallerIdDetail(int lineno,CallDirection dir,CallState state,
				 int length,char ring_type,int ring_quan,
				 QDateTime datetime,QString num,QString name)
{
  id_line_number=lineno;
  id_direction=dir;
  id_state=state;
  id_length=length;
  id_ring_type=ring_type;
  id_ring_quantity=ring_quan;
  id_datetime=datetime;
  id_number=num;
  id_name=name;
  num.toULongLong(&id_number_is_valid);
}


int CallerIdDetail::lineNumber() const
{
  return id_line_number;
}


CallerIdDetail::CallDirection CallerIdDetail::direction() const
{
  return id_direction;
}


CallerIdDetail::CallState CallerIdDetail::state() const
{
  return id_state;
}


int CallerIdDetail::length() const
{
  return id_length;
}


char CallerIdDetail::ringType() const
{
  return id_ring_type;
}


int CallerIdDetail::ringQuantity() const
{
  return id_ring_quantity;
}


QDateTime CallerIdDetail::datetime() const
{
  return id_datetime;
}


QString CallerIdDetail::number() const
{
  return id_number;
}


void CallerIdDetail::setNumber(const QString &pnum)
{
  id_number=pnum;
  pnum.toULongLong(&id_number_is_valid);
}


QString CallerIdDetail::name() const
{
  return id_name;
}


bool CallerIdDetail::numberIsValid() const
{
  return id_number_is_valid;
}


QString CallerIdDetail::simplifiedNumber(QString pnum)
{
  pnum.replace("-","");
  pnum.replace("(","");
  pnum.replace(")","");
  pnum.replace(" ","");
  return pnum;
}


QString CallerIdDetail::formattedNumber(QString pnum)
{
  switch(pnum.length()) {
      case 7:
	pnum.insert(3,"-");
	break;

      case 10:
	pnum.insert(6,"-");
	pnum.insert(3,") ");
	pnum.insert(0,"(");
	break;
  }
  return pnum;
}
