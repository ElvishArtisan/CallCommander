// filter_dialog.cpp
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

#include <QtGui/QPushButton>
#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include <mldatedialog.h>

#include "globals.h"
#include "filter_dialog.h"

FilterDialog::FilterDialog(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  setCaption("Call Manager - Call Filter");

  //
  // Generate Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // The Date Range
  //
  edit_startdate_edit=new QDateEdit(this);
  edit_startdate_edit->setGeometry(205,10,100,22);
  QPushButton *button=new QPushButton(this);
  button->setGeometry(310,10,22,22);
  button->setText("...");
  connect(button,SIGNAL(clicked()),this,SLOT(selectStartDateData()));
  QLabel *label=new QLabel(edit_startdate_edit,"Show calls on dates from",
			   this,"edit_startdate_label");
  label->setGeometry(10,10,190,22);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_enddate_edit=new QDateEdit(this);
  edit_enddate_edit->setGeometry(362,10,100,22);
  button=new QPushButton(this);
  button->setGeometry(467,10,22,22);
  button->setText("...");
  connect(button,SIGNAL(clicked()),this,SLOT(selectEndDateData()));
  label=new QLabel(edit_enddate_edit,"to",this,"edit_enddate_label");
  label->setGeometry(337,10,20,22);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignCenter);

  //
  // The Time Range
  //
  edit_starttime_edit=new QTimeEdit(this);
  edit_starttime_edit->setGeometry(205,35,100,22);
  label=new QLabel(edit_starttime_edit,"and between the times of",
		   this,"edit_starttime_label");
  label->setGeometry(10,35,190,22);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_endtime_edit=new QTimeEdit(this);
  edit_endtime_edit->setGeometry(345,35,100,22);
  label=new QLabel(edit_endtime_edit,"and",this,"edit_endtime_label");
  label->setGeometry(310,35,30,22);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignCenter);

  //
  // Load Data
  //
  edit_startdate_edit->setDate(start_datetime.date());
  edit_enddate_edit->setDate(end_datetime.date());
  edit_starttime_edit->setTime(start_datetime.time());
  edit_endtime_edit->setTime(end_datetime.time());

  //
  //  OK Button
  //
  button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


FilterDialog::~FilterDialog()
{
}


QSize FilterDialog::sizeHint() const
{
  return QSize(504,125);
}


QSizePolicy FilterDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void FilterDialog::selectStartDateData()
{
  MLDateDialog *dialog=new MLDateDialog(base_year,QDate::currentDate().year(),
					this,"dialog");
  QDate date=edit_startdate_edit->date();
  if(dialog->exec(&date)==0) {
    edit_startdate_edit->setDate(date);
  }
  delete dialog;
}


void FilterDialog::selectEndDateData()
{
  MLDateDialog *dialog=new MLDateDialog(base_year,QDate::currentDate().year(),
					this,"dialog");
  QDate date=edit_enddate_edit->date();
  if(dialog->exec(&date)==0) {
    edit_enddate_edit->setDate(date);
  }
  delete dialog;
}


void FilterDialog::okData()
{
  if(edit_startdate_edit->date()>edit_enddate_edit->date()) {
    QMessageBox::warning(this,"Date Error",
			 "The Start Date is later than the End Date!");
    return;
  }
  if(edit_starttime_edit->time()>edit_endtime_edit->time()) {
    QMessageBox::warning(this,"Time Error",
			 "The Start Time is later than the End Time!");
    return;
  }
  start_datetime.setDate(edit_startdate_edit->date());
  start_datetime.setTime(edit_starttime_edit->time());
  end_datetime.setDate(edit_enddate_edit->date());
  end_datetime.setTime(edit_endtime_edit->time());

  done(0);
}


void FilterDialog::cancelData()
{
  done(-1);
}
