// edit_ttysettings.cpp
//
// Edit TTY Parameter Settings.
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

#include <stdlib.h>

#include <QtGui/QLabel>
#include <QtGui/QMessageBox>
#include <QtGui/QPushButton>

#include "edit_ttysettings.h"

EditTtySettings::EditTtySettings(TtySettings *settings,
				 QWidget *parent,const char *name)
  : QDialog(parent,name,false)
{
  edit_settings=settings;

  //
  // HACK: Disable the Broken Custom SuSE Dialogs
  //
#ifndef WIN32
  setenv("QT_NO_KDE_INTEGRATION","1",1);
#endif  // WIN32

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption(tr("Serial Parameters"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);

  //
  // Connection Baud Rate
  //
  edit_speed_box=new QComboBox(this,"edit_speed_box");
  edit_speed_box->setGeometry(120,10,80,20);
  QLabel *label=new QLabel(edit_speed_box,tr("Baud Rate:"),
			   this,"edit_speed_label");
  label->setGeometry(10,10,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  label=new QLabel(edit_speed_box,tr("bits/sec"),this,"edit_speed_unit");
  label->setGeometry(205,10,50,20);
  label->setFont(font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Word Length
  //
  edit_wordlength_box=new QComboBox(this,"edit_wordlength_box");
  edit_wordlength_box->setGeometry(120,32,50,20);
  label=new QLabel(edit_wordlength_box,tr("Data Bits:"),
		   this,"edit_wordlength_label");
  label->setGeometry(10,32,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Connection Stop Bits
  //
  edit_stopbits_box=new QComboBox(this,"edit_stopbits_box");
  edit_stopbits_box->setGeometry(120,54,50,20);
  label=new QLabel(edit_stopbits_box,tr("Stop Bits:"),
		   this,"edit_stopbits_label");
  label->setGeometry(10,54,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Connection Parity
  //
  edit_parity_box=new QComboBox(this,"edit_parity_box");
  edit_parity_box->setGeometry(120,76,80,20);
  label=new QLabel(edit_parity_box,tr("Parity:"),
		   this,"edit_parity_label");
  label->setGeometry(10,76,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Ok Button
  //
  QPushButton *button=new QPushButton(tr("OK"),this,"edit_ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));


  //
  // Load Device Values
  //
  edit_speed_box->insertItem("50");
  edit_speed_box->insertItem("75");
  edit_speed_box->insertItem("110");
  edit_speed_box->insertItem("134");
  edit_speed_box->insertItem("150");
  edit_speed_box->insertItem("200");
  edit_speed_box->insertItem("300");
  edit_speed_box->insertItem("600");
  edit_speed_box->insertItem("1200");
  edit_speed_box->insertItem("1800");
  edit_speed_box->insertItem("2400");
  edit_speed_box->insertItem("4800");
  edit_speed_box->insertItem("9600");
  edit_speed_box->insertItem("19200");
  edit_speed_box->insertItem("38400");
  edit_speed_box->insertItem("57600");
  edit_speed_box->insertItem("115200");
  edit_speed_box->insertItem("230400");
  for(int i=0;i<edit_speed_box->count();i++) {
    if(edit_speed_box->text(i)==QString().sprintf("%d",settings->speed())) {
      edit_speed_box->setCurrentItem(i);
    }
  }

  edit_wordlength_box->insertItem("5");
  edit_wordlength_box->insertItem("6");
  edit_wordlength_box->insertItem("7");
  edit_wordlength_box->insertItem("8");
  if((settings->wordLength()>=5)&&(settings->wordLength()<=8)) {
    edit_wordlength_box->setCurrentItem(settings->wordLength()-5);
  }

  edit_stopbits_box->insertItem("1");
  edit_stopbits_box->insertItem("2");
  if((settings->stopBits()>=1)&&(settings->stopBits()<=2)) {
    edit_stopbits_box->setCurrentItem(settings->stopBits()-1);
  }

  edit_parity_box->insertItem(tr("None"));
  edit_parity_box->insertItem(tr("Even"));
  edit_parity_box->insertItem(tr("Odd"));
  edit_parity_box->setCurrentItem((int)settings->parity());
}


EditTtySettings::~EditTtySettings()
{
}


QSize EditTtySettings::sizeHint() const
{
  return QSize(300,180);
}


QSizePolicy EditTtySettings::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditTtySettings::okData()
{
  switch(edit_speed_box->currentItem()) {
      case 0:
	edit_settings->setSpeed(50);
	break;

      case 1:
	edit_settings->setSpeed(75);
	break;

      case 2:
	edit_settings->setSpeed(110);
	break;

      case 3:
	edit_settings->setSpeed(134);
	break;

      case 4:
	edit_settings->setSpeed(150);
	break;

      case 5:
	edit_settings->setSpeed(200);
	break;

      case 6:
	edit_settings->setSpeed(300);
	break;

      case 7:
	edit_settings->setSpeed(600);
	break;

      case 8:
	edit_settings->setSpeed(1200);
	break;

      case 9:
	edit_settings->setSpeed(1800);
	break;

      case 10:
	edit_settings->setSpeed(2400);
	break;

      case 11:
	edit_settings->setSpeed(4800);
	break;

      case 12:
	edit_settings->setSpeed(9600);
	break;

      case 13:
	edit_settings->setSpeed(19200);
	break;

      case 14:
	edit_settings->setSpeed(38400);
	break;

      case 15:
	edit_settings->setSpeed(57600);
	break;

      case 16:
	edit_settings->setSpeed(115200);
	break;

      case 17:
	edit_settings->setSpeed(230400);
	break;
  }
  edit_settings->setWordLength(edit_wordlength_box->currentItem()+5);
  edit_settings->setStopBits(edit_stopbits_box->currentItem()+1);
  edit_settings->setParity((MLTTYDevice::Parity)edit_parity_box->currentItem());
  done(0);
}


void EditTtySettings::cancelData()
{
  done(-1);
}


