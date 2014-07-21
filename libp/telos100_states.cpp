// telos100_states.cpp
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

#include "telos100_states.h"

Telos100States::Telos100States()
{
  clear();
}


unsigned Telos100States::size() const
{
  return telos_states.size();
}


char Telos100States::state(unsigned line) const
{
  if(line<telos_states.size()) {
    return telos_states[line];
  }
  return 'Z';
}


void Telos100States::setState(unsigned line,char state)
{
  for(unsigned i=telos_states.size();i<=line;i++) {
    telos_states.push_back('Z');
  }
  telos_states[line]=state;
}


bool Telos100States::locked(unsigned line) const
{
  if(line<telos_locks.size()) {
    return telos_locks[line];
  }
  return false;
}


void Telos100States::setLocked(unsigned line,bool state)
{
  for(unsigned i=telos_locks.size();i<=line;i++) {
    telos_locks.push_back(false);
  }
  telos_locks[line]=state;
}


void Telos100States::clear()
{
  telos_states.clear();
  telos_locks.clear();
}
