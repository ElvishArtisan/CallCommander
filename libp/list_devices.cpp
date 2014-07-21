// list_devices.cpp
//
// List CallCommander Devices
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

#include "list_devices.h"
#include "edit_connection.h"
#include "telos2101_driver.h"
#include "telos100_driver.h"
#include "ts612_driver.h"
#include "virtual_driver.h"
#include "asterisk_driver.h"

ListDevices::ListDevices(ConnectionData *conn,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  list_connection=conn;

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

  setCaption(tr("Select a Phone System Type"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  
  //
  // Question Text
  //
  QLabel *label=
    new QLabel(tr("What type of phone system are you connecting to?"),this);
  label->setGeometry(10,10,sizeHint().width()-20,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignCenter);

  //
  // Device Box
  //
  list_type_box=new QComboBox(this,"list_type_box");
  list_type_box->setGeometry(10,32,sizeHint().width()-20,20);
  list_type_box->setFont(font);

  //
  // Ok Button
  //
  QPushButton *button=new QPushButton(tr("Next"),this,"list_ok_button");
  button->
    setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  button->setDefault(true);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));


  //
  // Cancel Button
  //
  button=new QPushButton(tr("Back"),this,"cancel_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Device Values
  //
  list_type_box->insertItem("Asterisk Open Source PBX (Experimental)");
  list_type_box->insertItem("CallCommander Virtual Telephone System");
  list_type_box->
    insertItem("Comrex/Gentner TS-612 Multi-Line, On-Air Telephone System");
  list_type_box->insertItem("Telos 100 1A2 Interface Module");
  list_type_box->insertItem("Telos 100 Direct Interface Module");

  list_type_box->insertItem("Telos Nx12 Talk Show System");


  list_type_box->insertItem("Telos ONE-x-Six Talk Show Telephone System");
  list_type_box->
    insertItem("Telos TWOx12 Multiline Broadcast Telephone System");
  list_type_box->
    insertItem("Telos Series 2101 Multi-line Multi-studio Broadcast Telephone System");
}

ListDevices::~ListDevices()
{
}


QSize ListDevices::sizeHint() const
{
  return QSize(450,130);
}


QSizePolicy ListDevices::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListDevices::okData()
{
  switch(list_type_box->currentItem()) {
      case 0:
	list_connection->setSystemType(ConnectionData::TypeAsterisk);
	list_connection->setSubType(0);
	break;

      case 1:
	list_connection->setSystemType(ConnectionData::TypeVirtual);
	list_connection->setSubType(0);
	break;

      case 2:
	list_connection->setSystemType(ConnectionData::TypeGentnerTs612);
	list_connection->setSubType(0);
	break;

      case 3:
	list_connection->setSystemType(ConnectionData::TypeTelos100Key);
	list_connection->setSubType(0);
	break;

      case 4:
	list_connection->setSystemType(ConnectionData::TypeTelos100Dim);
	list_connection->setSubType(0);
	break;

      case 5:
	list_connection->setSystemType(ConnectionData::TypeTelos2101);
	list_connection->setSubType(1);
	break;

      case 6:
	list_connection->setSystemType(ConnectionData::TypeTelos100OneXSix);
	list_connection->setSubType(0);
	break;

      case 7:
	list_connection->setSystemType(ConnectionData::TypeTelos2101);
	list_connection->setSubType(1);
	break;

      case 8:
	list_connection->setSystemType(ConnectionData::TypeTelos2101);
	list_connection->setSubType(0);
	break;
  }
  switch(list_connection->systemType()) {
      case ConnectionData::TypeAsterisk:
	list_connection->setConnectionType(AsteriskDriver::connectionType());
	list_connection->setTcpPort(AsteriskDriver::defaultTcpPort());
	break;

      case ConnectionData::TypeTelos2101:
	list_connection->setConnectionType(Telos2101Driver::connectionType());
	if(list_connection->subType()==1) {
	  list_connection->setUserName(Telos2101Driver::defaultUserName());
	  list_connection->
	    setUserPassword(Telos2101Driver::defaultUserPassword());
	}
	else {
	  list_connection->setUserName("");
	  list_connection->setUserPassword("");
	}
	list_connection->setTcpPort(Telos2101Driver::defaultTcpPort());
	break;

      case ConnectionData::TypeTelos100Key:
      case ConnectionData::TypeTelos100Dim:
      case ConnectionData::TypeTelos100OneXSix:
	list_connection->setConnectionType(Telos100Driver::connectionType());
	break;

      case ConnectionData::TypeGentnerTs612:
	list_connection->setConnectionType(Ts612Driver::connectionType());
	break;

      case ConnectionData::TypeVirtual:
	list_connection->setConnectionType(VirtualDriver::connectionType());
	list_connection->setUserName(VirtualDriver::defaultUserName());
	list_connection->
	  setUserPassword(VirtualDriver::defaultUserPassword());
	list_connection->setTcpPort(VirtualDriver::defaultTcpPort());
	break;

      case ConnectionData::TypeUnknown:
	break;
  }
  EditConnection *edit_conn=
    new EditConnection(list_connection,this);
  if(edit_conn->exec()<0) {
    return;
  }
  done(0);
}


void ListDevices::cancelData()
{
  done(-1);
}


