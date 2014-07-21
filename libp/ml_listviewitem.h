/*   ml_listviewitem.h
 *
 *   A QListViewItem class for MCallMan
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

#ifndef ML_LISTVIEWITEM_H
#define ML_LISTVIEWITEM_H

#include <Qt3Support/q3listview.h>
#include <QtCore/QString>
#include <QtCore/QStringList>

class MlListViewItem : public Q3ListViewItem
{
 public:
  MlListViewItem(Q3ListView *parent);
  ~MlListViewItem();
  int id() const;
  void setId(int id);
  QStringList *filterList();

 private:
  int item_id;
  QStringList *item_list;
};


#endif  // ML_LISTVIEWITEM_H
