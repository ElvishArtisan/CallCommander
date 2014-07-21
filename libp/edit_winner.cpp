// edit_winner.cpp
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

#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <math.h>

#include <escapestring.h>

#include "calleriddetail.h"
#include "edit_winner.h"

EditWinner::EditWinner(QWidget *parent)
  : QDialog(parent)
{
  edit_id=-1;
  edit_driver=NULL;
  edit_line=-1;
  edit_meta_data=NULL;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);

  //
  // Name
  //
  edit_first_name_edit=new QLineEdit(this);
  edit_first_name_edit->setFont(font);
  edit_first_name_label=
    new QLabel(edit_first_name_edit,"First Name",this);
  edit_first_name_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_first_name_label->setFont(label_font);

  edit_last_name_edit=new QLineEdit(this);
  edit_last_name_edit->setFont(font);
  edit_last_name_label=
    new QLabel(edit_last_name_edit,"Last Name",this);
  edit_last_name_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_last_name_label->setFont(label_font);

  //
  // Gender
  //
  edit_gender_box=new QComboBox(this);
  edit_gender_box->insertItem(0,"Unknown","U");
  edit_gender_box->insertItem(1,"Female","F");
  edit_gender_box->insertItem(2,"Male","M");
  edit_gender_label=
    new QLabel(edit_gender_box,"Gender",this);
  edit_gender_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_gender_label->setFont(label_font);

  //
  // Age
  //
  edit_age_spin=new QSpinBox(this);
  edit_age_spin->setRange(0,120);
  edit_age_spin->setSpecialValueText(tr("Unknown"));
  edit_age_label=new QLabel(edit_age_spin,"Age",this);
  edit_age_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_age_label->setFont(label_font);

  //
  // Status
  //
  edit_status_box=new QComboBox(this);
  edit_status_box->insertItem(0,"Complete");
  edit_status_box->insertItem(1,"Pending");
  edit_status_box->insertItem(2,"Cancelled");
  edit_status_label=new QLabel(edit_status_box,"Status",this);
  edit_status_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_status_label->setFont(label_font);

  //
  // Address
  //
  edit_address1_edit=new QLineEdit(this);
  edit_address1_edit->setFont(font);
  edit_address2_edit=new QLineEdit(this);
  edit_address2_edit->setFont(font);
  edit_address_label=new QLabel(edit_address1_edit,"Address",this);
  edit_address_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_address_label->setFont(label_font);

  //
  // City
  //
  edit_city_edit=new QLineEdit(this);
  edit_city_edit->setFont(font);
  edit_city_label=new QLabel(edit_city_edit,"City",this);
  edit_city_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_city_label->setFont(label_font);

  //
  // State
  //
  edit_state_edit=new QLineEdit(this);
  edit_state_edit->setFont(font);
  edit_state_label=new QLabel(edit_state_edit,"State",this);
  edit_state_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_state_label->setFont(label_font);

  //
  // Zipcode
  //
  edit_zipcode_edit=new QLineEdit(this);
  edit_zipcode_edit->setFont(font);
  edit_zipcode_label=new QLabel(edit_zipcode_edit,"Zipcode",this);
  edit_zipcode_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_zipcode_label->setFont(label_font);

  //
  // Contact Phone
  //
  edit_phone_edit=new QLineEdit(this);
  edit_phone_edit->setFont(font);
  edit_phone_label=new QLabel(edit_phone_edit,"Contact Phone Number",this);
  edit_phone_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_phone_label->setFont(label_font);

  //
  // E-Mail
  //
  edit_email_edit=new QLineEdit(this);
  edit_email_edit->setFont(font);
  edit_email_label=new QLabel(edit_email_edit,"E-Mail Address",this);
  edit_email_label->setAlignment(Qt::AlignLeft|Qt::AlignBottom);
  edit_email_label->setFont(label_font);

  //
  // Prize Description
  //
  edit_prize_description_edit=new QTextEdit(this);
  edit_prize_description_edit->setFont(font);
  edit_prize_description_label=
    new QLabel(edit_prize_description_edit,"Prize Description",this);
  edit_prize_description_label->setFont(label_font);

  //
  // Remarks
  //
  edit_remarks_edit=new QTextEdit(this);
  edit_remarks_edit->setFont(font);
  edit_remarks_label=new QLabel(edit_remarks_edit,"Remarks",this);
  edit_remarks_label->setFont(label_font);

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


EditWinner::~EditWinner()
{
}


QSize EditWinner::sizeHint() const
{
  return QSize(380,455);
} 


QSizePolicy EditWinner::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditWinner::resizeEvent(QResizeEvent *e)
{
  edit_first_name_label->setGeometry(15,10,110,20);
  edit_first_name_edit->setGeometry(10,30,size().width()/2-10,20);

  edit_last_name_label->setGeometry(size().width()/2+15,10,110,20);
  edit_last_name_edit->
    setGeometry(size().width()/2+10,30,size().width()/2-20,20);

  edit_gender_label->setGeometry(15,52,65,20);
  edit_gender_box->setGeometry(10,75,90,20);

  edit_age_label->setGeometry(115,52,35,20);
  edit_age_spin->setGeometry(110,74,90,22);

  edit_status_label->setGeometry(235,52,90,20);
  edit_status_box->setGeometry(210,75,100,20);

  edit_address_label->setGeometry(15,94,size().width()-20,20);
  edit_address1_edit->setGeometry(10,114,size().width()-20,20);
  edit_address2_edit->setGeometry(10,136,size().width()-20,20);

  edit_city_label->setGeometry(15,158,size().width()/2-25,20);
  edit_city_edit->setGeometry(10,178,size().width()/2-20,20);

  edit_state_label->
    setGeometry(size().width()/2+5,158,size().width()/4-20,20);
  edit_state_edit->setGeometry(size().width()/2,178,size().width()/4-10,20);

  edit_zipcode_label->
    setGeometry(size().width()*3/4+5,158,size().width()/4-20,20);
  edit_zipcode_edit->
    setGeometry(size().width()*3/4,178,size().width()/4-20,20);

  edit_phone_label->setGeometry(15,200,size().width()/2-20,20);
  edit_phone_edit->setGeometry(10,220,size().width()/2-10,20);

  edit_email_label->
    setGeometry(size().width()/2+15,200,size().width()/2-20,20);
  edit_email_edit->setGeometry(size().width()/2+10,220,size().width()/2-20,20);

  edit_prize_description_label->setGeometry(15,242,size().width()-20,20);
  edit_prize_description_edit->setGeometry(10,262,size().width()-20,50);

  edit_remarks_label->setGeometry(15,314,size().width()-20,20);
  edit_remarks_edit->setGeometry(10,334,size().width()-20,50);
}


int EditWinner::exec(int id)
{
  QString sql;
  QSqlQuery *q;

  edit_id=id;
  edit_driver=NULL;
  edit_meta_data=NULL;
  sql=QString().sprintf("select FIRST_NAME,LAST_NAME,GENDER,AGE,\
                         STATUS,ADDRESS1,ADDRESS2,CITY,STATE,\
                         ZIPCODE,PHONE,EMAIL,PRIZE_DESCRIPTION,\
                         REMARKS from WINNERS where ID=%d",
			edit_id);
  q=new QSqlQuery(sql);
  if(q->first()) {
    edit_first_name_edit->setText(q->value(0).toString());
    edit_last_name_edit->setText(q->value(1).toString());
    if(q->value(2).toString()=="U") {
      edit_gender_box->setCurrentIndex(0);
    }
    if(q->value(2).toString()=="F") {
      edit_gender_box->setCurrentIndex(1);
    }
    if(q->value(2).toString()=="M") {
      edit_gender_box->setCurrentIndex(2);
    }
    edit_age_spin->setValue(q->value(3).toInt());
    edit_status_box->setCurrentItem(q->value(4).toInt());
    edit_address1_edit->setText(q->value(5).toString());
    edit_address2_edit->setText(q->value(6).toString());
    edit_city_edit->setText(q->value(7).toString());
    edit_state_edit->setText(q->value(8).toString());
    edit_zipcode_edit->setText(q->value(9).toString());
    edit_phone_edit->setText(q->value(10).toString());
    edit_email_edit->setText(q->value(11).toString());
    edit_prize_description_edit->setText(q->value(12).toString());
    edit_remarks_edit->setText(q->value(13).toString());
  }
  delete q;
  setCaption("Call Manager - Update Winner");

  return QDialog::exec();
}


int EditWinner::exec(BusDriver *driver,int line,CallMetaData *data)
{
  QStringList fields;

  edit_id=-1;
  edit_driver=driver;
  edit_line=line;
  edit_meta_data=data;

  if(!data->element(BusDriver::NameElement).toString().isEmpty()) {
    fields=data->element(BusDriver::NameElement).toString().split(" ");
    QString first="";
    for(int i=0;i<(fields.size()-1);i++) {
      first+=(fields[i]+" ");
    }
    edit_first_name_edit->setText(first.simplified());
    edit_last_name_edit->setText(fields[fields.size()-1].simplified());
  }
  for(int i=0;i<edit_gender_box->count();i++) {
    if(edit_gender_box->text(i).left(1)==
       data->element(BusDriver::GenderElement).toString()) {
      edit_gender_box->setCurrentIndex(i);
    }
  }
  edit_age_spin->setValue(data->element(BusDriver::AgeElement).toInt());
  edit_status_box->setCurrentItem(1);
  edit_address1_edit->setText("");
  edit_address2_edit->setText("");
  edit_city_edit->setText(data->element(BusDriver::CityElement).toString());
  edit_state_edit->setText(data->element(BusDriver::StateElement).toString());
  edit_zipcode_edit->setText(data->element(BusDriver::ZipElement).toString());
  edit_phone_edit->setText(CallerIdDetail::formattedNumber(data->element(BusDriver::PhoneElement).toString()));
  edit_email_edit->setText("");
  edit_prize_description_edit->setText("");
  edit_remarks_edit->setText("");
  setCaption("Call Screener - Update Winner");

  return QDialog::exec();
}


void EditWinner::okData()
{
  if(edit_last_name_edit->text().isEmpty()) {
    QMessageBox::warning(this,tr("Call Manager"),
			 tr("You must provide a Last Name!"));
    return;
  }
  if(edit_id>0) {
    SqlSave();
  }
  if(edit_driver!=NULL) {
    BusSave();
  }
  done(0);
}


void EditWinner::cancelData()
{
  done(-1);
}


void EditWinner::SqlSave() const
{
  QString sql;
  QSqlQuery *q;

  sql=QString().sprintf("update WINNERS set FIRST_NAME=\"%s\",\
                         LAST_NAME=\"%s\",\
                         GENDER=\"%s\",\
                         AGE=%d,\
                         STATUS=%d,\
                         ADDRESS1=\"%s\",\
                         ADDRESS2=\"%s\",\
                         CITY=\"%s\",\
                         STATE=\"%s\",\
                         ZIPCODE=\"%s\",\
                         PHONE=\"%s\",\
                         EMAIL=\"%s\",\
                         PRIZE_DESCRIPTION=\"%s\",\
                         REMARKS=\"%s\" \
                         where ID=%d",
			(const char *)
			EscapeString(edit_first_name_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_last_name_edit->text()).toAscii(),
			(const char *)
			edit_gender_box->currentText().left(1),
			edit_age_spin->value(),
			edit_status_box->currentIndex(),
			(const char *)
			EscapeString(edit_address1_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_address2_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_city_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_state_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_zipcode_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_phone_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_email_edit->text()).toAscii(),
			(const char *)
			EscapeString(edit_prize_description_edit->text()).
			toAscii(),
			(const char *)
			EscapeString(edit_remarks_edit->text()).toAscii(),
			edit_id);
			q=new QSqlQuery(sql);
  delete q;
}


void EditWinner::BusSave() const
{
  edit_driver->sendString(BusDriver::FirstNameAwardElement,edit_line,
			  edit_first_name_edit->text());
  edit_driver->sendString(BusDriver::LastNameAwardElement,edit_line,
			  edit_last_name_edit->text());
  edit_driver->sendString(BusDriver::GenderAwardElement,edit_line,
	       edit_gender_box->currentText().left(1));
  edit_driver->sendString(BusDriver::Addr1AwardElement,edit_line,
			  edit_address1_edit->text());
  edit_driver->sendString(BusDriver::Addr2AwardElement,edit_line,
			  edit_address2_edit->text());
  edit_driver->sendString(BusDriver::CityAwardElement,edit_line,
			  edit_city_edit->text());
  edit_driver->sendString(BusDriver::StateAwardElement,edit_line,
			  edit_state_edit->text());
  edit_driver->sendString(BusDriver::ZipAwardElement,edit_line,
			  edit_zipcode_edit->text());
  edit_driver->sendString(BusDriver::PhoneAwardElement,edit_line,
			  edit_phone_edit->text());
  edit_driver->sendString(BusDriver::EmailAwardElement,edit_line,
			  edit_email_edit->text());
  edit_driver->sendString(BusDriver::PrizeAwardElement,edit_line,
			  edit_prize_description_edit->text());
  edit_driver->sendString(BusDriver::RemarksAwardElement,edit_line,
			  edit_remarks_edit->text());
  edit_driver->sendString(BusDriver::NumberAwardElement,edit_line,
			  edit_meta_data->element(BusDriver::PhoneElement).
			  toString());
  edit_driver->sendString(BusDriver::DetailIdAwardElement,edit_line,
			  edit_meta_data->element(BusDriver::DetailIdElement).
			  toString());
  edit_driver->sendString(BusDriver::UsernameAwardElement,edit_line,
			  edit_driver->userName());
  edit_driver->sendString(BusDriver::AgeAwardElement,edit_line,
			  QString().sprintf("%d",edit_age_spin->value()));
  edit_driver->sendString(BusDriver::CommitAwardElement,edit_line,"1");
}

