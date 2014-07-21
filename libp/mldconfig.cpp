// mldconfig.cpp
//
// A container class for mld(8) Configuration
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

#include <stdio.h>

#include <QtGui/QMessageBox>
#include <QtCore/QSettings>

#include "mlprofile.h"
#include "mldconfig.h"


MldConfig::MldConfig()
{
  clear();
}


MldConfig::MldConfig(QString filename)
{
  clear();
  conf_filename=filename;
}


QString MldConfig::filename() const
{
  return conf_filename;
}


void MldConfig::setFilename(QString filename)
{
  conf_filename=filename;
}


QString MldConfig::mysqlHostname() const
{
  return conf_mysql_hostname;
}


void MldConfig::setMysqlHostname(const QString &str)
{
  conf_mysql_hostname=str;
}


QString MldConfig::mysqlUsername() const
{
  return conf_mysql_username;
}


void MldConfig::setMysqlUsername(const QString &str)
{
  conf_mysql_username=str;
}


QString MldConfig::mysqlPassword() const
{
  return conf_mysql_password;
}


void MldConfig::setMysqlPassword(const QString &str)
{
  conf_mysql_password=str;
}


QString MldConfig::mysqlDbname() const
{
  return conf_mysql_dbname;
}


void MldConfig::setMysqlDbname(const QString &str)
{
  conf_mysql_dbname=str;
}


QString MldConfig::mysqlDbtype() const
{
  return conf_mysql_dbtype;
}


int MldConfig::detailsRefreshInterval() const
{
  return conf_details_refresh_interval;
}


QString MldConfig::stationName() const
{
  return conf_station_name;
}


int MldConfig::mcidmdMaxBank() const
{
  return conf_mcidmd_max_bank;
}


void MldConfig::dumpConfig(FILE *stream)
{
  if(!mysqlHostname().isEmpty()) {
    fprintf(stream,"[mySQL]\n");
    fprintf(stream,
	    "  Hostname = %s\n",(const char *)mysqlHostname().toAscii());
    fprintf(stream,
	    "  Loginname = %s\n",(const char *)mysqlUsername().toAscii());
    fprintf(stream,
	    "  Database = %s\n",(const char *)mysqlDbname().toAscii());
    fprintf(stream,
	    "  Password = %s\n",(const char *)mysqlPassword().toAscii());
    fprintf(stream,"\n");
  }
  fprintf(stream,"[MCallMan]\n");
  fprintf(stream,"  DetailsRefreshInterval = %d\n",
	  conf_details_refresh_interval);
  fprintf(stream,"\n");
}


bool MldConfig::load()
{
#ifdef WIN32
  conf_station_name=QString("windows");
  QSettings settings;
  settings.setPath("SalemRadioLabs","CallCommander",QSettings::UserScope);
  settings.insertSearchPath(QSettings::Windows,"/SalemRadioLabs");
  conf_mysql_hostname=
    settings.value("/MysqlHostname",DEFAULT_MYSQL_HOSTNAME).toString();
  conf_mysql_username=
    settings.value("/MysqlUsername",DEFAULT_MYSQL_USERNAME).toString();
  conf_mysql_password=
    settings.value("/MysqlPassword").toString();
  conf_mysql_dbname=
    settings.value("/MysqlDatabase",DEFAULT_MYSQL_DATABASE).toString();
  conf_mysql_dbtype=
    settings.value("/MysqlServerType",DEFAULT_MYSQL_DBTYPE).toString();
  conf_details_refresh_interval=
    settings.value("/DetailsRefreshInterval",
		   ML_DEFAULT_DETAILS_REFRESH_INTERVAL).toInt();
  return true;
#else
  char sname[256];
  QString group;
  QString str;

  conf_station_name=gethostname(sname,255);

  MLProfile *profile=new MLProfile();
  profile->setSource(conf_filename);

  //
  // [Global] Section
  //
  //
  // [mySQL] Section
  //
  conf_mysql_hostname=
    profile->stringValue("mySQL","Hostname",DEFAULT_MYSQL_HOSTNAME);
  conf_mysql_username=
    profile->stringValue("mySQL","Loginname",DEFAULT_MYSQL_USERNAME);
  conf_mysql_password=
    profile->stringValue("mySQL","Password",DEFAULT_MYSQL_PASSWORD);
  conf_mysql_dbname=
    profile->stringValue("mySQL","Database",DEFAULT_MYSQL_DATABASE);
  conf_mysql_dbtype=
    profile->stringValue("mySQL","ServerType",DEFAULT_MYSQL_DBTYPE);

  //
  // [MCallMan] Section
  //
  conf_details_refresh_interval=profile->
    intValue("MCallMan","DetailsRefreshInterval",
	     ML_DEFAULT_DETAILS_REFRESH_INTERVAL);

  //
  // [Mcidmd] Section
  //
  conf_mcidmd_max_bank=profile->intValue("Mcidmd","MaxBank",MAX_BANKS);

  delete profile;
  return true;
#endif
}


bool MldConfig::save()
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("SalemRadioLabs","CallCommander",QSettings::SystemScope);
  settings.setValue("/MysqlHostname",conf_mysql_hostname);
  settings.setValue("/MysqlUsername",conf_mysql_username);
  settings.setValue("/MysqlPassword",conf_mysql_password);
  settings.setValue("/MysqlDatabase",conf_mysql_dbname);
  settings.setValue("/MysqlServerType",conf_mysql_dbtype);
  settings.setValue("/DetailsRefreshInterval",conf_details_refresh_interval);

#else
  FILE *f=NULL;
  QString filename=conf_filename;

#ifdef OS_X
  char temp_name[256];
  strcpy(temp_name,"/tmp/mdbadminXXXXXX");
  mktemp(temp_name);
  filename=temp_name;
#endif  // OS_X
  if((f=fopen(filename,"w"))==NULL) {
    return false;
  }
  fprintf(f,"[mySQL]\n");
  fprintf(f,"Hostname=%s\n",(const char *)conf_mysql_hostname.toAscii());
  fprintf(f,"Username=%s\n",(const char *)conf_mysql_username.toAscii());
  fprintf(f,"Password=%s\n",(const char *)conf_mysql_password.toAscii());
  fprintf(f,"Database=%s\n",(const char *)conf_mysql_dbname.toAscii());
  fprintf(f,"ServerType=%s\n",(const char *)conf_mysql_dbtype.toAscii());

  fprintf(f,"[MCallMan]\n");
  fprintf(f,"DetailsRefreshInterval=%d\n",conf_details_refresh_interval);
  fclose(f);
#ifdef OS_X
  QString cmd=QString("cat ")+filename+" | /usr/libexec/authopen -w "+MLD_CONF_FILE;
  system(cmd);
  unlink(filename);
#endif  // OS_X

#endif  // WIN32
  return true;
}


void MldConfig::clear()
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("SalemRadioLabs","CallCommander",QSettings::UserScope);
  settings.insertSearchPath(QSettings::Windows,"/SalemRadioLabs");
  conf_filename=
    QString().sprintf("%s\\%s",(const char *)settings.
		      readEntry("/InstallDir","C:\\etc"),
		      DEFAULT_WIN_CONF_FILE);
#else
  conf_filename=MLD_CONF_FILE;
#endif  // WIN32
  conf_mysql_hostname="";
  conf_mysql_username="";
  conf_mysql_password="";
  conf_mysql_dbname="";
  conf_mysql_dbtype="";
  conf_station_name="";
  conf_details_refresh_interval=ML_DEFAULT_DETAILS_REFRESH_INTERVAL;
  conf_mcidmd_max_bank=MAX_BANKS;
}


QTime MldConfig::LoadTime(QString &timestr,bool *ok)
{
  int hour;
  int min;
  int sec;
  char str[9];

  if(timestr.length()!=8) {
    *ok=false;
    return QTime();
  }
  strcpy(str,timestr);
  if((str[2]!=':')||(str[5]!=':')) {
    *ok=false;
    return QTime();
  }
  str[2]=0;
  if(sscanf(str,"%d",&hour)!=1) {
    *ok=false;
    return QTime();
  }
  str[5]=0;
  if(sscanf(str+3,"%d",&min)!=1) {
    *ok=false;
    return QTime();
  }
  if(sscanf(str+6,"%d",&sec)!=1) {
    *ok=false;
    return QTime();
  }
  *ok=true;
  return QTime(hour,min,sec);
}
