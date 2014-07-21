// mlconf.h
//
// The header file for the mlconf package
//
//   (C) Copyright 1996-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlconf.h,v 1.6 2010/11/22 13:11:40 cvs Exp $
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

#ifndef MLCONF_H
#define MLCONF_H

#define MAX_RETRIES 10
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QFont>
#include <QtNetwork/QHostAddress>
#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>
#include <QtCore/QStringList>
#include <stdlib.h>
#include <stdio.h>

/* Function Prototypes */
int GetPrivateProfileBool(const char *,const char *,const char *,bool);
int GetPrivateProfileString(const char *,const char *,const char *,char *,
			    const char *,int);
int GetPrivateProfileInt(const char *,const char *,const char *,int);
int GetPrivateProfileHex(const char *,const char *,const char *,int);
double GetPrivateProfileDouble(const char *,const char *,const char *,double);
int GetIni(const char *,const char *,const char *,char *,int);
int GetIniLine(FILE *,char *);
QString MLGetPathPart(QString path);
QString MLGetBasePart(QString path);
QString MLGetShortDate(QDate);
/**
 * Returns the name of the weekday in english regardless of the locale
 * configured.
 * @param weekday Integer value for the weekday; 1 = "Mon", 2 = "Tue", 
 * ... 7 = "Sun".  If the value is out of range 1 is defaulted to.
 **/
QString MLGetShortDayNameEN(int weekday);
QFont::Weight MLGetFontWeight(QString);
bool MLDetach();
bool MLBool(QString);
QString MLYesNo(bool);
QHostAddress MLGetHostAddr();
QString MLGetDisplay(bool strip_point=false);
bool MLDoesRowExist(QString table,QString name,QString test);
bool MLDoesRowExist(QString table,QString name,unsigned test);
QVariant MLGetSqlValue(QString table,QString name,QString test,
		      QString param,bool *valid=0);
QVariant MLGetSqlValue(QString table,QString name,unsigned test,
		      QString param,bool *valid=0);
bool MLIsSqlNull(QString table,QString name,QString test,QString param);
bool MLIsSqlNull(QString table,QString name,unsigned test,QString param);
QString MLGetTimeLength(int mseconds,bool leadzero=false,bool tenths=true);
int MLSetTimeLength(QString string);
bool MLCopy(QString srcfile,QString destfile);
#ifndef WIN32
bool MLWritePid(QString dirname,QString filename,int owner=-1,int group=-1);
void MLDeletePid(QString dirname,QString filename);
bool MLCheckPid(QString dirname,QString filename);
pid_t MLGetPid(QString pidfile);
#endif  // WIN32
QString MLGetHomeDir(bool *found=0);
QString MLTruncateAfterWord(QString str,int word,bool add_dots=false);
QString MLHomeDir();
QString MLTempDir();
QStringList MLKernString(const QString &str,int maxwidth);

#endif   // MLCONF_H
