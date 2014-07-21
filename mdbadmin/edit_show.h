// edit_show.h
//
// Edit a CallCommander Show Configuration Entry.
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

#ifndef EDIT_SHOW_H
#define EDIT_SHOW_H

#include <vector>

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <Qt3Support/q3listview.h>
#include <QtGui/QPushButton>

#include <ml_listviewitem.h>
#include <ml_timeslot.h>
#include <connection_data.h>

class EditShow : public QDialog
{
 Q_OBJECT
 public:
  EditShow(QString showcode,QWidget *parent=0,const char *name=0);
  ~EditShow();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void activeToggledData(bool on);
  void connectionTextChangedData(const QString &text);
  void selectConnectionData();
  void selectSourceData();
  void selectCidmapData();
  void doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int col);
  void addSlotData();
  void editSlotData();
  void deleteSlotData();
  void okData();
  void cancelData();

 private:
  void DisplayCallerIdSource();
  void RefreshList();
  void RefreshItem(MlListViewItem *item,MlTimeslot *slot);
  QLineEdit *edit_showcode_edit;
  QLineEdit *edit_title_edit;
  QComboBox *edit_defaultlabel_box;
  QCheckBox *edit_isactive_box;
  QLabel *edit_hostname_label;
  QLineEdit *edit_hostname_edit;
  QLabel *edit_connection_label;
  QLineEdit *edit_connection_edit;
  QPushButton *edit_connection_button;
  QLabel *edit_callerid_label;
  QLineEdit *edit_callerid_edit;
  QPushButton *edit_callerid_button;
  QPushButton *edit_cidmap_button;
  QLabel *edit_areacode_label;
  QLineEdit *edit_areacode_edit;
  QLabel *edit_killcalls_label;
  QCheckBox *edit_killcalls_box;
  QLabel *edit_active_label;
  Q3ListView *edit_showslots_list;
  QPushButton *edit_addslot_button;
  QPushButton *edit_editslot_button;
  QPushButton *edit_deleteslot_button;
  QString edit_showcode;
  int edit_callerid_hostid;
  std::vector<int> edit_deleted_slots;
};


#endif


