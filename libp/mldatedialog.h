// mldatedialog.h
//
// A Dialog Box for using an RDatePicker widget.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mldatedialog.h,v 1.3 2010/11/22 13:11:40 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#ifndef MLDATEDIALOG_H
#define MLDATEDIALOG_H

#include <QtGui/QDialog>
#include <QtGui/QSizePolicy>
#include <QtGui/QPushButton>
#include <QtGui/QColor>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QLabel>

#include <mldatepicker.h>

class MLDateDialog : public QDialog
{
 Q_OBJECT
 public:
  MLDateDialog(int low_year,int high_year,QWidget *parent=0,
	       const char *name=0);
  ~MLDateDialog();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 public slots:
  int exec(QDate *date);

 private slots:
  void okData();
  void cancelData();

 private:
  MLDatePicker *date_picker;
  QDate *date_date;
};


#endif  // MLDATEDIALOG_H
