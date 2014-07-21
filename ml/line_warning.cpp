// line_warning.cpp
//
// Line warning dialog for CallCommander. 
//
//   (C) Copyright 2002-2005 Fred Gleason <fredg@paravelsystems.com>
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

#include "line_warning.h"

LineWarning::LineWarning(QWidget *parent,const char *name)
  : QDialog(parent,name,false,Qt::WStyle_Customize|Qt::WStyle_NoBorder)
{
  setBackgroundColor(QColor(Qt::red));

  warn_label=new QLabel(this,"line_label");
  warn_label->setAlignment(Qt::AlignLeft|Qt::AlignTop);
  warn_label->setBackgroundColor(QColor(Qt::red));

  warn_button=new QPushButton(this,"line_button");
  warn_button->setText("OK");
  warn_button->setPalette(QPalette(warn_button->
				   palette().color(QPalette::Active,
						   QColorGroup::Base),
				   QColor(Qt::red)));
  connect(warn_button,SIGNAL(clicked()),SLOT(okData()));
}


QSize LineWarning::sizeHint() const
{
  return QSize(0,0);
}


QSizePolicy LineWarning::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void LineWarning::show(QString text)
{
  warn_label->setText(text);
  QDialog::show();
}


void LineWarning::setGeometry(int x,int y,int w,int h)
{
  warn_label->setGeometry(5,5,w-60,h-10);
  warn_button->setGeometry(w-50,h-50,50,50);
  QDialog::setGeometry(x+2,y+23,w,h);
}


void LineWarning::okData()
{
  done(0);
}
