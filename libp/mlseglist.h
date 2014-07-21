// mlseglist.cpp
//
//   A segment list container class for MLSegClock.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlseglist.h,v 1.4 2011/07/27 19:40:54 cvs Exp $
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

#ifndef MLSEGLIST_H
#define MLSEGLIST_H

#include <vector>

#include <QtCore/QDateTime>

#include <mlsegment.h>

struct seglist {
  QTime start;
  QTime finish;
  char label[255];
};

class MLSegList
{
  public:
    MLSegList();
    ~MLSegList();
    unsigned size() const;
    int offset() const;
    void setOffset(int offset);
    bool loadSegFile(QString &filename);
    void addSegment(MLSegment *seg);
    void deleteEntry(int n);
    int index(const QTime &time) const;
    int secondsRemaining(int n,const QTime &now) const;
    MLSegment *segment(int n) const;
  
  private:
    std::vector<MLSegment *> list_segments;
    int list_offset;
};

#endif  // MLSEGLIST_H
