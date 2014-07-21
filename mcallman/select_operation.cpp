// select_operation.cpp
//
// Select CallCommander Show Records.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <math.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>

#include "list_actions.h"
#include "list_details.h"
#include "list_winners.h"
#include "select_operation.h"

SelectOperation::SelectOperation(QString showcode,QString showtitle,
		   QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  select_showcode=showcode;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  setCaption("Call Manager - Select Operation");

  //
  // Create Fonts
  //
  QFont label_font=QFont("Helvetica",14,QFont::Bold);
  label_font.setPixelSize(14);
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Show Label
  //
  QLabel *label=new QLabel(showtitle,this,"show_label");
  label->setGeometry(10,5,sizeHint().width()-20,20);
  label->setFont(label_font);
  label->setAlignment(Qt::AlignCenter);

  //
  //  Actions Button
  //
  QPushButton *button=new QPushButton(this,"actions_button");
  button->setGeometry(10,32,100,50);
  button->setFont(font);
  button->setText("Configure \n&Actions");
  connect(button,SIGNAL(clicked()),this,SLOT(actionsData()));

  //
  //  Details Button
  //
  button=new QPushButton(this,"details_button");
  button->setGeometry(120,32,100,50);
  button->setFont(font);
  button->setText("View Call\n&Details");
  connect(button,SIGNAL(clicked()),this,SLOT(detailsData()));

  //
  //  Contests Button
  //
  button=new QPushButton(this,"contests_button");
  button->setGeometry(65,92,100,50);
  button->setFont(font);
  button->setText("Manage\n&Contests");
  connect(button,SIGNAL(clicked()),this,SLOT(contestsData()));

  //
  //  Close Button
  //
  button=new QPushButton(this,"close_button");
  button->setGeometry(10,sizeHint().height()-60,sizeHint().width()-20,50);
  button->setFont(font);
  button->setText("&Close");
  button->setDefault(true);
  connect(button,SIGNAL(clicked()),this,SLOT(closeData()));
}


SelectOperation::~SelectOperation()
{
}


QSize SelectOperation::sizeHint() const
{
  return QSize(225,215);
} 


QSizePolicy SelectOperation::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void SelectOperation::actionsData()
{
  ListActions *list=new ListActions(select_showcode,this,"list");
  list->exec();
  delete list;
}


void SelectOperation::detailsData()
{
  ListDetails *list=new ListDetails(select_showcode,this,"list");
  list->exec();
  delete list;
}


void SelectOperation::contestsData()
{
  ListWinners *list=new ListWinners(select_showcode,this,"list");
  list->exec();
  delete list;
}


void SelectOperation::closeData()
{
  done(-1);
}
