// callers.cpp
//
// Caller dialog for Call Commander.
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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
#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3buttongroup.h>

#include <math.h>

#include <mlconf.h>
#include <edit_winner.h>

#include "callers.h"
#include "comment.h"


Callers::Callers(bool extra,bool cid_active,int line,CallMetaData *data,
		 BusDriver *driver,QWidget *parent)
  : QDialog(parent)
{
  call_driver=driver;
  call_line=line;
  call_data=data;

  if(data->element(BusDriver::PhoneElement).toString().isEmpty()) {
    setCaption("Screen Caller");
  }
  else {
    setCaption(QString().sprintf("Screen Caller - 1-(%s)-%s-%s #%d",
		       (const char *)data->
		       element(BusDriver::PhoneElement).toString().left(3),
		       (const char *)data->
		       element(BusDriver::PhoneElement).toString().mid(3,3),
		       (const char *)data->
		       element(BusDriver::PhoneElement).toString().right(4),
		       call_data->
		       element(BusDriver::CounterElement).toInt()));
  }

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",10,QFont::Normal);
  font.setPixelSize(10);
  QFont notice_font=QFont("Helvetica",10,QFont::Bold);
  notice_font.setPixelSize(10);
  QFont button_font=QFont("Helvetica",14,QFont::Bold);
  button_font.setPixelSize(14);

  if(!extra) {
    //
    // OK Button
    //
    QPushButton *ok_button=new QPushButton(this,"ok_button");
    ok_button->setGeometry(305,10,100,55);
    ok_button->setFont(button_font);
    ok_button->setText("&OK");
    ok_button->setDefault(true);
    connect(ok_button,SIGNAL(clicked()),this,SLOT(okData()));

    //
    // Cancel Button
    //
    QPushButton *cancel_button=new QPushButton(this,"cancel_button");
    cancel_button->setGeometry(305,70,100,55);
    cancel_button->setFont(button_font);
    cancel_button->setText("&Cancel");
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

    //
    // Award Button
    //
    QPushButton *award_button=new QPushButton(this,"award_button");
    award_button->setGeometry(305,190,100,55);
    award_button->setFont(button_font);
    award_button->setText("&Award\nPrize");
    award_button->setEnabled(cid_active);
    connect(award_button,SIGNAL(clicked()),this,SLOT(awardData()));
  }
  else {
    //
    // Screen Button
    //
    QPushButton *screen_button=new QPushButton(this,"screen_button");
    screen_button->setGeometry(305,10,100,55);
    screen_button->setFont(button_font);
    screen_button->setText("&Screen");
    screen_button->setDefault(true);
    connect(screen_button,SIGNAL(clicked()),this,SLOT(screenData()));

    //
    // Hold Button
    //
    QPushButton *hold_button=new QPushButton(this,"hold_button");
    hold_button->setGeometry(305,70,100,55);
    hold_button->setFont(button_font);
    hold_button->setText("&Hold");
    connect(hold_button,SIGNAL(clicked()),this,SLOT(holdData()));

    //
    // Cancel Button
    //
    QPushButton *cancel_button=new QPushButton(this,"cancel_button");
    cancel_button->setGeometry(305,130,100,55);
    cancel_button->setFont(button_font);
    cancel_button->setText("&Cancel");
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));

    //
    // Award Button
    //
    QPushButton *award_button=new QPushButton(this,"award_button");
    award_button->setGeometry(305,250,100,55);
    award_button->setFont(button_font);
    award_button->setText("&Award\nPrize");
    award_button->setEnabled(cid_active);
    connect(award_button,SIGNAL(clicked()),this,SLOT(awardData()));
  }

  //
  // Caller Name
  //
  call_name=new QLineEdit(this,"call_name");
  call_name->setFont(font);
  call_name->setGeometry(70,10,200,19);
  call_name->setMaxLength(16);
  if(call_data->element(BusDriver::NameElement).toString()!=" ") {
    call_name->
      setText(call_data->element(BusDriver::NameElement).toString());
  }
  call_name->setFocus();
  QLabel *call_name_label=new QLabel(call_name,"&Name:",this,
				       "call_name_label");
  call_name_label->setFont(font);
  call_name_label->setGeometry(10,12,55,19);
  call_name_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Caller City
  //
  call_city=new QLineEdit(this,"call_city");
  call_city->setFont(font);
  call_city->setGeometry(70,31,125,19);
  call_city->setMaxLength(20);
  if(call_data->element(BusDriver::CityElement).toString()!=" ") {
    call_city->
      setText(call_data->element(BusDriver::CityElement).toString());
  }
  QLabel *call_city_label=new QLabel(call_city,"Cit&y:",this,
				       "call_city_label");
  call_city_label->setFont(font);
  call_city_label->setGeometry(10,33,55,19);
  call_city_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Caller State
  //
  call_state=new QLineEdit(this,"call_state");
  call_state->setFont(font);
  call_state->setGeometry(239,31,30,19);
  call_state->setMaxLength(2);
  if(call_data->element(BusDriver::StateElement).toString()!=" ") {
    call_state->
      setText(call_data->element(BusDriver::StateElement).toString());
  }
  QLabel *call_state_label=new QLabel(call_state,"S&tate:",this,
				       "call_state_label");
  call_state_label->setFont(font);
  call_state_label->setGeometry(200,33,35,19);
  call_state_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Caller Comment
  //
  call_comment=new Comment(this);
  call_comment->setFont(font);
  call_comment->setGeometry(70,52,199,40);
  if(call_data->element(BusDriver::CommentElement).toString()!=" ") {
    call_comment->setText(call_data->
			  element(BusDriver::CommentElement).toString());
  }
  call_comment->setTextFormat(Qt::PlainText);
  QLabel *call_comment_label=new QLabel(call_comment,"Co&mment:",this,
				       "call_comment_label");
  call_comment_label->setFont(font);
  call_comment_label->setGeometry(10,54,60,19);
  call_comment_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Cellphone Flag
  //
  Q3ButtonGroup *cell_group=new Q3ButtonGroup(1,Qt::Horizontal,
					    "C&ell Phone",this,"cell_group");
  cell_group->setFont(font);
  cell_group->setGeometry(70,96,95,90);
  call_cell_yes=new QRadioButton(cell_group,"call_cell_yes");
  call_cell_yes->setText(tr("Yes"));
  QRadioButton *cell_no=new QRadioButton(cell_group,"call_cell_no");
  cell_no->setText(tr("No"));
  if(data->element(BusDriver::CellphoneElement).toBool()) {
    call_cell_yes->setChecked(true);
  }
  else {
    cell_no->setChecked(true);
  }

  //
  // Caller Gender
  //
  Q3ButtonGroup *gender_group=new Q3ButtonGroup(1,Qt::Horizontal,
					    "Gen&der",this,"gender_group");
  gender_group->setFont(font);
  gender_group->setGeometry(175,96,95,90);
  call_gender_male=new QRadioButton(gender_group,"call_gender_male");
  call_gender_male->setText("Male");
  if(call_data->
     element(BusDriver::GenderElement).toString().contains("M",false)) {
    call_gender_male->setChecked(true);
  }
  call_gender_female=new QRadioButton(gender_group,
				      "call_gender_female");
  call_gender_female->setText("Female");
  if(call_data->
     element(BusDriver::GenderElement).toString().contains("F",false)) {
    call_gender_female->setChecked(true);
  }

  //
  // Caller Station
  //
  call_station=new QLineEdit(this,"call_station");
  call_station->setGeometry(70,193,50,19);
  call_station->setMaxLength(4);
  call_station->setFont(font);
  if(call_data->element(BusDriver::StationElement).toString()!=" ") {
    call_station->setText(call_data->
			  element(BusDriver::StationElement).toString());
  }
  QLabel *call_station_label=new QLabel(call_station,"Stat&ion:",this,
				       "call_station_label");
  call_station_label->setFont(font);
  call_station_label->setGeometry(20,195,45,19);
  call_station_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Caller Age
  //
  call_age=new QLineEdit(this,"call_age");
  call_age->setFont(font);
  call_age->setGeometry(70,214,25,19);
  call_age->setMaxLength(3);
  if(data->element(BusDriver::AgeElement).toInt()>-1) {
    call_age->setText(QString().sprintf("%d",data->
		 element(BusDriver::AgeElement).toInt()));
  }
  QLabel *call_age_label=new QLabel(call_age,"&Age:",this,
				       "call_age_label");
  call_age_label->setFont(font);
  call_age_label->setGeometry(30,216,35,19);
  call_age_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Caller Zip
  //
  call_zip=new QLineEdit(this,"call_zip");
  call_zip->setFont(font);
  call_zip->setGeometry(70,235,50,19);
  call_zip->setMaxLength(10);
  if(call_data->element(BusDriver::ZipElement).toString()!=" ") {
    call_zip->setText(call_data->element(BusDriver::ZipElement).toString());
  }
  QLabel *call_zip_label=new QLabel(call_zip,"&ZIP:",this,
				       "call_zip_label");
  call_zip_label->setFont(font);
  call_zip_label->setGeometry(40,237,25,19);
  call_zip_label->setAlignment(Qt::AlignRight|Qt::TextShowMnemonic);

  //
  // Quality Rating Buttons
  //
  Q3ButtonGroup *quality_group=
    new Q3ButtonGroup(1,Qt::Horizontal,
		     "Call &Quality",this,"quality_group");
  quality_group->setFont(font);
  quality_group->setGeometry(150,193,117,170);

  call_quality_excellent=
    new QRadioButton(quality_group,"call_quality_excellent");
  call_quality_excellent->setText("Excellent");
  if(data->element(BusDriver::QualityElement).toInt()==5) {
    call_quality_excellent->setChecked(true);
  }
  call_quality_good=new QRadioButton(quality_group,"call_quality_good");
  call_quality_good->setText("Good");
  if(data->element(BusDriver::QualityElement).toInt()==4) {
    call_quality_good->setChecked(true);
  }
  call_quality_fair=new QRadioButton(quality_group,"call_quality_fair");
  call_quality_fair->setText("Fair");
  if(data->element(BusDriver::QualityElement).toInt()==3) {
    call_quality_fair->setChecked(true);
  }
  call_quality_poor=new QRadioButton(quality_group,"call_quality_poor");
  call_quality_poor->setText("Poor");
  if(data->element(BusDriver::QualityElement).toInt()==2) {
    call_quality_poor->setChecked(true);
  }
  call_quality_verypoor=
    new QRadioButton(quality_group,"call_quality_verypoor");
  call_quality_verypoor->setText("Very Poor");
  if(data->element(BusDriver::QualityElement).toInt()==1) {
    call_quality_verypoor->setChecked(true);
  }

  //
  // Last Call Data
  //
  if(call_data->element(BusDriver::LastCallDatetimeElement).toInt()>0) {
    QDateTime current=QDateTime(QDate::currentDate(),QTime::currentTime());
    QDateTime datetime;
    datetime.setTime_t(call_data->element(BusDriver::LastCallDatetimeElement).
		       toUInt());

    QLabel *label=
      new QLabel("Last Called on "+DateTimeString(datetime,current),this);
    label->setGeometry(15,375,sizeHint().width()-20,20);
    label->setFont(notice_font);
  }

  //
  // Last OnAir Data
  //
  if(call_data->element(BusDriver::LastOnAirDatetimeElement).toInt()>0) {
    QDateTime current=QDateTime(QDate::currentDate(),QTime::currentTime());
    QDateTime datetime;
    datetime.setTime_t(call_data->element(BusDriver::LastOnAirDatetimeElement).
		       toUInt());

    QLabel *label=
      new QLabel("Last On Air on "+DateTimeString(datetime,current),this);
    label->setGeometry(15,390,sizeHint().width()-20,20);
    label->setFont(notice_font);
  }

  //
  // Last Award Data
  //
  if(call_data->element(BusDriver::LastAwardDatetimeElement).toInt()>0) {
    QDateTime current=QDateTime(QDate::currentDate(),QTime::currentTime());
    QDateTime datetime;
    datetime.setTime_t(call_data->element(BusDriver::LastAwardDatetimeElement).
		       toUInt());

    QLabel *label=
      new QLabel("Last prize awarded on "+DateTimeString(datetime,current),
		 this);
    label->setGeometry(15,405,sizeHint().width()-20,20);
    label->setFont(notice_font);
  }
}


QSize Callers::sizeHint() const
{
  int h=370;

  if(call_data->element(BusDriver::LastCallDatetimeElement)!=0) {
    h+=20;
  }
  if(call_data->element(BusDriver::LastOnAirDatetimeElement)!=0) {
    h+=20;
  }
  if(call_data->element(BusDriver::LastAwardDatetimeElement)!=0) {
    h+=20;
  }
  return QSize(420,h);
} 


QSizePolicy Callers::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void Callers::cancelData()
{
  done(1);
}


void Callers::okData()
{
  WriteCallData(call_data);
  done(0);
}


void Callers::screenData()
{
  WriteCallData(call_data);
  done(2);
}


void Callers::holdData()
{
  WriteCallData(call_data);
  done(3);
}


void Callers::awardData()
{
  CallMetaData data;
  WriteCallData(&data);
  data.setElement(BusDriver::PhoneElement,
		   call_data->element(BusDriver::PhoneElement).toString());
  data.setElement(BusDriver::DetailIdElement,
		   call_data->element(BusDriver::DetailIdElement).toUInt());
  EditWinner *d=new EditWinner(this);
  d->exec(call_driver,call_line,&data);
  delete d;
}


void Callers::paintEvent(QPaintEvent *paintevent)
{
  QPainter *p=new QPainter(this);
  p->drawLine(10,370,410,370);
  p->end();
}


//
// Encode empty fields as " ".  Ugly, but needed to deal with Telos' 125 byte
// packet size limitation in the TWOx12.
//
void Callers::WriteCallData(CallMetaData *data)
{
  int quality=0;

  data->setElement(BusDriver::NameElement,call_name->text());
  data->setElement(BusDriver::AgeElement,call_age->text());
  data->
    setElement(BusDriver::CellphoneElement,call_cell_yes->isChecked());
  if(call_quality_verypoor->isChecked()) {
    quality=1;
  }
  if(call_quality_poor->isChecked()) {
    quality=2;
  }
  if(call_quality_fair->isChecked()) {
    quality=3;
  }
  if(call_quality_good->isChecked()) {
    quality=4;
  }
  if(call_quality_excellent->isChecked()) {
    quality=5;
  }
  data->setElement(BusDriver::QualityElement,quality);
  data->setElement(BusDriver::CityElement,call_city->text());
  data->setElement(BusDriver::StateElement,call_state->text());
  data->setElement(BusDriver::ZipElement,call_zip->text());
  data->setElement(BusDriver::StationElement,call_station->text());
  if(call_gender_male->isChecked()) {
    data->setElement(BusDriver::GenderElement,"M");
  }
  else {
    if(call_gender_female->isChecked()) {
      data->setElement(BusDriver::GenderElement,"F");
    }
    else {
      data->setElement(BusDriver::GenderElement,"");
    }
  }
  data->setElement(BusDriver::CommentElement,call_comment->text());
}


QString Callers::DateTimeString(const QDateTime &datetime,const QDateTime &now)
{
  unsigned diff=datetime.secsTo(now);
  QString ret=QString().sprintf("%s at %s",(const char *)datetime.
				toString("MM/dd/yyyy"),
				(const char *)datetime.
				toString("hh:mm:ss"));
  if(diff>86400) {
    if((diff/86400)==1) {
      ret+=" - 1 day ago.";
    }
    else {
      ret+=QString().sprintf(" - %u days ago.",diff/86400);
    }
  }
  else {
    if(diff>3600) {
      if((diff/3600)==1) {
	ret+=" - 1 hour ago.";
      }
      else {
	ret+=QString().sprintf(" - %u hours ago.",diff/3600);
      }
    }
    else {
      if(diff>60) {
	if((diff/60)==1) {
	  ret+=" - 1 minute ago.";
	}
	else {
	  ret+=QString().sprintf(" - %u minutes ago.",diff/60);
	}
      }
      else {
	ret+=QString().sprintf(" - %u seconds ago.",diff);
      }
    }
  }

  return ret;
}
