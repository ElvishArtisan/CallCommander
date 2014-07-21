// mcallman.h
//
// The Call Manager Utility for CallCommander.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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


#ifndef MCALLMAN_H
#define MCALLMAN_H

#include <QtGui/QWidget>
#include <QtGui/QSizePolicy>
#include <QtSql/QSqlDatabase>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPixmap>
#include <QtGui/QCloseEvent>

class MainWidget : public QWidget
{
 Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0);
  ~MainWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int c);
  void openData();
  void closeData();

 protected:
  void closeEvent(QCloseEvent *e);

 private:
  void RefreshList();
  Q3ListView *man_shows_list;
  QSqlDatabase man_database;
  QPixmap *man_callcommander_map;
};


#endif 
