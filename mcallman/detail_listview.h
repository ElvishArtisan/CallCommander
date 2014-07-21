/*   detail_listview.h
 *
 *   The Detail ListView widget for MCallMan.
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

#ifndef DETAIL_LISTVIEW_H
#define DETAIL_LISTVIEW_H

#include <Qt3Support/q3listview.h>
#include <QtGui/QPixmap>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3listview.h>
#include <QtGui/QMouseEvent>

class DetailListView : public Q3ListView
{
  Q_OBJECT

 public:
  DetailListView(const QString &showcode,QWidget *parent,const char *name=0);

 private slots:
  void aboutToShowData();
  void editActionData();

 protected:
  void contentsMousePressEvent(QMouseEvent *e);

 private:
  Q3PopupMenu *detail_menu;
  Q3ListViewItem *detail_menu_item;
  QWidget *detail_parent;
  QString detail_showcode;
};


#endif  // DETAIL_LISTVIEW_H
