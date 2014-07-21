// save_connection.cpp
//
// Save a CallCommander Connection
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
#include <QtCore/QFile>

#include "ml.h"
#include "save_connection.h"
#include "list_shows.h"
#include "telos2101_driver.h"

SaveConnection::SaveConnection(ConnectionData *conn,
			       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  save_connection=conn;

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

  setCaption(tr("Save Connection"));

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
    new QLabel(tr("Enter a name and description for this connection."),this);
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);

  //
  // Filename
  //
  save_filename_edit=new QLineEdit(this,"save_filename_edit");
  save_filename_edit->setGeometry(10,45,sizeHint().width()-20,20);
  save_filename_edit->setFont(font);
  connect(save_filename_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(textChangedData(const QString &)));
  label=new QLabel(save_filename_edit,tr("Name"),this,"save_filename_label");
  label->setGeometry(15,25,sizeHint().width()-25,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignBottom|Qt::AlignLeft);

  //
  // Description
  //
  save_description_edit=new QLineEdit(this,"save_description_edit");
  save_description_edit->setGeometry(10,85,sizeHint().width()-20,20);
  save_description_edit->setFont(font);
  label=new QLabel(save_description_edit,tr("Description"),
		   this,"save_description_label");
  label->setGeometry(15,65,sizeHint().width()-25,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignBottom|Qt::AlignLeft);

  //
  // Launch Checkbox
  //
  save_launch_box=new QCheckBox(this,"save_launch_box");
  save_launch_box->setGeometry(30,110,15,15);
  save_launch_box->setChecked(true);
  save_launch_box->setEnabled(conn->offerStartConnection());
  label=new QLabel(save_launch_box,"Open this connection now.",
		   this,"save_launch_label");
  label->setGeometry(50,108,sizeHint().width()-50,20);
  label->setFont(button_font);
  label->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
  label->setEnabled(conn->offerStartConnection());

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
  save_ok_button=new QPushButton(tr("Done"),this,"save_ok_button");
  save_ok_button->
    setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  save_ok_button->setFont(button_font);
  save_ok_button->setDefault(true);
  connect(save_ok_button,SIGNAL(clicked()),this,SLOT(okData()));
}


SaveConnection::~SaveConnection()
{
}


QSize SaveConnection::sizeHint() const
{
  return QSize(300,195);
}


QSizePolicy SaveConnection::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SaveConnection::textChangedData(const QString &text)
{
  save_ok_button->setDisabled(text.isEmpty());
}


void SaveConnection::okData()
{
  if(save_filename_edit->text().isEmpty()) {
    QMessageBox::warning(this,tr("Missing Name"),
			 tr("You must supply a connection name!"));
    return;
  }
  save_connection->setName(save_filename_edit->text());
  if(save_connection->exists()) {
    if(QMessageBox::question(this,tr("Connection Exists"),
     tr("A connection with this name already exists.\nOverwrite it?"),
		     QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) {
      return;
    }
  }
  save_connection->setDescription(save_description_edit->text());
  save_connection->setStartConnection(save_launch_box->isChecked());
  if(!save_connection->save()) {
    QMessageBox::warning(this,tr("Save Error"),
			 tr("Unable to save the connection!"));
  }
  done(0);
}


void SaveConnection::cancelData()
{
  done(-1);
}


