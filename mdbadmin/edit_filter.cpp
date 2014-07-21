// edit_filter.cpp
//
// Edit a CallCommander Filter String
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QValidator>

#include "edit_filter.h"

EditFilter::EditFilter(QString *str,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  edit_str=str;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Administrator - Edit Filter String");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Filter String
  //
  edit_filter_edit=new QLineEdit(this,"edit_filter_edit");
  edit_filter_edit->setGeometry(112,10,sizeHint().width()-122,20);
  edit_filter_label=new QLabel(edit_filter_edit,tr("Filter String:"),
		   this,"edit_filter_label");
  edit_filter_label->setGeometry(22,10,85,20);
  edit_filter_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  edit_filter_label->setFont(label_font);
  edit_filter_edit->setText(*str);

  //
  // OK Button
  //
  QPushButton *button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(label_font);
  button->setText("&OK");
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  // Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(label_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


EditFilter::~EditFilter()
{
}


QSize EditFilter::sizeHint() const
{
  return QSize(300,100);
}


QSizePolicy EditFilter::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditFilter::okData()
{
  *edit_str=edit_filter_edit->text();
  done(0);
}


void EditFilter::cancelData()
{
  done(-1);
}
