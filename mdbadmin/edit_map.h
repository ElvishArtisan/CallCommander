// edit_map.h
//
// Edit a CallCommander CID Line Map.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_MAP_H
#define EDIT_MAP_H

#include <QtGui/QDialog>
#include <Qt3Support/q3listview.h>

class EditMap : public QDialog
{
 Q_OBJECT
 public:
  EditMap(QString showcode,QWidget *parent=0,const char *name=0);
  ~EditMap();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addData();
  void editData();
  void deleteData();
  void doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int col);
  void okData();
  void cancelData();

 private:
  Q3ListView *edit_map_list;
  QString edit_showcode;
};


#endif  // EDIT_SHOW_H


