// edit_report.cpp
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
#include <QtGui/QMessageBox>
#include <QtSql/QSqlQuery>

#include <math.h>

#include "edit_report.h"

EditReport::EditReport(QString showcode,QString title,
		       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_showcode=showcode;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Manager - Edit Report");

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
  edit_title_edit=new QLineEdit(this,"edit_title_edit");
  edit_title_edit->setGeometry(100,10,sizeHint().width()-110,20);
  edit_title_edit->setFont(font);
  edit_title_edit->setReadOnly(true);
  QLabel *label=
    new QLabel(edit_title_edit,"Title:",this,"edit_title_label");
  label->setGeometry(10,10,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Age
  //
  edit_startage_spin=new QSpinBox(this,"edit_startage_spin");
  edit_startage_spin->setGeometry(100,32,40,20);
  edit_startage_spin->setRange(0,120);
  label=new QLabel(edit_startage_spin,"Age:",this,"edit_age_label");
  label->setGeometry(10,32,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  edit_endage_spin=new QSpinBox(this,"edit_endage_spin");
  edit_endage_spin->setGeometry(215,32,40,20);
  edit_endage_spin->setRange(0,120);
  label=new QLabel(edit_endage_spin,"through",this,"edit_age_label");
  label->setGeometry(145,32,65,20);
  label->setAlignment(Qt::AlignCenter);
  label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this,"edit_city_edit");
  edit_city_edit->setGeometry(100,54,120,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setMaxLength(20);
  label=new QLabel(edit_city_edit,"City:",this,"edit_city_label");
  label->setGeometry(10,54,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // State
  //
  edit_state_edit=new QLineEdit(this,"edit_state_edit");
  edit_state_edit->setGeometry(100,76,30,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setMaxLength(2);
  label=new QLabel(edit_state_edit,"State:",
			      this,"edit_state_label");
  label->setGeometry(10,76,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Zipcode
  //
  edit_zipcode_edit=new QLineEdit(this,"edit_zipcode_edit");
  edit_zipcode_edit->setGeometry(100,98,80,20);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_edit->setMaxLength(10);
  label=new QLabel(edit_zipcode_edit,"Zip Code:",
				this,"edit_zipcode_label");
  label->setGeometry(10,98,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Gender
  //
  edit_gender_box=new QComboBox(this,"edit_gender_box");
  edit_gender_box->setGeometry(100,120,120,22);
  edit_gender_box->setFont(font);
  label=new QLabel(edit_gender_box,"Gender:",this,"edit_gender_label");
  label->setGeometry(10,120,85,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_gender_box->insertItem("Female");
  edit_gender_box->insertItem("Male");
  edit_gender_box->insertItem("All");

  //
  // Station
  //
  edit_station_edit=new QLineEdit(this,"edit_station_edit");
  edit_station_edit->setGeometry(100,142,70,20);
  edit_station_edit->setFont(font);
  edit_station_edit->setMaxLength(10);
  label=new QLabel(edit_station_edit,"Station:",
				this,"edit_station_label");
  label->setGeometry(10,142,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Lines
  //
  label=new QLabel(edit_gender_box,"Use Lines:",this,"edit_lines_label");
  label->setGeometry(10,164,85,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  for(int i=0;i<(MAX_LINES/3);i++) {
    for(int j=0;j<3;j++) {
      
      edit_line_box[i*3+j]=new QCheckBox(this);
      edit_line_box[i*3+j]->setGeometry(125+65*i,167+22*j,15,15);
      label=
	new QLabel(edit_line_box[i*3+j],QString().sprintf("%d",i*3+j+1),this);
      label->setGeometry(100+65*i,164+22*j,20,22);
      label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    }
  }

  //
  // Report Type
  //
  edit_reporttype_box=new QComboBox(this,"edit_reporttype_box");
  edit_reporttype_box->setGeometry(100,238,sizeHint().width()-110,22);
  edit_reporttype_box->setFont(font);
  label=new QLabel(edit_reporttype_box,"Type:",this,"edit_reporttype_label");
  label->setGeometry(10,238,85,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_reporttype_box->insertItem("Age & Gender Report");
  edit_reporttype_box->insertItem("Source Numbers Report");

  //
  // Header 1
  //
  edit_header1_edit=new QLineEdit(this,"edit_header1_edit");
  edit_header1_edit->setGeometry(100,260,sizeHint().width()-110,20);
  edit_header1_edit->setFont(font);
  label=new QLabel(edit_header1_edit,"Header1:",this,"edit_header1_label");
  label->setGeometry(10,260,85,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Header 2
  //
  edit_header2_edit=new QLineEdit(this,"edit_header2_edit");
  edit_header2_edit->setGeometry(100,282,sizeHint().width()-110,20);
  edit_header2_edit->setFont(font);
  label=new QLabel(edit_header2_edit,"Header2:",this,"edit_header2_label");
  label->setGeometry(10,282,85,20);
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

  //
  // Load Data
  //
  QString sql=QString().sprintf("select START_AGE,END_AGE,\
                                 CITY,STATE,ZIPCODE,GENDER,\
                                 TYPE,HEADER1,HEADER2,\
                                 INCLUDE_LINE1,INCLUDE_LINE2,INCLUDE_LINE3,\
                                 INCLUDE_LINE4,INCLUDE_LINE5,INCLUDE_LINE6,\
                                 INCLUDE_LINE7,INCLUDE_LINE8,INCLUDE_LINE9,\
                                 INCLUDE_LINE10,INCLUDE_LINE11,INCLUDE_LINE12,\
                                 STATION from REPORTS where \
                                 (SHOW_CODE=\"%s\")&&(TITLE=\"%s\")",
				(const char *)edit_showcode,
				(const char *)title);
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_title_edit->setText(title);
    edit_startage_spin->setValue(q->value(0).toInt());
    edit_endage_spin->setValue(q->value(1).toInt());
    edit_city_edit->setText(q->value(2).toString());
    edit_state_edit->setText(q->value(3).toString());
    edit_zipcode_edit->setText(q->value(4).toString());
    if(q->value(5).isNull()) {
      edit_gender_box->setCurrentItem(2);
    }
    else {
      if(q->value(5).toString()=="M") {
	edit_gender_box->setCurrentItem(1);
      }
      else {
	edit_gender_box->setCurrentItem(0);
      }
    }
    edit_station_edit->setText(q->value(21).toString());
    edit_reporttype_box->setCurrentItem(q->value(6).toInt());
    edit_header1_edit->setText(q->value(7).toString());
    edit_header2_edit->setText(q->value(8).toString());
    for(int i=0;i<MAX_LINES;i++) {
      if(q->value(9+i).toString()=="Y") {
	edit_line_box[i]->setChecked(true);
      }
    }
  }
  delete q;
}


EditReport::~EditReport()
{
}


QSize EditReport::sizeHint() const
{
  return QSize(380,372);
} 


QSizePolicy EditReport::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditReport::okData()
{
  if(edit_startage_spin->value()>edit_endage_spin->value()) {
    QMessageBox::warning(this,"Invalid Ages",
			 "The Start Age is greater than the End Age!");
    return;
  }

  QString gender;

  switch(edit_gender_box->currentItem()) {
      case 0:
	gender="\"F\"";
	break;

      case 1:
	gender="\"M\"";
	break;

      default:
	gender="NULL";
	break;
  }

  QString sql=QString().sprintf("update REPORTS set\
                                 START_AGE=%d,\
                                 END_AGE=%d,\
                                 CITY=\"%s\",\
                                 STATE=\"%s\",\
                                 ZIPCODE=\"%s\",\
                                 GENDER=%s,\
                                 STATION=\"%s\",\
                                 TYPE=%d,\
                                 HEADER1=\"%s\",\
                                 HEADER2=\"%s\",",
				edit_startage_spin->value(),
				edit_endage_spin->value(),
				(const char *)edit_city_edit->text(),
				(const char *)edit_state_edit->text(),
				(const char *)edit_zipcode_edit->text(),
				(const char *)gender,
				(const char *)edit_station_edit->text(),
				edit_reporttype_box->currentItem(),
				(const char *)edit_header1_edit->text(),
				(const char *)edit_header2_edit->text());
  for(int i=0;i<MAX_LINES;i++) {
    if(edit_line_box[i]->isChecked()) {
      sql+=QString().sprintf("INCLUDE_LINE%d=\"Y\",",i+1);
    }
    else {
      sql+=QString().sprintf("INCLUDE_LINE%d=\"N\",",i+1);
    }
  }
  sql=sql.left(sql.length()-1);
  sql+=QString().sprintf(" where (SHOW_CODE=\"%s\")&&(TITLE=\"%s\")",
			 (const char *)edit_showcode,
			 (const char *)edit_title_edit->text());

  QSqlQuery *q=new QSqlQuery(sql);
  delete q;

  done(0);
}


void EditReport::cancelData()
{
  done(-1);
}
