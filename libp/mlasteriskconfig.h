// mlasteriskconfig.h
//
// A container class for CallCommander Asterisk Configuration
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MLASTERISKCONFIG_H
#define MLASTERISKCONFIG_H

#include <vector>

#include <QtCore/QString>

#include <ml.h>

class MLAsteriskConfig
{
 public:
  MLAsteriskConfig();
  int asterisk2Line(const QString &chan) const;
  QString line2Asterisk(int line) const;
  int asterisk2Bank(const QString &chan) const;
  QString bank2Asterisk(int bank) const;
  int asterisk2Console(const QString &chan) const;
  QString console2Asterisk(int console) const;
  QString extension(int bank,int console) const;
  QString context(int bank,int console) const;
  QString parkingExtension() const;
  QString parkingContext() const;
  void load(std::vector<QString> *values);
  void clear();

 private:
  QString ask_lines[MAX_LINES];
  QString ask_banks[MAX_BANKS];
  QString ask_consoles[ML_MAX_CONSOLES];
  QString ask_bank_contexts[MAX_BANKS];
  QString ask_bank_extensions[MAX_BANKS];
  QString ask_console_contexts[ML_MAX_CONSOLES];
  QString ask_console_extensions[ML_MAX_CONSOLES];
  QString ask_parking_extension;
  QString ask_parking_context;
};


#endif  // MLASTERISKCONFIG_H
