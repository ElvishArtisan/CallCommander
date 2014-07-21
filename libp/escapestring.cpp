// escapestring.cpp
//
// Escape non-valid characters in a string.
//
//   (C) Copyright 2002-2005,2011 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: escapestring.cpp,v 1.2 2011/08/02 00:33:36 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#include <vector>

#include <escapestring.h>

QString EscapeString(QString const &str)
{
  QString res;

  if(str.isEmpty()) {
    return str;
  }
  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {
	case '(':
	  res+=QString("\\\(");
	  break;

	case ')':
	  res+=QString("\\)");
	  break;

	case '{':
	  res+=QString("\\\{");
	  break;

	case '"':
	  res+=QString("\\\"");
	  break;

	case '´':
	  res+=QString("\\´");
	  break;

	case '`':
	  res+=QString("\\`");
	  break;

	case '[':
	  res+=QString("\\\[");
	  break;

	case '\'':
	  res+=QString("\\\'");
	  break;

	case '\\':
	  res+=QString("\\");
	  res+=QString("\\");
	  break;

	case '?':
	  res+=QString("\\\?");
	  break;

	case ' ':
	  res+=QString("\\ ");
	  break;

	case '&':
	  res+=QString("\\&");
	  break;

        case ';':
	  res+=QString("\\;");
	  break;

        case '<':
	  res+=QString("\\<");
	  break;

        case '>':
	  res+=QString("\\>");
	  break;

        case '|':
	  res+=QString("\\|");
	  break;

	default:
	  res+=((const char *)str)[i];
	  break;
    }
  }
  return res;
}



QString EscapeStringSQLColumn(QString const &str)
{
  QString res;

  for(int i=0;i<str.length();i++) {
    switch(((const char *)str)[i]) {
        case 0:
          res+=QString("\\0");
          break;

        case '/':
          res+=QString("_");
          break;

        case '\\':
          res+=QString("_");
          break;

        case '.':
          res+=QString("_");
          break;

	default:
	  res+=((const char *)str)[i];
	  break;
    }
  }
  return res;
}


