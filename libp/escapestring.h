// escapestring.h
//
// Escape non-valid characters in a string.
//
//   (C) Copyright 2002-2005,2011 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: escapestring.h,v 1.1 2011/07/29 22:57:21 cvs Exp $
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

#include <QtCore/QString>

#ifndef ESCAPESTRING_H
#define ESCAPESTRING_H

QString EscapeString(const QString &str);
QString EscapeStringSQLColumn(const QString &str);


#endif  // ESCAPESTRING_H
