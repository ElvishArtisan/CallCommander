//   client_listviewitem.cpp
//
//   The Client QListViewItem class for CallCommander.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: client_listviewitem.cpp,v 1.3 2010/11/28 19:50:04 cvs Exp $
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
//

#include "client_listviewitem.h"

ClientListViewItem::ClientListViewItem(Q3ListView *parent)
  : Q3ListViewItem(parent)
{
  item_line=-1;
}

int ClientListViewItem::line() const
{
  return item_line;
}


void ClientListViewItem::setLine(int line)
{
  item_line=line;
}
