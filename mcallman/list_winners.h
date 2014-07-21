// list_winners.h
//
// List CallCommander Contest Winners
//
//   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef LIST_WINNERS_H
#define LIST_WINNERS_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtCore/QDateTime>
#include <QtGui/QLabel>
#include <QtGui/QResizeEvent>
#include <QtGui/QCheckBox>
#include <Qt3Support/Q3ListView>

#include "winner_listviewitem.h"
#include "winner_listview.h"

#include <edit_winner.h>

class ListWinners : public QDialog
{
 Q_OBJECT
 public:
  ListWinners(QString showcode,QWidget *parent=0,const char *name=0);
  ~ListWinners();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void filterStateEnabledData(int state);
  void filterStateChangedData(int state);
  void setFilterData();
  void addData();
  void editData();
  void doubleClickedData(Q3ListViewItem *item);
  void reportsData();
  void dumpData();
  void closeData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void RefreshList();
  void RefreshItem(WinnerListViewItem *item);
  void RefreshFilter();
  QString GenerateWhere();
  int GetBaseYear();
  WinnerListView *list_winners_list;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_filter_button;
  QPushButton *list_report_button;
  QPushButton *list_dump_button;
  QPushButton *list_close_button;
  QString list_showcode;
  int list_max_id;
  QDateTime list_start_datetime;
  QDateTime list_end_datetime;
  QLabel *list_filter_label;
  EditWinner *list_edit_winner;
  QPixmap *list_checkmark_map;
  QPixmap *list_ex_map;
  QLabel *list_filterbox_label;
  QCheckBox *list_filterbox_box;
  QLabel *list_pending_label;
  QCheckBox *list_pending_box;
  QLabel *list_complete_label;
  QCheckBox *list_complete_box;
};


#endif  // LIST_WINNERS_H
