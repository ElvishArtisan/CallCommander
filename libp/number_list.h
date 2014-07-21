// number_list.h
//
// A Container Class for CallCommander Redialler Number Entries.
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

#ifndef NUMBER_LIST_H
#define NUMBER_LIST_H

#include <vector>

#include <QtCore/QString>

class NumberList
{
 public:
  NumberList();
  unsigned size() const;
  void setMaxSize(int size);
  QString number(int n) const;
  void pushNumber(const QString &number);

 private:
  void LoadNumbers();
  void SaveNumbers() const;
  void TrimList();
  std::vector<QString> list_numbers;
  QString list_filename;
  unsigned list_max_size;
};

#endif  // NUMBER_LIST_H
