// mldconfig.h
//
// A container class for CallCommander mld(8) Configuration
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

#ifndef MLDCONFIG_H
#define MLDCONFIG_H

#include <vector>

#include <QtCore/QString>

#include <ml.h>
#include <mldshow.h>

using namespace std;

//
// Default Configuration Values
//
#define DEFAULT_CONF_FILE "/etc/mld.conf"
#define DEFAULT_WIN_CONF_FILE "mld.ini"


class MldConfig
{
 public:
  enum SwitchProtocol {Unknown=0,Telos1A2=1,Telos1X6=2,TelosDim=3,
		       TelosTwoX12=4,Ts612=5,Telos2101=6};
  enum LabelAction {Blank=0,UseFirstKnown=1,UseLastKnown=2,UseSpecified=3};
  enum RouteAction {Accept=0,Warn=1,Drop=2};
  enum DirectorMode {Talent='T',Producer='F'};
  MldConfig();
  MldConfig(QString filename);
  QString filename() const;
  void setFilename(QString filename);
  QString stationName() const;
  QString mysqlHostname() const;
  void setMysqlHostname(const QString &str);
  QString mysqlUsername() const;
  void setMysqlUsername(const QString &str);
  QString mysqlPassword() const;
  void setMysqlPassword(const QString &str);
  QString mysqlDbname() const;
  void setMysqlDbname(const QString &str);
  QString mysqlDbtype() const;
  int detailsRefreshInterval() const;
  int mcidmdMaxBank() const;
  void dumpConfig(FILE *stream);
  bool load();
  bool save();
  void clear();

 private:
  QTime LoadTime(QString &timestr,bool *ok);
  QString conf_filename;
  QString conf_mysql_hostname;
  QString conf_mysql_username;
  QString conf_mysql_password;
  QString conf_mysql_dbname;
  QString conf_mysql_dbtype;
  QString conf_station_name;
  int conf_mcidmd_max_bank;
  int conf_details_refresh_interval;
};


#endif  // MLDCONFIG_H
