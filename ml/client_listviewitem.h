//   client_listviewitem.h
//
//   The Client QListViewItem class for CallCommander.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: client_listviewitem.h,v 1.2 2010/11/22 13:11:41 cvs Exp $
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

#ifndef CLIENT_LISTVIEWITEM_H
#define CLIENT_LISTVIEWITEM_H

#include <Qt3Support/q3listview.h>

class ClientListViewItem : public Q3ListViewItem
{
 public:
  ClientListViewItem(Q3ListView *parent);
  int line() const;
  void setLine(int line);

 private:
  int item_line;
};


#endif  // CLIENT_LISTVIEWITEM_H
