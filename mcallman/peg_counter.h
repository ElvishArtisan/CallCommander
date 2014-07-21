// peg_counter.h
//
// Generate a peg-count list.
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef PEG_COUNTER_H
#define PEG_COUNTER_H

#include <vector>

#include <QtCore/QString>

class PegCounter
{
 public:
  PegCounter();
  int size() const;
  QString itemName(int n) const;
  int itemCount(int n) const;
  void addItem(QString name);
  void clear();

 private:
  std::vector<QString> name_list;
  std::vector<int> count_list;
};


#endif  // PEG_COUNTER_H
