// edit_report.h
//
// Edit a CallCommander Report.
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

#ifndef EDIT_REPORT_H
#define EDIT_REPORT_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <Qt3Support/q3textedit.h>
#include <QtGui/QSpinBox>
#include <QtGui/QCheckBox>

#include <ml.h>

class EditReport : public QDialog
{
 Q_OBJECT
 public:
 EditReport(QString showcode,QString title,
	    QWidget *parent=0,const char *name=0);
  ~EditReport();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_title_edit;
  QSpinBox *edit_startage_spin;
  QSpinBox *edit_endage_spin;
  QLineEdit *edit_city_edit;
  QLineEdit *edit_state_edit;
  QLineEdit *edit_zipcode_edit;
  QComboBox *edit_gender_box;
  QLineEdit *edit_station_edit;
  QComboBox *edit_reporttype_box;
  QLineEdit *edit_header1_edit;
  QLineEdit *edit_header2_edit;
  QString edit_showcode;
  QString edit_title;
  QCheckBox *edit_line_box[MAX_LINES];
};


#endif


