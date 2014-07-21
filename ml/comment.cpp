// comment.cpp
//
// Text Editor Widget for the Mithlond Project.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
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

#include "comment.h"

Comment::Comment(QWidget *parent)
  : QTextEdit(parent)
{
}


void Comment::keyPressEvent(QKeyEvent *e)
{
  if(e->ascii()==9) {   // TAB Key
    e->ignore();
    return;
  }
  if(e->ascii()==13) {   // RETURN Key
    e->ignore();
    return;
  }
  QTextEdit::keyPressEvent(e);
}
