/*   winner_listview.h
 *
 *   The Winner ListView widget for MCallMan.
 *
 *   (C) Copyright 2002-2006,2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WINNER_LISTVIEW_H
#define WINNER_LISTVIEW_H

#include <Qt3Support/q3listview.h>
#include <QtGui/QPixmap>
#include <Qt3Support/q3popupmenu.h>
#include <Qt3Support/q3listview.h>
#include <QtGui/QMouseEvent>

#include "winner_listviewitem.h"

class WinnerListView : public Q3ListView
{
  Q_OBJECT

 public:
  WinnerListView(const QString &showcode,QWidget *parent);

 private slots:
  void aboutToShowData();
  void editActionData();
  void showDetailData();

 protected:
  void contentsMousePressEvent(QMouseEvent *e);

 private:
  Q3PopupMenu *winner_menu;
  WinnerListViewItem *winner_menu_item;
  QWidget *winner_parent;
  QString winner_showcode;
};


#endif  // WINNER_LISTVIEW_H
