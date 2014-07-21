// list_details.h
//
// List CallCommander Call Details.
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

#ifndef LIST_DETAILS_H
#define LIST_DETAILS_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QResizeEvent>

#include "detail_listview.h"

class ListDetails : public QDialog
{
 Q_OBJECT
 public:
  ListDetails(QString showcode,QWidget *parent=0,const char *name=0);
  ~ListDetails();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void detailsRefreshData();
  void setFilterData();
  void reportsData();
  void dumpData();
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  void RefreshFilter();
  int GetBaseYear();
  DetailListView *list_details_list;
  QPushButton *list_filter_button;
  QPushButton *list_report_button;
  QPushButton *list_dump_button;
  QPushButton *list_close_button;
  QString list_showcode;
  int list_max_id;
  QDateTime list_start_datetime;
  QDateTime list_end_datetime;
  QLabel *list_filter_label;
};


#endif


