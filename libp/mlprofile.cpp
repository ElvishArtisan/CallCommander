// mlprofile.cpp
//
// A class to read an ini formatted configuration file.
//
// (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlprofile.cpp,v 1.5 2011/07/20 17:12:52 cvs Exp $
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
//

#include <QtCore/QFile>
#include <Qt3Support/q3textstream.h>

#include "mlprofile.h"

MLProfile::MLProfile()
{
}


QString MLProfile::source() const
{
  return profile_source;
}


bool MLProfile::setSource(QString filename)
{
  QString section;
  int offset;

  profile_source=filename;
  profile_section.resize(0);
  profile_section.push_back(MLProfileSection());
  profile_section.back().setName("");
  QFile *file=new QFile(filename);
  if(!file->open(QIODevice::ReadOnly)) {
    delete file;
    return false;
  }
  Q3TextStream *text=new Q3TextStream(file);
  QString line=text->readLine().stripWhiteSpace();
  while(!line.isNull()) {
    if((line.left(1)!=";")&&(line.left(1)!="#")) {
      if((line.left(1)=="[")&&(line.right(1)=="]")) {
	section=line.mid(1,line.length()-2);
	profile_section.push_back(MLProfileSection());
	profile_section.back().setName(section);
      }
      else if(((offset=line.find('='))!=-1)) {
//      else if(((offset=line.find('='))!=-1)&&(!section.isEmpty())) {
	profile_section.back().
	  addValue(line.left(offset),
		   line.right(line.length()-offset-1).stripWhiteSpace());
      }
    }
    line=text->readLine().stripWhiteSpace();
  }
  delete text;
  delete file;
  return true;
}


bool MLProfile::setSource(std::vector<QString> *values)
{
  QString section;
  int offset;

  profile_section.resize(0);
  profile_section.push_back(MLProfileSection());
  profile_section.back().setName("");
  for(unsigned i=0;i<values->size();i++) {
    if((values->at(i).left(1)!=";")&&(values->at(i).left(1)!="#")) {
      if((values->at(i).left(1)=="[")&&(values->at(i).right(1)=="]")) {
	section=values->at(i).mid(1,values->at(i).length()-2);
	profile_section.push_back(MLProfileSection());
	profile_section.back().setName(section);
      }
      else if(((offset=values->at(i).find('='))!=-1)) {
	profile_section.back().
	  addValue(values->at(i).left(offset),
		   values->at(i).right(values->at(i).length()-offset-1).
		   stripWhiteSpace());
      }
    }
  }
  return true;
}


QString MLProfile::stringValue(QString section,QString tag,
			      QString default_str,bool *ok) const
{
  QString result;

  for(unsigned i=0;i<profile_section.size();i++) {
    if(profile_section[i].name()==section) {
      if(profile_section[i].getValue(tag,&result)) {
	if(ok!=NULL) {
	  *ok=true;
	}
	return result;
      }
      if(ok!=NULL) {
	*ok=false;
      }
      return default_str;
    }
  }
  if(ok!=NULL) {
    *ok=false;
  }
  return default_str;
}


int MLProfile::intValue(QString section,QString tag,
		       int default_value,bool *ok) const
{
  bool valid;

  int result=stringValue(section,tag).toInt(&valid,10);
  if(!valid) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=true;
  }
  return result;
}


int MLProfile::hexValue(QString section,QString tag,
		       int default_value,bool *ok) const
{
  bool valid;

  int result=stringValue(section,tag).toInt(&valid,16);
  if(!valid) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=true;
  }
  return result;
}


float MLProfile::floatValue(QString section,QString tag,
			   float default_value,bool *ok) const
{
  bool valid;

  float result=stringValue(section,tag).toDouble(&valid);
  if(!valid) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=true;
  }
  return result;
}


double MLProfile::doubleValue(QString section,QString tag,
			    double default_value,bool *ok) const
{
  bool valid;

  double result=stringValue(section,tag).toDouble(&valid);
  if(!valid) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if(ok!=NULL) {
    *ok=true;
  }
  return result;
}


bool MLProfile::boolValue(QString section,QString tag,
			 bool default_value,bool *ok) const
{
  bool valid;

  QString str=stringValue(section,tag,"",&valid).lower();
  if(!valid) {
    if(ok!=NULL) {
      *ok=false;
    }
    return default_value;
  }
  if((str=="yes")||(str=="true")||(str=="on")) {
    if(ok!=NULL) {
      *ok=true;
    }
    return true;
  }
  if((str=="no")||(str=="false")||(str=="off")) {
    if(ok!=NULL) {
      *ok=true;
    }
    return false;
  }
  if(ok!=NULL) {
    *ok=false;
  }
  return default_value;
}


void MLProfile::clear()
{
  profile_source="";
  profile_section.resize(0);
}
