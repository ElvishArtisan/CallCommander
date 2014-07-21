// edit_connection.cpp
//
// Edit a CallCommander Connection
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

#include "edit_connection.h"
#include "list_shows.h"
#include "telos2101_driver.h"
#include "telos100_driver.h"
#include "ts612_driver.h"
#include "virtual_driver.h"
#include "asterisk_driver.h"


EditConnection::EditConnection(ConnectionData *conn,
			       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_connection=conn;

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

  setCaption(tr("System Connection"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  
  //
  // Progress Dialog
  //
  edit_progress_dialog=new Q3ProgressDialog(this,"edit_progress_dialog");
  edit_progress_dialog->setLabelText(tr("Attempting to connect to system..."));
  edit_progress_dialog->setCancelButton(NULL);
  edit_progress_dialog->setCaption(tr("Connecting"));

  //
  // Question Text
  //
  QLabel *label=
    new QLabel(tr("Enter the connection settings."),this);
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Hostname
  //
  edit_hostname_edit=new QLineEdit(this,"edit_hostname_edit");
  edit_hostname_edit->setGeometry(120,30,sizeHint().width()-130,20);
  edit_hostname_edit->setFont(font);
  connect(edit_hostname_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));

  //
  // Serial Port
  //
  edit_serialport_box=new QComboBox(this,"edit_serialport_box");
  edit_serialport_box->setGeometry(120,30,sizeHint().width()-130,20);
  MLTTYDevice *dev=new MLTTYDevice(this);
  QString sname;
  for(int i=0;i<64;i++) {
  //for(int i=0;i<4;i++) {
#ifdef WIN32
    sname=QString().sprintf("COM%d",i+1);
#else
    sname=QString().sprintf("/dev/ttyS%d",i);
#endif  // WIN32
    dev->setName(sname);
    if(dev->open(QIODevice::ReadWrite)) {
      edit_serialport_box->insertItem(sname);
      dev->close();
    }
  }
#ifndef WIN32
  for(int i=0;i<64;i++) {
    sname=QString().sprintf("/dev/ttyUSB%d",i);
    dev->setName(sname);
    if(dev->open(QIODevice::ReadWrite)) {
      edit_serialport_box->insertItem(sname);
      dev->close();
    }
  }
#endif  // WIN32
  label=new QLabel(edit_hostname_edit,tr("Host Name:"),
		   this,"edit_hostname_label");
  label->setGeometry(10,30,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  if(conn->connectionType()==BusDriver::SerialConnection) {
    label->setText(tr("Serial Port:"));
    edit_hostname_edit->hide();
  }
  else {
    label->setText(tr("Host Name:"));
    edit_serialport_box->hide();
  }

  //
  // Username
  //
  edit_username_edit=new QLineEdit(this,"edit_username_edit");
  edit_username_edit->setGeometry(120,55,sizeHint().width()-130,20);
  edit_username_edit->setFont(font);
  connect(edit_username_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));
  label=new QLabel(edit_username_edit,tr("User Name:"),
		   this,"edit_username_label");
  label->setGeometry(10,55,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  if((conn->connectionType()!=BusDriver::TcpConnection)||
     (conn->subType()==1)) {
    edit_username_edit->hide();
    label->hide();
  }

  //
  // Password
  //
  edit_password_edit=new QLineEdit(this,"edit_password_edit");
  edit_password_edit->setGeometry(120,80,sizeHint().width()-130,20);
  edit_password_edit->setFont(font);
  edit_password_edit->setEchoMode(QLineEdit::Password);
  label=new QLabel(edit_password_edit,tr("User Password:"),
		   this,"edit_password_label");
  label->setGeometry(10,80,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  if((conn->connectionType()!=BusDriver::TcpConnection)||
     (conn->subType()==1)) {
    edit_password_edit->hide();
    label->hide();
  }

  //
  // TCP Port
  //
  edit_tcpport_spin=new QSpinBox(this,"edit_tcpport_spin");
  edit_tcpport_spin->setRange(0,65535);
  edit_tcpport_spin->setFont(font);
  label=new QLabel(edit_tcpport_spin,tr("TCP Port:"),
		   this,"edit_tcpport_label");
  if(conn->subType()==1) {
    edit_tcpport_spin->setGeometry(120,55,60,20);
    label->setGeometry(10,55,105,20);
  }
  else {
    edit_tcpport_spin->setGeometry(120,105,60,20);
    label->setGeometry(10,105,105,20);
  }
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
  if(conn->connectionType()!=BusDriver::TcpConnection) {
    edit_tcpport_spin->hide();
    label->hide();
  }

  QPushButton *button;

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Back"),this,"cancel_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Ok Button
  //
  edit_ok_button=new QPushButton(tr("Next"),this,"edit_ok_button");
  edit_ok_button->
    setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  edit_ok_button->setFont(button_font);
  edit_ok_button->setDefault(true);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Load Device Values
  //
  switch(conn->connectionType()) {
      case BusDriver::TcpConnection:
	edit_hostname_edit->setText(edit_connection->hostName());
	edit_username_edit->setText(edit_connection->userName());
	edit_password_edit->setText(edit_connection->userPassword());
	edit_tcpport_spin->setValue(edit_connection->tcpPort());
	break;

      case BusDriver::SerialConnection:
	edit_hostname_edit->setText(edit_connection->ttyPort());
	break;

      case BusDriver::UnknownConnection:
	break;
  }
  textChangedData("");

  //
  // The Protocol Bus Driver
  //
  switch(edit_connection->systemType()) {
      case ConnectionData::TypeAsterisk:
	edit_bus_driver=new AsteriskDriver(0,this,"edit_bus_driver");
	break;
 
      case ConnectionData::TypeTelos2101:
	edit_bus_driver=new Telos2101Driver(0,this,"edit_bus_driver");
	break;
 
      case ConnectionData::TypeTelos100Key:
	edit_bus_driver=
	  new Telos100Driver(Telos100Driver::KeyType,0,this,"edit_bus_driver");
	break;

      case ConnectionData::TypeTelos100Dim:
	edit_bus_driver=
	  new Telos100Driver(Telos100Driver::DimType,0,this,"edit_bus_driver");
	break;

      case ConnectionData::TypeTelos100OneXSix:
	edit_bus_driver=
	  new Telos100Driver(Telos100Driver::OneXSixType,0,this,
			     "edit_bus_driver");
	break;

      case ConnectionData::TypeGentnerTs612:
	edit_bus_driver=new Ts612Driver(0,this,"edit_bus_driver");
	break;

      case ConnectionData::TypeVirtual:
	edit_bus_driver=new VirtualDriver(0,this,"edit_bus_driver");
	break;

      default:
	edit_bus_driver=NULL;
	break;
  }
  connect(edit_bus_driver,SIGNAL(connected(unsigned,bool)),
	  this,SLOT(busConnectedData(unsigned,bool)));
  connect(edit_bus_driver,SIGNAL(error(unsigned,BusDriver::Error)),
	  this,SLOT(busErrorData(unsigned,BusDriver::Error)));
}


EditConnection::~EditConnection()
{
}


QSize EditConnection::sizeHint() const
{
  switch(edit_connection->connectionType()) {
    case BusDriver::TcpConnection:
      if(edit_connection->subType()==1) {
	return QSize(300,155);
      }
      return QSize(300,205);
      
    case BusDriver::SerialConnection:
    case BusDriver::UnknownConnection:
      break;
  }
  return QSize(300,130);
}


QSizePolicy EditConnection::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditConnection::textChangedData(const QString &text)
{
  if(edit_hostname_edit->text().isEmpty()||
     edit_username_edit->text().isEmpty()) {
    edit_ok_button->setDisabled(false);
  }
}


void EditConnection::busConnectedData(unsigned id,bool state)
{
  if(state) {
    switch(edit_connection->connectionType()) {
	case BusDriver::TcpConnection:
	  edit_connection->setHostName(edit_hostname_edit->text());
	  edit_connection->setUserName(edit_username_edit->text());
	  edit_connection->setUserPassword(edit_password_edit->text());
	  edit_connection->setTcpPort(edit_tcpport_spin->value());
	  break;

	case BusDriver::SerialConnection:
	  edit_connection->setTtyPort(edit_serialport_box->currentText());
	  break;

	case BusDriver::UnknownConnection:
	  break;
    }
    ListShows *list_shows=
      new ListShows(edit_connection,edit_bus_driver,this);
    if(list_shows->exec()<0) {
      delete list_shows;
      return;
    }
    delete list_shows;
    done(0);
  }
  else {
    edit_progress_dialog->reset();
    QMessageBox::warning(this,tr("Connection Error"),
			 tr("Unable to connect to system!"));
  }
}


void EditConnection::busErrorData(unsigned id,BusDriver::Error err)
{
  edit_progress_dialog->reset();
  QMessageBox::warning(this,tr("Network Error"),BusDriver::errorText(err));
}


void EditConnection::connectionStepChangedData(unsigned id,int step)
{
  edit_progress_dialog->
    setProgress(step,edit_bus_driver->totalConnectionSteps());
}


void EditConnection::okData()
{
  edit_progress_dialog->setTotalSteps(edit_bus_driver->totalConnectionSteps());
  connect(edit_bus_driver,SIGNAL(connectionStepChanged(unsigned,int)),
	  this,SLOT(connectionStepChangedData(unsigned,int)));
  switch(edit_connection->connectionType()) {
      case BusDriver::TcpConnection:
	edit_bus_driver->connectToHost(edit_hostname_edit->text(),
				       edit_username_edit->text(),
				       edit_password_edit->text(),
				       edit_tcpport_spin->value());
	break;

      case BusDriver::SerialConnection:
	edit_bus_driver->connectToSerial(edit_serialport_box->currentText());
	break;

      case BusDriver::UnknownConnection:
	break;
  }
}


void EditConnection::cancelData()
{
  done(-1);
}
