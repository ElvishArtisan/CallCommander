// add_map_line.cpp
//
// Edit a CallCommander CID Line Map.
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

#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QCloseEvent>

#include "add_map_line.h"

AddMapLine::AddMapLine(int *src_line,int *dest_line,bool add_mode,
		       QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  add_source_line=src_line;
  add_destination_line=dest_line;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  if(add_mode) {
    setCaption("Call Administrator - Add CID Map Entry");
  }
  else {
    setCaption("Call Administrator - Edit CID Map Entry");
  }

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont day_font=QFont("Helvetica",10,QFont::Normal);
  day_font.setPixelSize(10);

  //
  // Source Line
  //
  add_source_spin=new QSpinBox(this,"add_source_spin");
  add_source_spin->setGeometry(120,10,60,20);
  add_source_spin->setRange(1,64);
  QLabel *label=new QLabel(add_source_spin,tr("Source Line:"),
			   this,"add_source_spin");
  label->setGeometry(10,10,105,20);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  // Destination Line
  //
  add_destination_spin=new QSpinBox(this,"add_destination_spin");
  add_destination_spin->setGeometry(120,40,60,20);
  add_destination_spin->setRange(0,64);
  add_destination_spin->setSpecialValueText(tr("OFF"));
  label=new QLabel(add_destination_spin,tr("Destination Line:"),
		   this,"add_destination_spin");
  label->setGeometry(10,40,105,20);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

  //
  //  Ok Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-190,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText(tr("&Ok"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Load Data
  //
  if(!add_mode) {
    add_source_spin->setValue(*src_line);
    add_source_spin->setRange(*src_line,*src_line);
    add_destination_spin->setValue(*dest_line);
  }
}


AddMapLine::~AddMapLine()
{
}


QSize AddMapLine::sizeHint() const
{
  return QSize(200,150);
}


QSizePolicy AddMapLine::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void AddMapLine::okData()
{
  *add_source_line=add_source_spin->value();
  *add_destination_line=add_destination_spin->value();
  done(0);
}


void AddMapLine::cancelData()
{
  done(-1);
}


void AddMapLine::closeEvent(QCloseEvent *e)
{
  cancelData();
}
