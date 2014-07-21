// list_directory.h
//
// List the CallCommander Phone Number Directory.
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

#ifndef LIST_DIRECTORY_H
#define LIST_DIRECTORY_H

#include <QtGui/QDialog>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPushButton>
#include <Qt3Support/q3progressdialog.h>
#include <QtGui/QResizeEvent>

class ListDirectory : public QDialog
{
 Q_OBJECT
 public:
  ListDirectory(QWidget *parent=0,const char *name=0);
  ~ListDirectory();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addData();
  void editData();
  void deleteData();
  void importData();
  void purgeData();
  void doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int c);
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  void UpdateItem(Q3ListViewItem *item);
  bool ImportNanpaData(QString filename);
  Q3ListView *list_directory_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
  QPushButton *list_import_button;
  QPushButton *list_purge_button;
  QPushButton *list_close_button;
  Q3ProgressDialog *list_progress_bar;
  QWidget *list_parent;
};


#endif


