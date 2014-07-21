// edit_show_slot.h
//
// Edit a CallCommander Show Time Slot Entry.
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

#ifndef EDIT_SHOW_SLOT_H
#define EDIT_SHOW_SLOT_H

#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>
#include <Qt3Support/q3datetimeedit.h>
#include <QtGui/QPushButton>
#include <Qt3Support/q3listview.h>
#include <Qt3Support/q3buttongroup.h>

#include <ml_timeslot.h>

class EditShowSlot : public QDialog
{
 Q_OBJECT
 public:
  EditShowSlot(MlTimeslot *slot,QWidget *parent=0,const char *name=0);
  ~EditShowSlot();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void addFilterData();
  void editFilterData();
  void deleteFilterData();
  void doubleClickedData(Q3ListViewItem *item,const QPoint &pt,int col);
  void okData();
  void cancelData();

 private:
  QLabel *edit_starttime_label;
  Q3TimeEdit *edit_starttime_edit;
  QLabel *edit_endtime_label;
  Q3TimeEdit *edit_endtime_edit;
  QLabel *edit_sun_label;
  QCheckBox *edit_sun_button;
  QLabel *edit_mon_label;
  QCheckBox *edit_mon_button;
  QLabel *edit_tue_label;
  QCheckBox *edit_tue_button;
  QLabel *edit_wed_label;
  QCheckBox *edit_wed_button;
  QLabel *edit_thu_label;
  QCheckBox *edit_thu_button;
  QLabel *edit_fri_label;
  QCheckBox *edit_fri_button;
  QLabel *edit_sat_label;
  QCheckBox *edit_sat_button;
  QLabel *edit_filter_label;
  Q3ListView *edit_filter_list;
  MlTimeslot *edit_slot;
  Q3ButtonGroup *edit_filter_policy_group;
};


#endif  // EDIT_SHOW_SLOT_H

