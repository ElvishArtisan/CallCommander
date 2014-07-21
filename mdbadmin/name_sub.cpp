// name_sub.cpp
//
// A name lookup and substition class.
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

#include "name_sub.h"

NameSub::NameSub()
{
}


int NameSub::size()
{
  return sub_keys.size();
}


bool NameSub::value(QString key,QString *value)
{
  bool ok;
  int pos;
  QString str;

  if(key.isEmpty()) {
    return false;
  }
  key.right(1).toInt(&ok);
  if(!ok) {
    return false;
  }
  if((pos=key.find("ZN"))>0) {
    str=key.left(pos);
  }
  else {
    if((pos=key.find("ZON"))>0) {
      str=key.left(pos);
    }
    else {
      str=key;
    }
  }
  for(unsigned i=0;i<sub_keys.size();i++) {
    if(sub_keys[i]==str) {
      *value=sub_values[i];
      return true;
    }
  }
  return false;
}


void NameSub::addValue(QString key,QString value)
{
  sub_keys.push_back(key);
  sub_values.push_back(value);
}
