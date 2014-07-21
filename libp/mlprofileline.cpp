// mlprofileline.cpp
//
// A container class for profile lines.
//
// (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlprofileline.cpp,v 1.4 2011/07/20 17:12:52 cvs Exp $
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

#include "mlprofileline.h"

MLProfileLine::MLProfileLine()
{
  clear();
}


QString MLProfileLine::tag() const
{
  return line_tag;
}


void MLProfileLine::setTag(QString tag)
{
  line_tag=tag;
}


QString MLProfileLine::value() const
{
  return line_value;
}


void MLProfileLine::setValue(QString value)
{
  line_value=value;
}


void MLProfileLine::clear()
{
  line_tag="";
  line_value="";
}
