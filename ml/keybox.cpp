// keybox.cpp
//
// DTMF Keypad dialog for Call Commander.
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QPainter>
#include <QtCore/QEvent>
#include <QtCore/QSignalMapper>
#include <QtGui/QFontMetrics>

#include <math.h>

#include "globals.h"
#include "redial_dialog.h"
#include "keybox.h"

Keybox::Keybox(BusDriver *driver,const QString &area_code,int line,
	       QWidget *parent,const char *name) 
  : QDialog(parent,name,true)
{
  QFont number_font;
  QFont letter_font;
  QFont edit_font;

  key_driver=driver;
  key_area_code=area_code;
  key_line=line;
  setCaption("Touch Tone Dialpad");
  QPixmap *pixmap=new QPixmap(60,60);
  QPainter *p=new QPainter();

  //
  // Generate Fonts
  //
  number_font=QFont("Helvetica",36,QFont::Bold);
  number_font.setPixelSize(36);
  letter_font=QFont("Helvetica",14,QFont::Bold);
  letter_font.setPixelSize(14);
  QFontMetrics letter_fm(letter_font);
  edit_font=QFont("Helvetica",18,QFont::Bold);
  edit_font.setPixelSize(18);

  //
  // The Button Mapper
  //
  QSignalMapper *mapper=new QSignalMapper(this,"mapper");
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(sendDigitData(int)));

  //
  // '1' Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(10,10,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(number_font);
  p->drawText(20,55,"1");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("2"));
  button->setDefault(true);
  mapper->setMapping(button,'1');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '2' Button
  //
  button=new QPushButton(this,"two_button");
  button->setGeometry(95,10,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"A");
  p->drawText(26,17,"B");
  p->drawText(41,17,"C");
  p->setFont(number_font);
  p->drawText(20,55,"2");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("2"));
  button->setDefault(true);
  mapper->setMapping(button,'2');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '3' Button
  //
  button=new QPushButton(this,"three_button");
  button->setGeometry(180,10,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"D");
  p->drawText(26,17,"E");
  p->drawText(41,17,"F");
  p->setFont(number_font);
  p->drawText(20,55,"3");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("3"));
  button->setDefault(true);
  mapper->setMapping(button,'3');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '4' Button
  //
  button=new QPushButton(this,"four_button");
  button->setGeometry(10,95,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"G");
  p->drawText(26,17,"H");
  p->drawText(41,17,"I");
  p->setFont(number_font);
  p->drawText(20,55,"4");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("4"));
  button->setDefault(true);
  mapper->setMapping(button,'4');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '5' Button
  //
  button=new QPushButton(this,"five_button");
  button->setGeometry(95,95,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"J");
  p->drawText(26,17,"K");
  p->drawText(41,17,"L");
  p->setFont(number_font);
  p->drawText(20,55,"5");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("5"));
  button->setDefault(true);
  mapper->setMapping(button,'5');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '6' Button
  //
  button=new QPushButton(this,"six_button");
  button->setGeometry(180,95,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"M");
  p->drawText(26,17,"N");
  p->drawText(41,17,"O");
  p->setFont(number_font);
  p->drawText(20,55,"6");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("6"));
  button->setDefault(true);
  mapper->setMapping(button,'6');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '7' Button
  //
  button=new QPushButton(this,"seven_button");
  button->setGeometry(10,180,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"P");
  p->drawText(26,17,"R");
  p->drawText(41,17,"S");
  p->setFont(number_font);
  p->drawText(20,55,"7");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("7"));
  button->setDefault(true);
  mapper->setMapping(button,'7');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '8' Button
  //
  button=new QPushButton(this,"eight_button");
  button->setGeometry(95,180,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"T");
  p->drawText(26,17,"U");
  p->drawText(41,17,"V");
  p->setFont(number_font);
  p->drawText(20,55,"8");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("8"));
  button->setDefault(true);
  mapper->setMapping(button,'8');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '9' Button
  //
  button=new QPushButton(this,"nine_button");
  button->setGeometry(180,180,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText(10,17,"W");
  p->drawText(26,17,"X");
  p->drawText(41,17,"Y");
  p->setFont(number_font);
  p->drawText(20,55,"9");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("9"));
  button->setDefault(true);
  mapper->setMapping(button,'9');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '*' Button
  //
  button=new QPushButton(this,"star_button");
  button->setGeometry(10,265,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(number_font);
  p->drawText(25,50,"*");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("*"));
  button->setDefault(true);
  mapper->setMapping(button,'*');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '0' Button
  //
  button=new QPushButton(this,"zero_button");
  button->setGeometry(95,265,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(letter_font);
  p->drawText((60-letter_fm.width("OPER"))/2,17,"OPER");
  p->setFont(number_font);
  p->drawText(20,55,"0");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("0"));
  button->setDefault(true);
  mapper->setMapping(button,'0');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // '#' Button
  //
  button=new QPushButton(this,"pound_button");
  button->setGeometry(180,265,80,80);
  p->begin(pixmap);
  p->fillRect(0,0,60,60,palette().color(QPalette::Button));
  p->setPen(QColor(Qt::color1));
  p->setFont(number_font);
  p->drawText(20,40,"#");
  p->end();
  button->setPixmap(*pixmap);
  button->setAccel(QKeySequence("#"));
  button->setDefault(true);
  mapper->setMapping(button,'#');
  connect(button,SIGNAL(clicked()),mapper,SLOT(map()));

  //
  // Number Display
  //
  key_number_edit=new QLineEdit(this,"key_number_edit");
  key_number_edit->setGeometry(10,355,sizeHint().width()-20,24);
  key_number_edit->setFont(edit_font);
  key_number_edit->setReadOnly(true);

  //
  // Redial Button
  //
  button=new QPushButton(this,"redial_button");
  button->setGeometry(10,390,120,80);
  button->setFont(letter_font);
  button->setText("&Redial\nLast #");
  connect(button,SIGNAL(clicked()),this,SLOT(redialData()));

  //
  // Redial List Button
  //
  button=new QPushButton(this,"redial_button");
  button->setGeometry(140,390,120,80);
  button->setFont(letter_font);
  button->setText("&Redial\nList");
  connect(button,SIGNAL(clicked()),this,SLOT(redialListData()));

  //
  // Close Button
  //
  button=new QPushButton(this,"close_button");
  button->setGeometry(10,480,250,80);
  button->setFont(number_font);
  button->setText("&Close");
  button->setDefault(true);
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));
}


QSize Keybox::sizeHint() const
{
  return QSize(270,570);
} 


QSizePolicy Keybox::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void Keybox::sendDigitData(int digit)
{
  key_number_edit->
    setText(key_number_edit->text()+QString().sprintf("%c",(char)digit));
  key_driver->sendDtmf((char)digit);
}


void Keybox::redialData()
{
  if(number_list->size()==0) {
    return;
  }
  key_number_edit->setText(number_list->number(0));
  DialNumber();
  closeData();
}


void Keybox::redialListData()
{
  QString number;
  RedialDialog *dialog=new RedialDialog(&number,this,"redial_dialog");
  if(dialog->exec()==0) {
    key_number_edit->setText(number);
    DialNumber();
    closeData();
  }
  delete dialog;
}


void Keybox::closeData()
{
  if(!key_number_edit->text().isEmpty()) {
    number_list->pushNumber(key_number_edit->text());
    SendNumber(key_number_edit->text());
  }
  done(0);
}


void Keybox::DialNumber()
{
  QString number=key_number_edit->text();
  for(int i=0;i<number.length();i++) {
    key_driver->sendDtmf(((const char *)number)[i]);
  }
}


void Keybox::SendNumber(QString pnum)
{
  if((key_line<0)||(pnum.length()<7)) {
    return;
  }
  if(pnum.length()>=10) {
    pnum=pnum.right(10);
  }
  if(pnum.length()<10) {
    pnum=key_area_code+pnum.right(7);
  }
  key_driver->sendString(BusDriver::PhoneElement,key_line,pnum);
}
