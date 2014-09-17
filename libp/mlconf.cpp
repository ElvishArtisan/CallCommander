// mlconf.c
// A small library for handling common configuration file tasks
// 
// Adopted from conflib
//
//   (C) Copyright 1996-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlconf.cpp,v 1.6 2010/11/22 13:11:40 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <QtNetwork/QHostAddress>
#include <QtCore/QVariant>
#include <QtGui/QMessageBox>
#include <QtCore/QDir>
#include <QtSql/QSqlQuery>
#include "mlconf.h"
#ifndef WIN32
#include <unistd.h>
#include <netdb.h>
#endif

#define BUFFER_SIZE 1024

int GetPrivateProfileBool(const char *sFilename,const char *cHeader,
			 const char *cLabel,bool bDefault=false)
{
  char temp[255];

  if(GetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",254)<0) {
    return bDefault;
  }
  if(temp[0]==0) {
    return bDefault;
  }
#ifdef WIN32
  if((!stricmp(temp,"yes"))||(!stricmp(temp,"on"))) {
    return true;
  }
  if((!stricmp(temp,"no"))||(!stricmp(temp,"off"))) {
    return false;
  }
#else
  if((!strcasecmp(temp,"yes"))||(!strcasecmp(temp,"on"))) {
    return true;
  }
  if((!strcasecmp(temp,"no"))||(!strcasecmp(temp,"off"))) {
    return false;
  }
#endif
  return bDefault;
}


int GetPrivateProfileString(const char *sFilename,const char *cHeader,
			    const char *cLabel,char *cValue,
			    const char *cDefault,int dValueLength)
{
  int i;
  
  i=GetIni(sFilename,cHeader,cLabel,cValue,dValueLength);
  if(i==0) {
    return 0;
  }
  else {
    strcpy(cValue,cDefault);
    return -1;
  }
}


int GetPrivateProfileInt(const char *sFilename,const char *cHeader,
			 const char *cLabel,int dDefault)
{
  int c;
  char sNum[12];

  if(GetIni(sFilename,cHeader,cLabel,sNum,11)==0) {
    if(sscanf(sNum,"%d",&c)==1) {
      return c;
    }
    else {
      return dDefault;
    }
  }
  else {
    return dDefault;
  }
}


int GetPrivateProfileHex(const char *sFilename,const char *cHeader,
			 const char *cLabel,int dDefault)
{
  char temp[256];
  int n=dDefault;

  GetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",255);
  sscanf(temp,"0x%x",&n);
  return n;
}


double GetPrivateProfileDouble(const char *sFilename,const char *cHeader,
			 const char *cLabel,double dfDefault)
{
  char temp[256];
  double n=dfDefault;

  GetPrivateProfileString(sFilename,cHeader,cLabel,temp,"",255);
  sscanf(temp,"%lf",&n);
  return n;
}


int GetIni(const char *sFileName,const char *cHeader,const char *cLabel,
	   char *cValue,int dValueLength)	
     /* get a value from the ini file */
     
{
  FILE *cIniName;
  char sName[BUFFER_SIZE];
  char cIniBuffer[BUFFER_SIZE],cIniHeader[80],cIniLabel[80];
  int i,j;
  /*  int iFileStat=NULL;  */
  int iFileStat;

  strcpy(sName,sFileName);
#ifdef WIN32
  for(int i=0;i<strlen(sName);i++) {
    if(sName[i]=='/') {
      sName[i]='\\';
    }
  }
#endif  // WIN32
  cIniName=fopen(sName,"r");
  if(cIniName==NULL) {
    return 2;	/* ini file doesn't exist! */
  }
  while(GetIniLine(cIniName,cIniBuffer)!=EOF) {
    if(cIniBuffer[0]=='[') {	/* start of section */
      i=1;
      while(cIniBuffer[i]!=']' && cIniBuffer!=0) {
	cIniHeader[i-1]=cIniBuffer[i];
	i++;
      }
      cIniHeader[i-1]=0;
      if(strcmp(cIniHeader,cHeader)==0) {		/* this is the right section! */
	iFileStat=EOF+1;   /* Make this anything other than EOF! */
	while(iFileStat!=EOF) {
	  iFileStat=GetIniLine(cIniName,cIniBuffer);
	  if(cIniBuffer[0]=='[') return 1;
	  i=0;
	  while(cIniBuffer[i]!='=' && cIniBuffer[i]!=0) {
	    cIniLabel[i]=cIniBuffer[i];
	    i++;
	  }
	  cIniLabel[i++]=0;
	  if(strcmp(cIniLabel,cLabel)==0) {	/* this is the right label! */
	    j=0;
	    while(j<dValueLength && cIniBuffer[i]!=0) {
	      cValue[j++]=cIniBuffer[i++];
	    }
	    cValue[j]=0;
	    fclose(cIniName);
	    return 0;		/* value found! */
	  }
	}
      }
    }
  }
  fclose(cIniName);
  return 1;		/* section or label not found! */
}




int GetIniLine(FILE *cIniName,char *cIniBuffer)		/* read a line from the ini file */
     
{
  int i;
  
  for(i=0;i<BUFFER_SIZE-1;i++) {
    cIniBuffer[i]=getc(cIniName);
    switch(cIniBuffer[i]) {
      
    case EOF:
      cIniBuffer[i]=0;
      return EOF;
      
    case 10:
      cIniBuffer[i]=0;
      return 0;
    }
  }
  return 0;
}


QString MLGetPathPart(QString path)
{
  int c;

  c=path.findRev('/');
  if(c<0) {
    return QString("");
  }
  path.truncate(c+1);
  return path;
}


QString MLGetBasePart(QString path)
{
  int c;

  c=path.findRev('/');
  if(c<0) {
    return path;
  }
  path.remove(0,c+1);
  return path;
}


QString MLGetShortDate(QDate date)
{
  return QString().sprintf("%02d/%02d/%04d",
			   date.month(),date.day(),date.year());
}

QString MLGetShortDayNameEN(int weekday)
{
  QString day_name;
  if ( weekday < 1 || weekday > 7 )
    weekday = 1;

  if (weekday == 1)
    day_name = "Mon";
  else if (weekday == 2)
    day_name = "Tue";
  else if (weekday == 3)
    day_name = "Wed";
  else if (weekday == 4)
    day_name = "Thu";
  else if (weekday == 5)
    day_name = "Fri";
  else if (weekday == 6)
    day_name = "Sat";
  else if (weekday == 7)
    day_name = "Sun";
  return day_name;
}

QFont::Weight MLGetFontWeight(QString string)
{
  if(string.contains("Light",false)) {
    return QFont::Light;
  }
  if(string.contains("Normal",false)) {
    return QFont::Normal;
  }
  if(string.contains("DemiBold",false)) {
    return QFont::DemiBold;
  }
  if(string.contains("Bold",false)) {
    return QFont::Bold;
  }
  if(string.contains("Black",false)) {
    return QFont::Black;
  }
  return QFont::Normal;
}


#ifndef WIN32
bool MLDetach()
{
  if(daemon(0,0)) {
    return false;
  }
  return true;
}
#endif


bool MLBool(QString string)
{
  if(string.contains("Y",false)) {
    return true;
  }
  return false;
}


QString MLYesNo(bool state)
{
  if(state) {
    return QString("Y");
  }
  return QString("N");
}


#ifndef WIN32
QHostAddress MLGetHostAddr()
{
  FILE *file;
  char host_name[256];
  struct hostent *host_ent;
  int host_address;

  if((file=fopen("/etc/HOSTNAME","r"))==NULL) {
    return QHostAddress();
  }
  if(fscanf(file,"%s",host_name)!=1) {
    return QHostAddress();
  }
  if((host_ent=gethostbyname(host_name))==NULL) {
    return QHostAddress();
  }
  host_address=16777216*(host_ent->h_addr_list[0][0]&0xff)+
    65536*(host_ent->h_addr_list[0][1]&0xff)+
    256*(host_ent->h_addr_list[0][2]&0xff)+
    (host_ent->h_addr_list[0][3]&0xff);
  return QHostAddress((Q_UINT32)host_address);
}
#endif  // WIN32


QString MLGetDisplay(bool strip_point)
{
#ifdef WIN32
  return QString("win32");
#else
  QString display;
  int l;

  if(getenv("DISPLAY")[0]==':') {
    display=QString().sprintf("%s%s",(const char *)MLGetHostAddr().toString(),
			     getenv("DISPLAY"));
  }
  else {
    display=QString(getenv("DISPLAY"));
  }
  if(strip_point) {
    l=display.length();
    while(display.at(l)!=':') {
      if(display.at(l--)=='.') {
	return display.left(l+1);
      }    
    }
  }
  return display;
#endif  // WIN32
}


bool MLDoesRowExist(QString table,QString name,QString test)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			(const char *)name,
			(const char *)table,
			(const char *)name,
			(const char *)test);
  q=new QSqlQuery(sql);
  if(q->size()>0) {
    delete q;
    return true;
  }
  delete q;
  return false;
}


bool MLDoesRowExist(QString table,QString name,unsigned test)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%d",
			(const char *)name,
			(const char *)table,
			(const char *)name,
			test);
  q=new QSqlQuery(sql);
  if(q->size()>0) {
    delete q;
    return true;
  }
  delete q;
  return false;
}


QVariant MLGetSqlValue(QString table,QString name,QString test,
		      QString param,bool *valid)
{
  QSqlQuery *q;
  QString sql;
  QVariant v;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			(const char *)param,
			(const char *)table,
			(const char *)name,
			(const char *)test);
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    v=q->value(0);
    if(valid!=NULL) {
      *valid=!q->isNull(0);
    }
    delete q;
    return v;
  }
  delete q;
  return QVariant();
}


bool MLIsSqlNull(QString table,QString name,QString test,
		QString param)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=\"%s\"",
			(const char *)param,
			(const char *)table,
			(const char *)name,
			(const char *)test);
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    if(q->isNull(0)) {
      delete q;
      return true;
    }
    else {
      delete q;
      return false;
    }
  }
  delete q;
  return true;
}


bool MLIsSqlNull(QString table,QString name,unsigned test,
		QString param)
{
  QSqlQuery *q;
  QString sql;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%d",
			(const char *)param,
			(const char *)table,
			(const char *)name,
			test);
  q=new QSqlQuery(sql);
  if(q->isActive()) {
    q->first();
    if(q->isNull(0)) {
      delete q;
      return true;
    }
    else {
      delete q;
      return false;
    }
  }
  delete q;
  return true;
}


QVariant MLGetSqlValue(QString table,QString name,unsigned test,
		      QString param,bool *valid)
{
  QSqlQuery *q;
  QString sql;
  QVariant v;

  sql=QString().sprintf("SELECT %s FROM %s WHERE %s=%u",
			(const char *)param,
			(const char *)table,
			(const char *)name,
			test);
  q=new QSqlQuery(sql);
  if(q->first()) {
    v=q->value(0);
    if(valid!=NULL) {
      *valid=!q->isNull(0);
    }
    delete q;
    return v;
  }
  delete q;
  return QVariant();
}


QString MLGetTimeLength(int mseconds,bool leadzero,bool tenths)
{
  int hour,min,seconds,tenthsecs;
  char negative[2];

  if(mseconds<0) {
    mseconds=-mseconds;
    strcpy(negative,"-");
  }
  else {
    negative[0]=0;
  }
  QTime time_length(QTime(0,0,0).addMSecs(mseconds));
  hour = time_length.hour();
  min = time_length.minute();
  seconds = time_length.second();
  mseconds = time_length.msec();
  tenthsecs=mseconds/100;
  if(leadzero) {
    if(tenths) {
     return QString().sprintf("%s%d:%02d:%02d.%d",negative,hour,min,seconds,
			      tenthsecs);
    }
    return QString().sprintf("%s%d:%02d:%02d",negative,hour,min,seconds);
  }
  if((hour==0)&&(min==0)) {
    if(tenths) {
      return QString().sprintf("%s:%02d.%d",negative,seconds,tenthsecs);
    }
    return QString().sprintf("%s:%02d",negative,seconds);
  }
  if(hour==0) {
    if(tenths) {
      return QString().sprintf("%s%2d:%02d.%d",negative,min,seconds,
			       tenthsecs);
    }
    return QString().sprintf("%s%2d:%02d",negative,min,seconds);
  }
  if(tenths) {
    return QString().sprintf("%s%2d:%02d:%02d.%d",negative,hour,min,seconds,
			     tenthsecs);
  }
  return QString().sprintf("%s%2d:%02d:%02d",negative,hour,min,seconds);
}


int MLSetTimeLength(QString str)
{
  int istate=2;
  QString field;
  int res=0;

  if(str.isEmpty()) {
    return -1;
  }
  for(int i=0;i<str.length();i++) {
    if(str.at(i)==':') {
      istate--;
    }
  }
  if(istate<0) {
    return -1;
  }
  for(int i=0;i<str.length();i++) {
    if(str.at(i).isNumber()) {
      field+=str.at(i);
    }
    else {
      if((str.at(i)==':')||(str.at(i)=='.')) {
	if(field.length()>2) {
	  return -1;
	}
	switch(istate) {
	    case 0:
	      res+=3600000*field.toInt();
	      break;

	    case 1:
	      res+=60000*field.toInt();
	      break;

	    case 2:
	      res+=1000*field.toInt();
	      break;
	}
	istate++;
	field="";
      }
      else {
	if(!str.at(i).isSpace()) {
	  return -2;
	}
      }
    }
  }
  switch(istate) {
      case 2:
	res+=1000*field.toInt();
	break;

      case 3:
	switch(field.length()) {
	    case 1:
	      res+=100*field.toInt();
	      break;

	    case 2:
	      res+=10*field.toInt();
	      break;

	    case 3:
	      res+=field.toInt();
	      break;
	}
  }

  return res;
}


#ifndef WIN32
bool MLCopy(QString srcfile,QString destfile)
{
  int src_fd;
  int dest_fd;
  struct stat src_stat;
  struct stat dest_stat;
  char *buf=NULL;
  int n;

  if((src_fd=open((const char *)srcfile,O_RDONLY))<0) {
    return false;
  }
  if(fstat(src_fd,&src_stat)<0) {
    close(src_fd);
    return false;
  }
  if((dest_fd=open((const char *)destfile,O_RDWR|O_CREAT,src_stat.st_mode))
     <0) {
    close(src_fd);
    return false;
  }
  if(fstat(dest_fd,&dest_stat)<0) {
    close(src_fd);
    close(dest_fd);
    return false;
  }
  buf=(char *)malloc(dest_stat.st_blksize);
  while((n=read(src_fd,buf,dest_stat.st_blksize))==dest_stat.st_blksize) {
    write(dest_fd,buf,dest_stat.st_blksize);
  }
  write(dest_fd,buf,n);
  free(buf);
  close(src_fd);
  close(dest_fd);
  return true;
}
#endif  // WIN32


#ifndef WIN32
bool MLWritePid(QString dirname,QString filename,int owner,int group)
{
  FILE *file;
  mode_t prev_mask;
  QString pathname=QString().sprintf("%s/%s",
				     (const char *)dirname,
				     (const char *)filename);

  prev_mask = umask(0113);      // Set umask so pid files are user and group writable.
  file=fopen((const char *)pathname,"w");
  umask(prev_mask);
  if(file==NULL) {
    return false;
  }
  fprintf(file,"%d",getpid());
  fclose(file);
  chown((const char *)pathname,owner,group);

  return true;
}


void MLDeletePid(QString dirname,QString filename)
{
  QString pid=QString().sprintf("%s/%s",
				(const char *)dirname,
				(const char *)filename);
  unlink((const char *)pid);
}


bool MLCheckPid(QString dirname,QString filename)
{
  QDir dir;
  QString path;
  path=QString("/proc/")+
    QString().sprintf("%d",MLGetPid(dirname+QString("/")+filename));
  dir.setPath(path);
  return dir.exists();
}


pid_t MLGetPid(QString pidfile)
{
  FILE *handle;
  pid_t ret;

  if((handle=fopen((const char *)pidfile,"r"))==NULL) {
    return -1;
  }
  if(fscanf(handle,"%d",&ret)!=1) {
    ret=-1;
  }
  fclose(handle);
  return ret;
}
#endif  // WIN32


QString MLGetHomeDir(bool *found)
{
  if(getenv("HOME")==NULL) {
    if(found!=NULL) {
      *found=false;
    }
    return QString("/");
  }
  if(found!=NULL) {
    *found=true;
  }
  return QString(getenv("HOME"));
}


QString MLTruncateAfterWord(QString str,int word,bool add_dots)
{
  QString simple=str.simplifyWhiteSpace();
  int quan=0;
  int point;

  for(int i=0;i<simple.length();i++) {
    if(simple.at(i).isSpace()) {
      quan++;
      point=i;
      if(quan==word) {
	if(add_dots) {
	  return simple.left(point)+QString("...");
	}
	else {
	  return simple.left(point);
	}
      }
    }
  }
  return simple;
}


QString MLHomeDir()
{
  if(getenv("HOME")==NULL) {
    return QString("/");
  }
  return QString(getenv("HOME"));
}


QString MLTempDir()
{
#ifdef WIN32
  if(getenv("TEMP")!=NULL) {
    return QString(getenv("TEMP"));
  }
  if(getenv("TMP")!=NULL) {
    return QString(getenv("TMP"));
  }
  return QString("C:\\");
#else
  if(getenv("TMPDIR")!=NULL) {
    return QString(getenv("TMPDIR"));
  }
  return QString("/tmp");
#endif  // WIN32
}


QStringList MLKernString(const QString &str,int maxwidth)
{
  QStringList sl;
  QString residue;
  int p;

  sl.push_back(QString());
  for(int i=0;i<str.length();i++) {
    if(sl.back().length()==maxwidth) {
      if((p=sl.back().findRev(" "))<0) {
	sl.push_back(QString());
      }
      else {
	residue=sl.back().right(sl.back().length()-p-1);
	sl.back()=sl.back().left(p);
	sl.push_back(residue);
      }
    }
    sl.back()+=str.at(i);
  }
  return sl;
}
