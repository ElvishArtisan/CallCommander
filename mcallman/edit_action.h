// edit_action.h
//
// Edit a CallCommander Action.
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

#ifndef EDIT_ACTION_H
#define EDIT_ACTION_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <Qt3Support/q3textedit.h>

class EditAction : public QDialog
{
 Q_OBJECT
 public:
  EditAction(QString num,QString showcode,int detail_id,
	     QWidget *parent=0);
  ~EditAction();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void labelActionActivatedData(int id);
  void routeActionActivatedData(int id);
  void copyDetailData();
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_number_edit;
  QComboBox *edit_route_box;
  QComboBox *edit_label_box;
  QLabel *edit_name_label;
  QLineEdit *edit_name_edit;
  QLabel *edit_age_label;
  QLineEdit *edit_age_edit;
  QLabel *edit_state_label;
  QLineEdit *edit_state_edit;
  QLabel *edit_city_label;
  QLineEdit *edit_city_edit;
  QLabel *edit_zipcode_label;
  QLineEdit *edit_zipcode_edit;
  QLabel *edit_gender_label;
  QComboBox *edit_gender_box;
  QString edit_showcode;
  QLabel *edit_warntext_label;
  Q3TextEdit *edit_warntext_edit;
  int edit_detail_id;
};


#endif


