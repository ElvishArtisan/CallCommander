// show_detail.h
//
// Show a CallCommander Call Detail.
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

#ifndef SHOW_DETAIL_H
#define SHOW_DETAIL_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>

class ShowDetail : public QDialog
{
 Q_OBJECT
 public:
  ShowDetail(const QString &showcode,int detail_id,QWidget *parent=0);
  ~ShowDetail();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void closeData();

 private:
  QLineEdit *edit_datetime_edit;
  QLineEdit *edit_number_edit;
  QLineEdit *edit_route_edit;
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
  QLineEdit *edit_gender_edit;
  QLabel *edit_station_label;
  QLineEdit *edit_station_edit;
  QString edit_showcode;
  QLabel *edit_comment_label;
  QTextEdit *edit_comment_edit;
  int edit_detail_id;
  QLabel *edit_ring_label;
  QLineEdit *edit_ring_edit;
  QLabel *edit_hold_label;
  QLineEdit *edit_hold_edit;
  QLabel *edit_air_label;
  QLineEdit *edit_air_edit;
};


#endif  // SHOW_DETAIL_H
