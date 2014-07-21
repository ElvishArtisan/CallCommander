// connection_data.cpp
//
// A Container Class for CallCommander Connection Entries.
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

#include <stdlib.h>
#include <stdio.h>

#include <QtCore/QDir>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <mlprofile.h>

#include "ml.h"
#include "connection_data.h"

ConnectionData::ConnectionData(ConnectionData::StorageType storage_type)
{
  clear();
  data_storage_type=storage_type;
  data_scope=QSettings::UserScope;
#ifdef WIN32
  QSettings s;
  s.setPath("SalemRadioLabs","CallCommander",QSettings::SystemScope);
  s.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
  if(s.readEntry("ConnectionManagerScope").toLower()=="system") {
    data_scope=QSettings::SystemScope;
  }
#endif  // WIN32
}


bool ConnectionData::exists() const
{
  bool ret=false;
  QSettings settings;
  QString key;

  switch(data_storage_type) {
      case ConnectionData::StorageNone:
	break;

      case ConnectionData::StorageFilesystem:
#ifdef WIN32
	settings.setPath("SalemRadioLabs","CallCommander",data_scope);
	settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
	key=QString().sprintf("%s/%s/",ML_REGISTRY_CONNECTION_DIR,
			      (const char *)data_name);
	if(settings.readNumEntry(key+"SystemType",0)!=0) {
	  ret=true;
	}
#else
	ret=QFile::exists(defaultFilename(data_name));
#endif
	break;

      case ConnectionData::StorageDatabase:
	QSqlQuery *q=
	  new QSqlQuery(QString().sprintf("select NAME from CONNECTIONS \
                                           where NAME=\"%s\"",
					  (const char *)data_name));
	ret=q->first();
	delete q;
	break;
  }
  return ret;
}


ConnectionData::SystemType ConnectionData::systemType() const
{
  return data_system_type;
}


void ConnectionData::setSystemType(ConnectionData::SystemType type)
{
  data_system_type=type;
}


int ConnectionData::subType() const
{
  return data_sub_type;
}


void ConnectionData::setSubType(int subtype)
{
  data_sub_type=subtype;
}


ConnectionData::StorageType ConnectionData::storageType() const
{
  return data_storage_type;
}


BusDriver::ConnectionType ConnectionData::connectionType() const
{
  return data_connection_type;
}


void ConnectionData::setConnectionType(BusDriver::ConnectionType type)
{
  data_connection_type=type;
}


QString ConnectionData::name() const
{
  return data_name;
}


void ConnectionData::setName(const QString &name)
{
  data_name=name;
}


QString ConnectionData::description() const
{
  return data_description;
}


void ConnectionData::setDescription(const QString &desc)
{
  data_description=desc;
}


QString ConnectionData::hostName() const
{
  return data_host_name;
}


void ConnectionData::setHostName(const QString &host_name)
{
  data_host_name=host_name;
}


Q_UINT16 ConnectionData::tcpPort() const
{
  return data_tcp_port;
}


void ConnectionData::setTcpPort(Q_UINT16 port)
{
  data_tcp_port=port;
}


QString ConnectionData::ttyPort() const
{
  return data_tty_port;
}


void ConnectionData::setTtyPort(const QString &port)
{
  data_tty_port=port;
}


int ConnectionData::ttySpeed() const
{
  return data_tty_speed;
}


void ConnectionData::setTtySpeed(int speed)
{
  data_tty_speed=speed;
}


int ConnectionData::ttyWordLength() const
{
  return data_tty_word_length;
}


void ConnectionData::setTtyWordLength(int len)
{
  data_tty_word_length=len;
}


int ConnectionData::ttyStopBits() const
{
  return data_tty_stop_bits;
}


void ConnectionData::setTtyStopBits(int bits)
{
  data_tty_stop_bits=bits;
}


MLTTYDevice::Parity ConnectionData::ttyParity() const
{
  return data_tty_parity;
}


void ConnectionData::setTtyParity(MLTTYDevice::Parity parity)
{
  data_tty_parity=parity;
}


QString ConnectionData::userName() const
{
  return data_user_name;
}


void ConnectionData::setUserName(const QString &user_name)
{
  data_user_name=user_name;
}


QString ConnectionData::userPassword() const
{
  return data_user_passwd;
}


void ConnectionData::setUserPassword(const QString &passwd)
{
  data_user_passwd=passwd;
}


QString ConnectionData::showName() const
{
  return data_show_name;
}


void ConnectionData::setShowName(const QString &show_name)
{
  data_show_name=show_name;
}


QString ConnectionData::showPassword() const
{
  return data_show_passwd;
}


void ConnectionData::setShowPassword(const QString &passwd)
{
  data_show_passwd=passwd;
}


QString ConnectionData::location() const
{
  return data_location;
}


void ConnectionData::setLocation(const QString &host_name)
{
  data_location=host_name;
}


int ConnectionData::console() const
{
  return data_console;
}


void ConnectionData::setConsole(int console)
{
  data_console=console;
}


QString ConnectionData::consoleName() const
{
  return data_console_name;
}


void ConnectionData::setConsoleName(const QString &name)
{
  data_console_name=name;
}


bool ConnectionData::startConnection() const
{
  return data_start_connection;
}


void ConnectionData::setStartConnection(bool state)
{
  data_start_connection=state;
}


bool ConnectionData::offerStartConnection() const
{
  return data_offer_start_connection;
}


void ConnectionData::setOfferStartConnection(bool state)
{
  data_offer_start_connection=state;
}


bool ConnectionData::load()
{
  switch(data_storage_type) {
      case ConnectionData::StorageNone:
	break;

      case ConnectionData::StorageFilesystem:
	return LoadFile(data_name);

      case ConnectionData::StorageDatabase:
	return LoadTable(data_name);
  }
  return false;
}


bool ConnectionData::save() const
{
  switch(data_storage_type) {
      case ConnectionData::StorageNone:
	break;

      case ConnectionData::StorageFilesystem:
	return SaveFile(data_name);

      case ConnectionData::StorageDatabase:
	return SaveTable(data_name);
  }
  return false;
}


void ConnectionData::remove() const
{
  QSqlQuery *q;
  QSettings settings;
  QStringList slist;
  QStringList::iterator it;
  QString key;

  switch(data_storage_type) {
      case ConnectionData::StorageNone:
	break;

      case ConnectionData::StorageFilesystem:
#ifdef WIN32
	settings.setPath("SalemRadioLabs","CallCommander",data_scope);
	settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
	key=QString().sprintf("%s/%s",ML_REGISTRY_CONNECTION_DIR,
			      (const char *)data_name);
	slist=settings.entryList(key);
	for(it=slist.begin();it!=slist.end();++it) {	
	  settings.removeEntry(key+"/"+(*it));
	}
	settings.removeEntry(key);
#else
	QFile::remove(defaultFilename(data_name));
#endif  // WIN32
	break;

      case ConnectionData::StorageDatabase:
	q=new QSqlQuery(QString().sprintf("delete from CONNECTIONS \
                                           where NAME=\"%s\"",
					  (const char *)data_name));
	delete q;
	break;
  }
}


void ConnectionData::clear()
{
  data_name="";
  data_storage_type=ConnectionData::StorageNone;
  data_system_type=ConnectionData::TypeUnknown;
  data_sub_type=0;
  data_description="";
  data_host_name="";
  data_tcp_port=0;
  data_tty_port="";
  data_user_name="";
  data_user_passwd="";
  data_show_name="";
  data_show_passwd="";
  data_location="";
  data_console=-1;
  data_console_name="";
  data_start_connection=false;
  data_offer_start_connection=false;
  data_tty_speed=0;
  data_tty_word_length=8;
  data_tty_stop_bits=1;
  data_tty_parity=MLTTYDevice::None;
};


QString ConnectionData::defaultFilename(const QString &name)
{
  return QString().sprintf("%s/%s/%s.ccc",(const char *)QDir::homeDirPath(),
			   ML_CONFIG_DIR,(const char *)name);
}


QStringList ConnectionData::connections(ConnectionData::StorageType type)
{
  QString confdir="/";
  QStringList slist;
  QStringList ret;
  QStringList::iterator it;
  QDir dir;
  QSettings s;
  QSettings settings;
#ifdef WIN32
  QSettings::Scope scope;
#endif  // WIN32
  switch(type) {
      case ConnectionData::StorageNone:
	break;

      case ConnectionData::StorageFilesystem:
#ifdef WIN32
	s.setPath("SalemRadioLabs","CallCommander",QSettings::SystemScope);
	s.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
	if(s.readEntry("ConnectionManagerScope").toLower()=="system") {
	  scope=QSettings::SystemScope;
	}
	settings.setPath("SalemRadioLabs","CallCommander",scope);
	settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
	ret=settings.subkeyList(ML_REGISTRY_CONNECTION_DIR);
#else
	//
	// Get the config directory, creating it if needs be
	//
	if(getenv("HOME")!=NULL) {
	  confdir=getenv("HOME");
	}
	if(confdir.right(1)!="/") {
	  confdir+="/";
	}
 	confdir+=ML_CONFIG_DIR;
	dir.setPath(confdir);
	if(!dir.exists()) {
	  if(!dir.mkdir(confdir,true)) {
	    return slist;
	  }
	}
	slist=dir.entryList();
	for(it=slist.begin();it!=slist.end();++it) {
	  ret.push_back((*it).left((*it).length()-
				   (strlen(ML_CONNECTION_EXTENSION)+1)));
	}
#endif  // WIN32
	break;

      case ConnectionData::StorageDatabase:
	QSqlQuery *q=new QSqlQuery("select NAME from CONNECTIONS");
	while(q->next()) {
	  ret.push_back(q->value(0).toString());
	}
	delete q;
	break;
  }
  return ret;
}


bool ConnectionData::LoadFile(const QString &name)
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("SalemRadioLabs","CallCommander",data_scope);
  settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
  QString key=
    QString().sprintf("%s/%s/",ML_REGISTRY_CONNECTION_DIR,(const char *)name);
  data_system_type=(ConnectionData::SystemType)settings.
    readNumEntry(key+"SystemType",0);
  if(data_system_type==0) {
    return false;
  }
  data_description=settings.readEntry(key+"Description");
  data_host_name=settings.readEntry(key+"HostName");
  data_tcp_port=settings.readNumEntry(key+"TcpPort");
  data_tty_port=settings.readEntry(key+"TtyPort");
  data_user_name=settings.readEntry(key+"UserName");
  data_user_passwd=settings.readEntry(key+"UserPassword");
  data_show_name=settings.readEntry(key+"ShowName");
  data_show_passwd=settings.readEntry(key+"ShowPassword");
  data_location=settings.readEntry(key+"Location");
  data_console=settings.readNumEntry(key+"Console");
  data_console_name=settings.readEntry(key+"ConsoleName");
#else
  QString filename;
  MLProfile *profile=NULL;
#ifdef OS_X
  profile=new MLProfile();
  filename=QDir::currentPath()+"/CallScreener.app/Contents/Resources/"+name+".ccc";
  if(!profile->setSource(filename)) {
    delete profile;
    profile=NULL;
  }
#endif  // OS_X
  if(profile==NULL) {
    profile=new MLProfile();
    filename=defaultFilename(name);
    profile->setSource(filename);
  }
  if((data_system_type=(ConnectionData::SystemType)profile->
      intValue("Connection","SystemType",0))==
     (int)ConnectionData::TypeUnknown) {
    delete profile;
    return false;
  }
  data_description=profile->
    stringValue("Connection","Description","CallCommander Connection");
  data_system_type=(ConnectionData::SystemType)profile->
    intValue("Connection","SystemType",ConnectionData::TypeUnknown);
  data_host_name=profile->
    stringValue("Connection","HostName","localhost");
  data_tcp_port=profile->intValue("Connection","TcpPort",0);
  data_tty_port=profile->
    stringValue("Connection","TtyPort","");
  data_user_name=profile->
    stringValue("Connection","UserName","tlsap");
  data_user_passwd=profile->
    stringValue("Connection","UserPassword","");
  data_show_name=profile->
    stringValue("Connection","ShowName","Hybrid1&2");
  data_show_passwd=profile->
    stringValue("Connection","ShowPassword","");
  data_location=profile->stringValue("Connection","Location","");
  data_console=profile->intValue("Connection","Console",ML_MAX_CONSOLES);
  data_console_name=profile->
    stringValue("Connection","ConsoleName",
		QString().sprintf("Console %d",data_console));
  delete profile;
#endif  // WIN32
  return true;
}


bool ConnectionData::LoadTable(const QString &name)
{
  QString sql;
  QSqlQuery *q;
  bool ret=false;

  sql=QString().sprintf("select DESCRIPTION,SYSTEM_TYPE,HOST_NAME,TCP_PORT,\
                         TTY_PORT,USER_NAME,USER_PASSWORD,SHOW_NAME,\
                         SHOW_PASSWORD,LOCATION,CONSOLE,CONSOLE_NAME \
                         from CONNECTIONS where NAME=\"%s\"",
			(const char *)data_name);
  q=new QSqlQuery(sql);
  if((ret=q->first())) {
    data_description=q->value(0).toString();
    data_system_type=(ConnectionData::SystemType)q->value(1).toInt();
    data_host_name=q->value(2).toString();
    data_tcp_port=q->value(3).toUInt();
    data_tty_port=q->value(4).toString();
    data_user_name=q->value(5).toString();
    data_user_passwd=q->value(6).toString();
    data_show_name=q->value(7).toString();
    data_show_passwd=q->value(8).toString();
    data_location=q->value(9).toString();
    data_console=q->value(10).toInt();
    data_console_name=q->value(11).toString();
  }
  delete q;
  return ret;
}


bool ConnectionData::SaveFile(const QString &name) const
{
#ifdef WIN32
  QSettings settings;
  settings.setPath("SalemRadioLabs","CallCommander",data_scope);
  settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
  QString key=
    QString().sprintf("%s/%s/",ML_REGISTRY_CONNECTION_DIR,(const char *)name);
  settings.writeEntry(key+"SystemType",(int)data_system_type);
  settings.writeEntry(key+"Description",data_description);
  settings.writeEntry(key+"HostName",data_host_name);
  settings.writeEntry(key+"TcpPort",data_tcp_port);
  settings.writeEntry(key+"TtyPort",data_tty_port);
  settings.writeEntry(key+"UserName",data_user_name);
  settings.writeEntry(key+"UserPassword",data_user_passwd);
  settings.writeEntry(key+"ShowName",data_show_name);
  settings.writeEntry(key+"ShowPassword",data_show_passwd);
  settings.writeEntry(key+"Location",data_location);
  settings.writeEntry(key+"Console",data_console);
  settings.writeEntry(key+"ConsoleName",data_console_name);
#else
  QString filename=defaultFilename(name);

  FILE *file=NULL;
  file=fopen(filename,"w");
  if(file==NULL) {
    return false;
  }
  fprintf(file,"[Connection]\n");
  fprintf(file,"SystemType=%d\n",data_system_type);
  fprintf(file,"Name=%s\n",(const char *)data_name);
  fprintf(file,"Description=%s\n",(const char *)data_description);
  fprintf(file,"HostName=%s\n",(const char *)data_host_name);
  fprintf(file,"TcpPort=%u\n",((unsigned)data_tcp_port)&0xffff);
  fprintf(file,"TtyPort=%s\n",(const char *)data_tty_port);
  fprintf(file,"UserName=%s\n",(const char *)data_user_name);
  fprintf(file,"UserPassword=%s\n",(const char *)data_user_passwd);
  fprintf(file,"ShowName=%s\n",(const char *)data_show_name);
  fprintf(file,"ShowPassword=%s\n",(const char *)data_show_passwd);
  fprintf(file,"Location=%s\n",(const char *)data_location);
  fprintf(file,"Console=%d\n",data_console);
  fprintf(file,"ConsoleName=%s\n",(const char *)data_console_name);
  fclose(file);
#endif  // WIN32
  return true;
}


bool ConnectionData::SaveTable(const QString &name) const
{
  QString sql;
  QSqlQuery *q;

  remove();
  sql=QString().sprintf("insert into CONNECTIONS set \
                         NAME=\"%s\",\
                         DESCRIPTION=\"%s\",\
                         SYSTEM_TYPE=%d,\
                         HOST_NAME=\"%s\",\
                         TCP_PORT=%u,\
                         TTY_PORT=\"%s\",\
                         USER_NAME=\"%s\",\
                         USER_PASSWORD=\"%s\",\
                         SHOW_NAME=\"%s\",\
                         SHOW_PASSWORD=\"%s\",\
                         LOCATION=\"%s\",\
                         CONSOLE=%d,\
                         CONSOLE_NAME=\"%s\"",
			(const char *)data_name,
			(const char *)data_description,
			data_system_type,
			(const char *)data_host_name,
			data_tcp_port,
			(const char *)data_tty_port,
			(const char *)data_user_name,
			(const char *)data_user_passwd,
			(const char *)data_show_name,
			(const char *)data_show_passwd,
			(const char *)data_location,
			data_console,
			(const char *)data_console_name);
  q=new QSqlQuery(sql);
  delete q;
  
  return true;
}
