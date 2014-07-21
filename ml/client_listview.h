//   client_listview.h
//
//   The Client ListView widget for CallCommander.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: client_listview.h,v 1.3 2010/11/28 19:50:04 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
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

#ifndef CLIENT_LISTVIEW_H
#define CLIENT_LISTVIEW_H

#include <Qt3Support/q3listview.h>
#include <QtGui/QPixmap>
#include <Qt3Support/q3popupmenu.h>
#include <QtGui/QMouseEvent>

#include "client_listviewitem.h"

class ClientListView : public Q3ListView
{
  Q_OBJECT

 public:
  ClientListView(QWidget *parent,const char *name=0);

 signals:
  void sizeChanged(int size);

 private slots:
  void aboutToShowData();
  void killClientData();

 protected:
  void contentsMousePressEvent(QMouseEvent *e);
  void contentsMouseDoubleClickEvent(QMouseEvent *e);

 private:
  QWidget *client_parent;
  Q3PopupMenu *client_menu;
  ClientListViewItem *client_menu_item;
};


#endif  // CLIENT_LISTVIEW_H
