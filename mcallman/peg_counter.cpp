// peg_counter.cpp
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

#include "peg_counter.h"

PegCounter::PegCounter()
{
}


int PegCounter::size() const
{
  return name_list.size();
}


QString PegCounter::itemName(int n) const
{
  return name_list[n];
}


int PegCounter::itemCount(int n) const
{
  return count_list[n];
}


void PegCounter::addItem(QString name)
{
  for(unsigned i=0;i<name_list.size();i++) {
    if(name_list[i]==name) {
      count_list[i]++;
      return;
    }
  }
  name_list.push_back(name);
  count_list.push_back(1);
}


void PegCounter::clear()
{
  name_list.clear();
  count_list.clear();
}
