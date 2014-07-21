// ttysettings.h
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

#ifndef TTYSETTINGS_H
#define TTYSETTINGS_H

#include <QtCore/QString>

#include <mlttydevice.h>

class TtySettings
{
 public:
  TtySettings();
  QString name() const;
  void setName(const QString &name);
  int speed() const;
  void setSpeed(int speed);
  int wordLength() const;
  void setWordLength(int len);
  int stopBits() const;
  void setStopBits(int bits);
  MLTTYDevice::Parity parity() const;
  void setParity(MLTTYDevice::Parity parity);
  void clear();

 private:
  QString tty_name;
  int tty_speed;
  int tty_word_length;
  int tty_stop_bits;
  MLTTYDevice::Parity tty_parity;
};

#endif  // TTYSETTINGS_H
