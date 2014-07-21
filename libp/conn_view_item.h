/*   conn_view_item.h
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

#ifndef CONN_VIEW_ITEM_H
#define CONN_VIEW_ITEM_H

#include <vector>

#include <Qt3Support/q3listview.h>
#include <QtGui/QPixmap>

#include <connection_data.h>
#include <show_data.h>
#include <console_data.h>

using namespace std;

class ConnViewItem : public Q3ListViewItem
{
 public:
  ConnViewItem(Q3ListView *parent);
  ~ConnViewItem();
  ConnectionData *connectionData() const;
  void setConnectionData(ConnectionData *data);
  ShowData *showData() const;
  void setShowData(ShowData *data);
  ConsoleData *consoleData() const;
  void setConsoleData(ConsoleData *data);

 private:
  ConnectionData *item_data;
  ShowData *item_show_data;
  ConsoleData *item_console_data;
};


#endif  // CONN_VIEW_ITEM_H
