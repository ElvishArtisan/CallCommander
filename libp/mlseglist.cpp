// mlseglist.cpp
//
//   A segment list container class for RSegClock.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlseglist.cpp,v 1.5 2011/07/27 19:40:54 cvs Exp $
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

#include <QtCore/QDateTime>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mlconf.h"
#include "mlseglist.h"
#include "mlprofile.h"

MLSegList::MLSegList()
{
  list_offset=0;
}


MLSegList::~MLSegList()
{
  for(unsigned i=0;i<list_segments.size();i++) {
    delete list_segments[i];
  }
  list_segments.clear();
}


unsigned MLSegList::size() const
{
  return list_segments.size();
}


int MLSegList::offset() const
{
  return list_offset;
}


void MLSegList::setOffset(int offset)
{
  list_offset=offset;
}


bool MLSegList::loadSegFile(QString &filename)
{
  bool ok=false;
  QString str;
  unsigned count=0;
  QString section;
  QStringList fields;
  QTime start;
  QTime end;
  MLProfile *p=new MLProfile();

  if(!p->setSource(filename)) {
    return false;
  }
  section=QString().sprintf("Segment%u",count+1);
  str=p->stringValue("Segments",section,"",&ok);
  while(ok) {
    fields=str.split(",");
    if(fields.size()>=3) {
      start=QTime::fromString(fields[0],"hh:mm:ss");
      end=QTime::fromString(fields[1],"hh:mm:ss");
      if(start.isValid()&&end.isValid()&&(start<end)) {
	for(int i=3;i<fields.size();i++) {
	  fields[2]+=(","+fields[i]);
	}
	list_segments.push_back(new MLSegment(start,end,fields[2]));
      }
    }
    count++;
    section=QString().sprintf("Segment%u",count+1);
    str=p->stringValue("Segments",section,"",&ok);
  }
  delete p;

  return true;
}


void MLSegList::addSegment(MLSegment *seg)
{
  list_segments.push_back(&(*seg));
}


void MLSegList::deleteEntry(int n)
{
  list_segments.erase(list_segments.begin()+n);
}


int MLSegList::index(const QTime &time) const
{
  for(unsigned i=0;i<list_segments.size();i++) {
    if((time>=list_segments[i]->startTime())&&
       (time<list_segments[i]->endTime())) {
      return i;
    }
  }
  return -1;
}


int MLSegList::secondsRemaining(int n,const QTime &now) const
{
  if(index(now)==n) {
    return now.secsTo(list_segments[n]->endTime());
  }
  return -1;
}


MLSegment *MLSegList::segment(int n) const
{
  return list_segments[n];
}
