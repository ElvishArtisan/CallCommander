// filter_dialog.h
//
// A dialog box for specifying the call detail filter in CallCommander.
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef FILTER_DIALOG_H
#define FILTER_DIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QDateTimeEdit>

class FilterDialog : public QDialog
{
 Q_OBJECT
 public:
  FilterDialog(QWidget *parent=0,const char *name=0);
  ~FilterDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void selectStartDateData();
  void selectEndDateData();
  void okData();
  void cancelData();

 private:
  QDateEdit *edit_startdate_edit;
  QDateEdit *edit_enddate_edit;
  QTimeEdit *edit_starttime_edit;
  QTimeEdit *edit_endtime_edit;
};


#endif  // FILTER_DIALOG_H


