// ttysettings.cpp
//
// A Container Class for TTY Parameter Settings
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

#include "ttysettings.h"

TtySettings::TtySettings()
{
  clear();
}


QString TtySettings::name() const
{
  return tty_name;
}


void TtySettings::setName(const QString &name)
{
  tty_name=name;
}


int TtySettings::speed() const
{
  return tty_speed;
}


void TtySettings::setSpeed(int speed)
{
  tty_speed=speed;
}


int TtySettings::wordLength() const
{
  return tty_word_length;
}


void TtySettings::setWordLength(int len)
{
  tty_word_length=len;
}


int TtySettings::stopBits() const
{
  return tty_stop_bits;
}


void TtySettings::setStopBits(int bits)
{
  tty_stop_bits=bits;
}


MLTTYDevice::Parity TtySettings::parity() const
{
  return tty_parity;
}


void TtySettings::setParity(MLTTYDevice::Parity parity)
{
  tty_parity=parity;
}


void TtySettings::clear()
{
  tty_name="";
  tty_speed=9600;
  tty_word_length=8;
  tty_stop_bits=1;
  tty_parity=MLTTYDevice::None;
}
