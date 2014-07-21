// name_sub.h
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

#ifndef NAME_SUB_H
#define NAME_SUB_H

#include <vector>

#include <QtCore/QString>

class NameSub
{
 public:
  NameSub();
  int size();
  bool value(QString key,QString *value);
  void addValue(QString key,QString value);

 private:
  std::vector<QString> sub_keys;
  std::vector<QString> sub_values;
};


#endif  // NAME_SUB_H

