/*   conn_view_item.cpp
 *
 *   A QListViewItem class for Holding CallCommander Connection Configs
 *
 *   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "conn_view_item.h"

ConnViewItem::ConnViewItem(Q3ListView *parent)
  : Q3ListViewItem(parent)
{
  item_data=NULL;
  item_show_data=NULL;
  item_console_data=NULL;
}


ConnViewItem::~ConnViewItem()
{
  if(item_data!=NULL) {
    delete item_data;
  }
  if(item_show_data!=NULL) {
    delete item_show_data;
  }
  if(item_console_data!=NULL) {
    delete item_console_data;
  }
}


ConnectionData *ConnViewItem::connectionData() const
{
  return item_data;
}


void ConnViewItem::setConnectionData(ConnectionData *data)
{
  item_data=data;
}


ShowData *ConnViewItem::showData() const
{
  return item_show_data;
}


void ConnViewItem::setShowData(ShowData *data)
{
  item_show_data=data;
}


ConsoleData *ConnViewItem::consoleData() const
{
  return item_console_data;
}


void ConnViewItem::setConsoleData(ConsoleData *data)
{
  item_console_data=data;
}
