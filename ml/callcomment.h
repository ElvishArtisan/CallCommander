// callcomment.h
//
// Text display widget for call comments. 
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CALLCOMMENT_H
#define CALLCOMMENT_H

#include <QtGui/QTextEdit>

class CallComment : public QTextEdit
{
  Q_OBJECT
 public:
  CallComment(QWidget *parent=0);

 protected:
  void mouseDoubleClickEvent(QMouseEvent *e); 
};


#endif  // CALLCOMMENT_H
