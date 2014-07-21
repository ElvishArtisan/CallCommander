// show_detail.cpp
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

#include <mldconfig.h>
#include <mlconf.h>

#include "calleriddetail.h"
#include "show_detail.h"

ShowDetail::ShowDetail(const QString &showcode,int detail_id,QWidget *parent)
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

  setCaption("Call Manager - Show Call Detail");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Call Date/Time
  //
  edit_datetime_edit=new QLineEdit(this);
  edit_datetime_edit->setGeometry(120,10,160,20);
  edit_datetime_edit->setFont(font);
  edit_datetime_edit->setReadOnly(true);
  QLabel *label=new QLabel(edit_datetime_edit,"Date/Time:",this);
  label->setGeometry(10,10,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Number
  //
  edit_number_edit=new QLineEdit(this);
  edit_number_edit->setGeometry(120,32,120,20);
  edit_number_edit->setFont(font);
  edit_number_edit->setReadOnly(true);
  label=new QLabel(edit_number_edit,"Number:",this);
  label->setGeometry(10,32,105,20);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Route Action
  //
  edit_route_edit=new QLineEdit(this);
  edit_route_edit->setGeometry(120,54,100,22);
  edit_route_edit->setFont(font);
  edit_route_edit->setReadOnly(true);
  label=new QLabel(edit_route_edit,"Route Action:",this);
  label->setGeometry(10,54,105,22);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  label->setFont(label_font);

  //
  // Name
  //
  edit_name_edit=new QLineEdit(this);
  edit_name_edit->setGeometry(120,76,120,20);
  edit_name_edit->setFont(font);
  edit_name_edit->setReadOnly(true);
  edit_name_label=new QLabel(edit_name_edit,"Name:",this);
  edit_name_label->setGeometry(10,76,105,20);
  edit_name_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_name_label->setFont(label_font);

  //
  // Age
  //
  edit_age_edit=new QLineEdit(this);
  edit_age_edit->setGeometry(120,98,30,20);
  edit_age_edit->setFont(font);
  edit_age_edit->setReadOnly(true);
  edit_age_label=new QLabel(edit_age_edit,"Age:",this);
  edit_age_label->setGeometry(10,98,105,20);
  edit_age_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_age_label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setGeometry(120,120,120,20);
  edit_city_edit->setFont(font);
  edit_city_edit->setReadOnly(true);
  edit_city_label=new QLabel(edit_city_edit,"City:",this);
  edit_city_label->setGeometry(10,120,105,20);
  edit_city_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_city_label->setFont(label_font);

  //
  // State
  //
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setGeometry(120,142,30,20);
  edit_state_edit->setFont(font);
  edit_state_edit->setReadOnly(true);
  edit_state_label=new QLabel(edit_state_edit,"State:",this);
  edit_state_label->setGeometry(10,142,105,20);
  edit_state_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_state_label->setFont(label_font);

  //
  // Zipcode
  //
  edit_zipcode_edit=new QLineEdit(this);
  edit_zipcode_edit->setGeometry(120,164,80,20);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_edit->setReadOnly(true);
  edit_zipcode_label=new QLabel(edit_zipcode_edit,"Zipcode:",this);
  edit_zipcode_label->setGeometry(10,164,105,20);
  edit_zipcode_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_zipcode_label->setFont(label_font);

  //
  // Gender
  //
  edit_gender_edit=new QLineEdit(this);
  edit_gender_edit->setGeometry(120,186,80,20);
  edit_gender_edit->setFont(font);
  edit_gender_edit->setReadOnly(true);
  edit_gender_label=new QLabel(edit_gender_edit,"Gender:",this);
  edit_gender_label->setGeometry(10,186,105,20);
  edit_gender_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_gender_label->setFont(label_font);

  //
  // Station
  //
  edit_station_edit=new QLineEdit(this);
  edit_station_edit->setGeometry(120,208,80,20);
  edit_station_edit->setFont(font);
  edit_station_edit->setReadOnly(true);
  edit_station_label=new QLabel(edit_station_edit,"Station:",this);
  edit_station_label->setGeometry(10,208,105,20);
  edit_station_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_station_label->setFont(label_font);

  //
  // Comment
  //
  edit_comment_edit=new QTextEdit(this);
  edit_comment_edit->setGeometry(120,230,sizeHint().width()-130,100);
  edit_comment_edit->setFont(font);
  edit_comment_edit->setReadOnly(true);
  edit_comment_label=new QLabel(edit_comment_edit,"Comment:",this);
  edit_comment_label->setGeometry(10,230,105,22);
  edit_comment_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_comment_label->setFont(label_font);

  //
  // Ring Timer
  //
  edit_ring_edit=new QLineEdit(this);
  edit_ring_edit->setGeometry(120,332,60,20);
  edit_ring_edit->setFont(font);
  edit_ring_edit->setReadOnly(true);
  edit_ring_label=new QLabel(edit_ring_edit,"Ring Time:",this);
  edit_ring_label->setGeometry(10,332,105,20);
  edit_ring_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_ring_label->setFont(label_font);

  //
  // Hold Timer
  //
  edit_hold_edit=new QLineEdit(this);
  edit_hold_edit->setGeometry(120,354,60,20);
  edit_hold_edit->setFont(font);
  edit_hold_edit->setReadOnly(true);
  edit_hold_label=new QLabel(edit_hold_edit,"Hold Time:",this);
  edit_hold_label->setGeometry(10,354,105,20);
  edit_hold_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_hold_label->setFont(label_font);

  //
  // Air Timer
  //
  edit_air_edit=new QLineEdit(this);
  edit_air_edit->setGeometry(120,376,60,20);
  edit_air_edit->setFont(font);
  edit_air_edit->setReadOnly(true);
  edit_air_label=new QLabel(edit_air_edit,"Air Time:",this);
  edit_air_label->setGeometry(10,376,105,20);
  edit_air_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_air_label->setFont(label_font);

  //
  // Close Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("Close");
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));

  //
  // Load Data
  //
  QString sql=
    QString().sprintf("select CALL_DATE,CALL_TIME,NUMBER,ROUTE_ACTION,NAME,\
                       AGE,CITY,STATE,ZIPCODE,GENDER,STATION,COMMENT,\
                       RING_TIMER,HOLD_TIMER,AIR_TIMER \
                       from DETAILS_%s where ID=%d",
		      (const char *)edit_showcode.toAscii(),
		      edit_detail_id);
  QSqlQuery *q=new QSqlQuery(sql);
  if(q->first()) {
    edit_datetime_edit->
      setText(q->value(0).toDate().toString("MM/dd/yyyy")+" - "+
	      q->value(1).toTime().toString("hh:mm:ss"));
    edit_number_edit->
      setText(CallerIdDetail::formattedNumber(q->value(2).toString()));
    switch((MldConfig::RouteAction)q->value(3).toInt()) {
    case MldConfig::Accept:
      edit_route_edit->setText(tr("Accept"));
      break;

    case MldConfig::Drop:
      edit_route_edit->setText(tr("Drop"));
      break;

    case MldConfig::Warn:
      edit_route_edit->setText(tr("Warn"));
      break;
    }
    edit_name_edit->setText(q->value(4).toString());
    if(q->value(5).toInt()!=0) {
      edit_age_edit->setText(q->value(5).toString());
    }
    edit_city_edit->setText(q->value(6).toString());
    edit_state_edit->setText(q->value(7).toString());
    edit_zipcode_edit->setText(q->value(8).toString());
    if(q->value(9).isNull()) {
      edit_gender_edit->setText(tr("Unknown"));
    }
    else {
      if(q->value(9).toString()=="M") {
	edit_gender_edit->setText(tr("Male"));;
      }
      if(q->value(9).toString()=="F") {
	edit_gender_edit->setText(tr("Female"));;
      }
      if(q->value(9).toString()=="U") {
	edit_gender_edit->setText(tr("Unknown"));;
      }
    }
    edit_station_edit->setText(q->value(10).toString());
    edit_comment_edit->setText(q->value(11).toString());
    edit_ring_edit->setText(MLGetTimeLength(q->value(12).toInt(),true,false));
    edit_hold_edit->setText(MLGetTimeLength(q->value(13).toInt(),true,false));
    edit_air_edit->setText(MLGetTimeLength(q->value(14).toInt(),true,false));
  }
  delete q;
}


ShowDetail::~ShowDetail()
{
}


QSize ShowDetail::sizeHint() const
{
  return QSize(380,464);
} 


QSizePolicy ShowDetail::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ShowDetail::closeData()
{
  done(0);
}
