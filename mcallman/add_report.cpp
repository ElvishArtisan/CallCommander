// add_report.cpp
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

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QValidator>
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <math.h>

#include "add_report.h"

AddReport::AddReport(QString showcode,QString *title,
		     QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_title=title;
  add_showcode=showcode;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Manager - Add Report");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Title
  //
  add_title_edit=new QLineEdit(this,"add_title_edit");
  add_title_edit->setGeometry(100,10,sizeHint().width()-110,20);
  add_title_edit->setFont(font);
  add_title_edit->setMaxLength(64);
  QLabel *label=
    new QLabel(add_title_edit,"Report Title:",this,"add_title_label");
  label->setGeometry(10,10,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  //  OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
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


AddReport::~AddReport()
{
}


QSize AddReport::sizeHint() const
{
  return QSize(330,100);
} 


QSizePolicy AddReport::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddReport::okData()
{
  QString sql=
    QString().sprintf("select TITLE from REPORTS where (SHOW_CODE=\"%s\")&&\
                       (TITLE=\"%s\")",
		      (const char *)add_showcode,
		      (const char *)add_title_edit->text());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    QMessageBox::warning(this,"Report Exists",
			 "A report witht that title already exists!");
    delete q;
    return;
  }
  delete q;
  *add_title=add_title_edit->text();
  done(0);
}


void AddReport::cancelData()
{
  done(-1);
}
