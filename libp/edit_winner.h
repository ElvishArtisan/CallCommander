// edit_winner.h
//
// Edit a CallCommander Contest Winner.
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

#ifndef EDIT_WINNER_H
#define EDIT_WINNER_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>

#include <bus_driver.h>
#include <call_meta_data.h>

class EditWinner : public QDialog
{
 Q_OBJECT
 public:
  EditWinner(QWidget *parent=0);
  ~EditWinner();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(int id);
  int exec(BusDriver *driver,int line,CallMetaData *data);

 private slots:
  void okData();
  void cancelData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  void SqlSave() const;
  void BusSave() const;
  QLabel *edit_first_name_label;
  QLineEdit *edit_first_name_edit;
  QLabel *edit_last_name_label;
  QLineEdit *edit_last_name_edit;
  QLabel *edit_gender_label;
  QComboBox *edit_gender_box;
  QLabel *edit_age_label;
  QSpinBox *edit_age_spin;
  QLabel *edit_status_label;
  QComboBox *edit_status_box;
  QLabel *edit_phone_label;
  QLineEdit *edit_phone_edit;
  QLabel *edit_email_label;
  QLineEdit *edit_email_edit;
  QLabel *edit_address_label;
  QLineEdit *edit_address1_edit;
  QLineEdit *edit_address2_edit;
  QLabel *edit_city_label;
  QLineEdit *edit_city_edit;
  QLabel *edit_state_label;
  QLineEdit *edit_state_edit;
  QLabel *edit_zipcode_label;
  QLineEdit *edit_zipcode_edit;
  QLabel *edit_prize_description_label;
  QTextEdit *edit_prize_description_edit;
  QLabel *edit_remarks_label;
  QTextEdit *edit_remarks_edit;
  int edit_id;
  BusDriver *edit_driver;
  int edit_line;
  CallMetaData *edit_meta_data;
};


#endif  // EDIT_WINNER_H
