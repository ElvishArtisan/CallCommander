// mlmld.h
//
// The CallCommander Logic Module Host Launcher
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MLMLD_H
#define MLMLD_H

#include <vector>

#include <QtCore/QObject>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3socketdevice.h>

#include <mldconfig.h>

#define MLMLD_USAGE "[-d]\n\nWhere '-d' places the daemon in \"debug\" mode.\n"

class MainObject : public QObject
{
  Q_OBJECT
 public:
  MainObject(QObject *parent=0,const char *name=0);

 private slots:
  void restartTimerData();

 private:
  void Init(bool initial_startup=false);
  void Release();
  QString GetCanonicalName() const;
  bool AuthenticateReloadPacket(char *b,int n);
  MldConfig *mlm_config;
  QSqlDatabase mlm_db;
  Q3SocketDevice *mlm_reload_socket;
  bool mlm_debug;
};


#endif  // MLMLD_H
