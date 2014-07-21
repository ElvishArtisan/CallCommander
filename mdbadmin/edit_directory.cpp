// edit_directory.cpp
//
// Edit a CallCommander Phone Number Directory Entry.
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
#include <QtSql/QSqlQuery>
#include <math.h>

#include "edit_directory.h"

EditDirectory::EditDirectory(QString num,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_number=num;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Directory");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Number
  //
  edit_number_edit=new QLineEdit(this,"edit_number_edit");
  edit_number_edit->setGeometry(100,10,120,20);
  edit_number_edit->setFont(font);
  edit_number_edit->setReadOnly(true);
  QLabel *label=
    new QLabel(edit_number_edit,"Number:",this,"edit_number_label");
  label->setGeometry(10,10,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this,"edit_city_edit");
  edit_city_edit->setGeometry(100,32,200,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setMaxLength(20);
  label=new QLabel(edit_city_edit,"City:",this,"edit_city_label");
  label->setGeometry(10,32,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // State
  //
  edit_state_edit=new QLineEdit(this,"edit_state_edit");
  edit_state_edit->setGeometry(100,54,30,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setMaxLength(2);
  label=new QLabel(edit_state_edit,"State:",this,"edit_state_label");
  label->setGeometry(10,54,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Cellphone
  //
  edit_cellphone_box=new QComboBox(this,"edit_cellphone_box");
  edit_cellphone_box->setGeometry(100,76,60,22);
  edit_cellphone_box->setFont(font);
  label=
    new QLabel(edit_cellphone_box,"Cellphone:",this,"edit_cellphone_label");
  label->setGeometry(10,76,85,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_cellphone_box->insertItem("Yes");
  edit_cellphone_box->insertItem("No");

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

  //
  // Load Data
  //
  QSqlQuery *q=new QSqlQuery(QString().sprintf("select CITY,STATE,CELLPHONE\
                                                from DIRECTORY where\
                                                NUMBER=\"%s\"",
					       (const char *)edit_number));
  if(q->first()) {
    edit_number_edit->setText(edit_number);
    edit_city_edit->setText(q->value(0).toString());
    edit_state_edit->setText(q->value(1).toString());
    if(q->value(2).toString().lower()=="y") {
      edit_cellphone_box->setCurrentItem(0);
    }
    else {
      edit_cellphone_box->setCurrentItem(1);
    }
  }
  delete q;
}


EditDirectory::~EditDirectory()
{
}


QSize EditDirectory::sizeHint() const
{
  return QSize(330,175);
} 


QSizePolicy EditDirectory::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditDirectory::okData()
{
  QString cell;

  if(edit_cellphone_box->currentItem()==0) {
    cell="Y";
  }
  else {
    cell="N";
  }
  QString sql=QString().sprintf("update DIRECTORY set\
                                 CITY=\"%s\",\
                                 STATE=\"%s\",\
                                 CELLPHONE=\"%s\",\
                                 CUSTOM=\"Y\"\
                                 where NUMBER=\"%s\"",
				(const char *)edit_city_edit->text(),
				(const char *)edit_state_edit->text(),
				(const char *)cell,
				(const char *)edit_number);
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;
  done(0);
}


void EditDirectory::cancelData()
{
  done(-1);
}
