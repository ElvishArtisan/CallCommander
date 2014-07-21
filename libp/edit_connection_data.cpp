// edit_connection_data.cpp
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

#include <QtGui/QMessageBox>
#include <QtGui/QLabel>

#include "save_connection.h"
#include "edit_connection_data.h"
#include "list_shows.h"
#include "telos2101_driver.h"
#include "ml.h"

EditConnectionData::EditConnectionData(ConnectionData *conn,QStringList *slist,
			       QWidget *parent,const char *name)
  : QDialog(parent,name,false)
{
  edit_connection=conn;
  edit_slist=slist;

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

  SetCaption();

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  
  //
  // Connection Type
  //
  edit_type_label=new QLabel(this,"edit_type_label");
  edit_type_label->setGeometry(120,10,sizeHint().width()-130,20);
  edit_type_label->setFont(font);
  edit_type_label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  QLabel *label=new QLabel(tr("System Type:"),
			   this,"edit_description_label");
  label->setGeometry(10,10,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Description
  //
  edit_description_edit=new QLineEdit(this,"edit_description_edit");
  edit_description_edit->setGeometry(120,35,sizeHint().width()-130,20);
  label=new QLabel(edit_description_edit,tr("Description:"),
		   this,"edit_description_label");
  label->setGeometry(10,35,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Hostname
  //
  edit_hostname_edit=new QLineEdit(this,"edit_hostname_edit");
  edit_hostname_edit->setGeometry(120,60,sizeHint().width()-130,20);
  label=new QLabel(edit_hostname_edit,tr("Host Name:"),
		   this,"edit_hostname_label");
  label->setGeometry(10,60,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // TCP Port
  //
  edit_tcpport_spin=new QSpinBox(this,"edit_tcpport_spin");
  edit_tcpport_spin->setGeometry(120,85,60,20);
  edit_tcpport_spin->setRange(0,65535);
  label=new QLabel(edit_tcpport_spin,tr("TCP Port:"),
		   this,"edit_tcpport_label");
  label->setGeometry(10,85,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // User Name
  //
  edit_username_edit=new QLineEdit(this,"edit_username_edit");
  edit_username_edit->setGeometry(120,110,sizeHint().width()-130,20);
  label=new QLabel(edit_username_edit,tr("User Name:"),
		   this,"edit_username_label");
  label->setGeometry(10,110,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // User Password
  //
  edit_userpasswd1_edit=new QLineEdit(this,"edit_userpasswd1_edit");
  edit_userpasswd1_edit->setGeometry(120,135,sizeHint().width()-130,20);
  edit_userpasswd1_edit->setEchoMode(QLineEdit::Password);
  edit_userpasswd2_edit=new QLineEdit(this,"edit_userpasswd2_edit");
  edit_userpasswd2_edit->setGeometry(120,157,sizeHint().width()-130,20);
  edit_userpasswd2_edit->setEchoMode(QLineEdit::Password);
  label=new QLabel(edit_userpasswd1_edit,tr("User Password:"),
		   this,"edit_userpasswd1_label");
  label->setGeometry(10,135,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Serial Port
  //
  edit_serialport_edit=new QLineEdit(this,"edit_serialport_edit");
  edit_serialport_edit->setGeometry(120,182,sizeHint().width()-130,20);
  label=new QLabel(edit_serialport_edit,tr("Serial Port:"),
		   this,"edit_serialport_label");
  label->setGeometry(10,182,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Show Name
  //
  edit_showname_edit=new QLineEdit(this,"edit_showname_edit");
  edit_showname_edit->setGeometry(120,207,sizeHint().width()-130,20);
  label=new QLabel(edit_showname_edit,tr("Show Name:"),
		   this,"edit_showname_label");
  label->setGeometry(10,207,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Show Password
  //
  edit_showpasswd1_edit=new QLineEdit(this,"edit_showpasswd1_edit");
  edit_showpasswd1_edit->setGeometry(120,232,sizeHint().width()-130,20);
  edit_showpasswd1_edit->setEchoMode(QLineEdit::Password);
  edit_showpasswd2_edit=new QLineEdit(this,"edit_showpasswd2_edit");
  edit_showpasswd2_edit->setGeometry(120,254,sizeHint().width()-130,20);
  edit_showpasswd2_edit->setEchoMode(QLineEdit::Password);
  label=new QLabel(edit_showpasswd1_edit,tr("Show Password:"),
		   this,"edit_showpasswd1_label");
  label->setGeometry(10,232,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Location
  //
  edit_location_edit=new QLineEdit(this,"edit_location_edit");
  edit_location_edit->setGeometry(120,279,sizeHint().width()-130,20);
  label=new QLabel(edit_location_edit,tr("Studio:"),
		   this,"edit_location_label");
  label->setGeometry(10,279,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Console
  //
  edit_console_spin=new QSpinBox(this,"edit_console_spin");
  edit_console_spin->setGeometry(120,304,40,20);
  edit_console_spin->setRange(0,ML_MAX_CONSOLES);
  label=new QLabel(edit_console_spin,tr("Console:"),
		   this,"edit_console_label");
  label->setGeometry(10,304,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Console Name
  //
  edit_consolename_edit=new QLineEdit(this,"edit_consolename_edit");
  edit_consolename_edit->setGeometry(120,329,sizeHint().width()-130,20);
  label=new QLabel(edit_consolename_edit,tr("Console Name:"),
		   this,"edit_consolename_label");
  label->setGeometry(10,329,105,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  //
  // Save Button
  //
  QPushButton *button=new QPushButton(tr("Save"),this,"edit_save_button");
  button->
    setGeometry(10,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(saveData()));

  //
  // Save As Button
  //
  button=new QPushButton(tr("Save As"),this,"edit_saveas_button");
  button->
    setGeometry(100,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(saveasData()));

  //
  // Ok Button
  //
  button=new QPushButton(tr("OK"),this,"edit_ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this,"cancel_button");
  button->
    setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Device Values
  //
  switch(conn->systemType()) {
      case ConnectionData::TypeVirtual:
	edit_type_label->setText("CallCommander Virtual Telephone System");
	break;

      case ConnectionData::TypeGentnerTs612:
	edit_type_label->
	  setText("Comrex/Gentner TS-612 Multi-Line, On-Air Telephone System");
	break;

      case ConnectionData::TypeTelos100Key:
	edit_type_label->setText("Telos 100 1A2 Interface Module");
	break;

      case ConnectionData::TypeTelos100Dim:
	edit_type_label->setText("Telos 100 Direct Interface Module");
	break;

      case ConnectionData::TypeTelos100OneXSix:
	edit_type_label->setText("Telos ONE-x-Six Talk Show Telephone System");
	break;

      case ConnectionData::TypeTelos2101:
	switch(conn->subType()) {
	    case 1:
	      edit_type_label->
		setText("Telos TWOx12 Multiline Broadcast Telephone System");
	      break;

	    case 0:
	      edit_type_label->
		setText("Telos Series 2101 Multi-line Multi-studio Broadcast Telephone System");
	      break;

	    default:
	      edit_type_label->setText("Unknown/Undefined Type");
	      break;
	}
	break;

      case ConnectionData::TypeUnknown:
	edit_type_label->setText("Unknown/Undefined Type");
	break;
  }
  switch(conn->systemType()) {
      case ConnectionData::TypeVirtual:
      case ConnectionData::TypeTelos2101:
	edit_serialport_edit->setDisabled(true);
	break;

      case ConnectionData::TypeGentnerTs612:
      case ConnectionData::TypeTelos100Key:
      case ConnectionData::TypeTelos100Dim:
      case ConnectionData::TypeTelos100OneXSix:
	edit_hostname_edit->setDisabled(true);
	edit_tcpport_spin->setDisabled(true);
	edit_username_edit->setDisabled(true);
	edit_userpasswd1_edit->setDisabled(true);
	edit_userpasswd2_edit->setDisabled(true);
	break;

      case ConnectionData::TypeUnknown:
	edit_serialport_edit->setDisabled(true);
	edit_hostname_edit->setDisabled(true);
	edit_tcpport_spin->setDisabled(true);
	edit_username_edit->setDisabled(true);
	edit_userpasswd1_edit->setDisabled(true);
	edit_userpasswd2_edit->setDisabled(true);
	edit_showname_edit->setDisabled(true);
	edit_showpasswd1_edit->setDisabled(true);
	edit_showpasswd2_edit->setDisabled(true);
	edit_location_edit->setDisabled(true);
	edit_console_spin->setDisabled(true);
	edit_consolename_edit->setDisabled(true);
	break;
  }
  edit_description_edit->setText(conn->description());
  edit_hostname_edit->setText(conn->hostName());
  edit_tcpport_spin->setValue(conn->tcpPort());
  edit_serialport_edit->setText(conn->ttyPort());
  edit_username_edit->setText(conn->userName());
  edit_userpasswd1_edit->setText(conn->userPassword());
  edit_userpasswd2_edit->setText(conn->userPassword());
  edit_showname_edit->setText(conn->showName());
  edit_showpasswd1_edit->setText(conn->showPassword());
  edit_showpasswd2_edit->setText(conn->showPassword());
  edit_location_edit->setText(conn->location());
  edit_console_spin->setValue(conn->console());
  edit_consolename_edit->setText(conn->consoleName());
}


EditConnectionData::~EditConnectionData()
{
}


QSize EditConnectionData::sizeHint() const
{
  return QSize(400,430);
}


QSizePolicy EditConnectionData::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditConnectionData::saveData()
{
  Save();
}


void EditConnectionData::saveasData()
{
  ConnectionData data(edit_connection->storageType());
  data=*edit_connection;
  data.setOfferStartConnection(false);
  SetConnectionData(&data);
  SaveConnection *save_conn=new SaveConnection(&data,this);
  if(save_conn->exec()==0) {
    *edit_connection=data;
    SetCaption();
    edit_description_edit->setText(edit_connection->description());
    edit_slist->push_back(edit_connection->name());
  }
}


void EditConnectionData::okData()
{
  if(Save()) {
    done(0);
  }
}


void EditConnectionData::cancelData()
{
  done(-1);
}


bool EditConnectionData::Save()
{
  if(!SetConnectionData(edit_connection)) {
    return false;
  }
  edit_connection->save();
  edit_slist->push_back(edit_connection->name());

  return true;
}


bool EditConnectionData::SetConnectionData(ConnectionData *conn)
{
  if(edit_userpasswd1_edit->text()!=edit_userpasswd2_edit->text()) {
    QMessageBox::warning(this,tr("Password Mismatch"),
			 tr("The User Passwords don't match!"));
    return false;
  }
  if(edit_showpasswd1_edit->text()!=edit_showpasswd2_edit->text()) {
    QMessageBox::warning(this,tr("Password Mismatch"),
			 tr("The Show Passwords don't match!"));
    return false;
  }
  conn->setDescription(edit_description_edit->text());
  conn->setHostName(edit_hostname_edit->text());
  conn->setTcpPort(edit_tcpport_spin->value());
  conn->setTtyPort(edit_serialport_edit->text());
  conn->setUserName(edit_username_edit->text());
  conn->setUserPassword(edit_userpasswd1_edit->text());
  conn->setShowName(edit_showname_edit->text());
  conn->setShowPassword(edit_showpasswd1_edit->text());
  conn->setLocation(edit_location_edit->text());
  conn->setConsole(edit_console_spin->value());
  conn->setConsoleName(edit_consolename_edit->text());

  return true;
}


void EditConnectionData::SetCaption()
{
  setCaption(QString().sprintf("%s \"%s\"",
			       (const char *)tr("Editing Connection"),
			       (const char *)edit_connection->name()));
}
