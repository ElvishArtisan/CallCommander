// telos100_states.h
//
// A Container Class for Telos 100 Line States.
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

#ifndef TELOS100_STATES_H
#define TELOS100_STATES_H

#include <vector>

class Telos100States
{
 public:
  Telos100States();
  unsigned size() const;
  char state(unsigned line) const;
  void setState(unsigned line,char state);
  bool locked(unsigned line) const;
  void setLocked(unsigned line,bool state);
  void clear();

 private:
  std::vector<char> telos_states;
  std::vector<bool> telos_locks;
};


#endif  // TELOS100_STATES_H
