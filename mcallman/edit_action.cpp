// edit_action.cpp
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

#include "calleriddetail.h"
#include "edit_action.h"

EditAction::EditAction(QString num,QString showcode,int detail_id,
		       QWidget *parent)
  : QDialog(parent)
{
  edit_showcode=showcode;
  edit_detail_id=detail_id;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Manager - Edit Action");

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
  edit_number_edit->setGeometry(120,10,120,20);
  edit_number_edit->setFont(font);
  edit_number_edit->setReadOnly(true);
  QLabel *label=
    new QLabel(edit_number_edit,"Number:",this,"edit_number_label");
  label->setGeometry(10,10,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Route Action
  //
  edit_route_box=new QComboBox(this,"edit_route_box");
  edit_route_box->setGeometry(120,32,100,22);
  edit_route_box->setFont(font);
  label=
    new QLabel(edit_route_box,"Route Action:",this,"edit_route_label");
  label->setGeometry(10,32,105,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_route_box->insertItem("Accept");
  edit_route_box->insertItem("Warn");
  edit_route_box->insertItem("Drop");
  connect(edit_route_box,SIGNAL(activated(int)),
	  this,SLOT(routeActionActivatedData(int)));

  //
  // Label Action
  //
  edit_label_box=new QComboBox(this,"edit_label_box");
  edit_label_box->setGeometry(120,54,sizeHint().width()-130,22);
  edit_label_box->setFont(font);
  label=
    new QLabel(edit_label_box,"Label Action:",this,"edit_label_label");
  label->setGeometry(10,54,105,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);
  edit_label_box->insertItem("Set Blank");
  edit_label_box->insertItem("Use Data from First Call");
  edit_label_box->insertItem("Use Data from Most Recent Call");
  edit_label_box->insertItem("Use Specified Data");
  connect(edit_label_box,SIGNAL(activated(int)),
	  this,SLOT(labelActionActivatedData(int)));

  //
  // Name
  //
  edit_name_edit=new QLineEdit(this,"edit_name_edit");
  edit_name_edit->setGeometry(120,76,120,20);
  edit_name_edit->setFont(font);
  edit_name_edit->setMaxLength(16);
  edit_name_label=new QLabel(edit_name_edit,"Name:",this,"edit_name_label");
  edit_name_label->setGeometry(10,76,105,20);
  edit_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_name_label->setFont(label_font);

  //
  // Age
  //
  edit_age_edit=new QLineEdit(this,"edit_age_edit");
  edit_age_edit->setGeometry(120,98,30,20);
  edit_age_edit->setFont(font);
  edit_age_edit->setMaxLength(3);
  edit_age_label=new QLabel(edit_age_edit,"Age:",this,"edit_age_label");
  edit_age_label->setGeometry(10,98,105,20);
  edit_age_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_age_label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this,"edit_city_edit");
  edit_city_edit->setGeometry(120,120,120,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setMaxLength(20);
  edit_city_label=new QLabel(edit_city_edit,"City:",this,"edit_city_label");
  edit_city_label->setGeometry(10,120,105,20);
  edit_city_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_city_label->setFont(label_font);

  //
  // State
  //
  edit_state_edit=new QLineEdit(this,"edit_state_edit");
  edit_state_edit->setGeometry(120,142,30,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setMaxLength(2);
  edit_state_label=new QLabel(edit_state_edit,"State:",
			      this,"edit_state_label");
  edit_state_label->setGeometry(10,142,105,20);
  edit_state_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_state_label->setFont(label_font);

  //
  // Zipcode
  //
  edit_zipcode_edit=new QLineEdit(this,"edit_zipcode_edit");
  edit_zipcode_edit->setGeometry(120,164,80,20);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_edit->setMaxLength(10);
  edit_zipcode_label=new QLabel(edit_zipcode_edit,"Zipcode:",
				this,"edit_zipcode_label");
  edit_zipcode_label->setGeometry(10,164,105,20);
  edit_zipcode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_zipcode_label->setFont(label_font);

  //
  // Gender
  //
  edit_gender_box=new QComboBox(this,"edit_gender_box");
  edit_gender_box->setGeometry(120,186,120,22);
  edit_gender_box->setFont(font);
  edit_gender_label=
    new QLabel(edit_label_box,"Gender:",this,"edit_gender_label");
  edit_gender_label->setGeometry(10,186,105,22);
  edit_gender_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_gender_label->setFont(label_font);
  edit_gender_box->insertItem("Female");
  edit_gender_box->insertItem("Male");
  edit_gender_box->insertItem("Not Specified");

  //
  // Warning Text
  //
  edit_warntext_edit=new Q3TextEdit(this,"edit_warntext_edit");
  edit_warntext_edit->setGeometry(10,232,sizeHint().width()-20,80);
  edit_warntext_edit->setFont(font);
  edit_warntext_label=
    new QLabel(edit_label_box,"Warning Text",this,"edit_warntext_label");
  edit_warntext_label->setGeometry(15,208,sizeHint().width()-30,22);
  edit_warntext_label->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
  edit_warntext_label->setFont(label_font);

  //
  //  Apply Detail Button
  //
  QPushButton *button=new QPushButton(this,"copy_detail_button");
  button->setGeometry(10,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("Copy Detail\nData");
  connect(button,SIGNAL(clicked()),this,SLOT(copyDetailData()));
  if(detail_id<0) {
    button->hide();
  }

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

  //
  // Load Data
  //
  QString sql=
    QString().sprintf("select ROUTE_ACTION,LABEL_ACTION,NAME,AGE,\
                       CITY,STATE,ZIPCODE,GENDER,WARNING_TEXT \
                       from ACTIONS_%s where NUMBER=\"%s\"",
		      (const char *)edit_showcode.toAscii(),
		      (const char *)CallerIdDetail::simplifiedNumber(num).
    toAscii());
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_number_edit->setText(CallerIdDetail::formattedNumber(num));
    edit_route_box->setCurrentItem(q->value(0).toInt());
    edit_label_box->setCurrentItem(q->value(1).toInt());
    edit_name_edit->setText(q->value(2).toString());
    if(q->value(3).toInt()!=0) {
      edit_age_edit->setText(q->value(3).toString());
    }
    edit_city_edit->setText(q->value(4).toString());
    edit_state_edit->setText(q->value(5).toString());
    edit_zipcode_edit->setText(q->value(6).toString());
    if(q->value(7).isNull()) {
      edit_gender_box->setCurrentItem(2);
    }
    else {
      if(q->value(7).toString()=="M") {
	edit_gender_box->setCurrentItem(1);
      }
      else {
	edit_gender_box->setCurrentItem(0);
      }
    }
    if(q->value(0).toInt()==1) {
      edit_warntext_edit->setText(q->value(8).toString());
    }
  }
  routeActionActivatedData(q->value(0).toInt());
  labelActionActivatedData(q->value(1).toInt());
  delete q;
}


EditAction::~EditAction()
{
}


QSize EditAction::sizeHint() const
{
  return QSize(380,387);
} 


QSizePolicy EditAction::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditAction::labelActionActivatedData(int id)
{
  if(id==3) {
    edit_name_edit->setEnabled(true);
    edit_age_edit->setEnabled(true);
    edit_state_edit->setEnabled(true);
    edit_city_edit->setEnabled(true);
    edit_zipcode_edit->setEnabled(true);
    edit_gender_box->setEnabled(true);
    edit_name_label->setEnabled(true);
    edit_age_label->setEnabled(true);
    edit_state_label->setEnabled(true);
    edit_city_label->setEnabled(true);
    edit_zipcode_label->setEnabled(true);
    edit_gender_label->setEnabled(true);
    return;
  }
  edit_name_edit->setDisabled(true);
  edit_age_edit->setDisabled(true);
  edit_state_edit->setDisabled(true);
  edit_city_edit->setDisabled(true);
  edit_zipcode_edit->setDisabled(true);
  edit_gender_box->setDisabled(true);
  edit_name_label->setDisabled(true);
  edit_age_label->setDisabled(true);
  edit_state_label->setDisabled(true);
  edit_city_label->setDisabled(true);
  edit_zipcode_label->setDisabled(true);
  edit_gender_label->setDisabled(true);
}


void EditAction::routeActionActivatedData(int id)
{
  if(id==1) {
    edit_warntext_edit->setEnabled(true);
    edit_warntext_label->setEnabled(true);
    return;
  }
  edit_warntext_edit->setDisabled(true);
  edit_warntext_label->setDisabled(true);
}


void EditAction::copyDetailData()
{
  QString sql;
  QSqlQuery *q;
  sql=QString().sprintf("select NAME,AGE,CITY,STATE,ZIPCODE,GENDER \
                         from DETAILS_%s where ID=%d",
			(const char *)edit_showcode,edit_detail_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_name_edit->setText(q->value(0).toString());
    if(q->value(1).toInt()>0) {
      edit_age_edit->setText(q->value(1).toString());
    }
    else {
      edit_age_edit->setText("");
    }
    edit_city_edit->setText(q->value(2).toString());
    edit_state_edit->setText(q->value(3).toString());
    edit_zipcode_edit->setText(q->value(4).toString());
    if(q->value(5).toString()=="F") {
      edit_gender_box->setCurrentItem(0);
    }
    else {
      if(q->value(5).toString()=="M") {
	edit_gender_box->setCurrentItem(1);
      }
      else {
	edit_gender_box->setCurrentItem(2);
      }
    }
    edit_label_box->setCurrentItem(3);
    labelActionActivatedData(3);
  }
  delete q;
}


void EditAction::okData()
{
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

  QString sql=QString().sprintf("update ACTIONS_%s set\
                                 ROUTE_ACTION=%d,\
                                 LABEL_ACTION=%d,\
                                 NAME=\"%s\",\
                                 AGE=%d,\
                                 CITY=\"%s\",\
                                 STATE=\"%s\",\
                                 ZIPCODE=\"%s\",\
                                 GENDER=%s,\
                                 WARNING_TEXT=\"%s\"\
                                 where NUMBER=\"%s\"",
				(const char *)edit_showcode,
				edit_route_box->currentItem(),
				edit_label_box->currentItem(),
				(const char *)edit_name_edit->text(),
				edit_age_edit->text().toInt(),
				(const char *)edit_city_edit->text(),
				(const char *)edit_state_edit->text(),
				(const char *)edit_zipcode_edit->text(),
				(const char *)gender,
				(const char *)edit_warntext_edit->text(),
				(const char *)CallerIdDetail::
				simplifiedNumber(edit_number_edit->text()));
  QSqlQuery *q=new QSqlQuery(sql);
  delete q;
  done(0);
}


void EditAction::cancelData()
{
  done(-1);
}
