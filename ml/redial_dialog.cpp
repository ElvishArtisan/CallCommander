// redial_dialog.cpp
//
// A dialog to select a redial number for CallCommander
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QPushButton>

#include "globals.h"
#include "redial_dialog.h"

RedialDialog::RedialDialog(QString *number,QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  redial_number=number;
  setCaption(tr("Numbers"));

  //
  // Create Fonts
  //
  QFont button_font=QFont("helvetica",16,QFont::Bold);
  button_font.setPixelSize(16);
  QFont list_font=QFont("helvetica",16,QFont::Normal);
  list_font.setPixelSize(16);

  //
  // Number List
  //
  redial_number_list=new Q3ListView(this,"redial_number_list");
  redial_number_list->setSelectionMode(Q3ListView::Single);
  redial_number_list->setGeometry(10,10,
			      sizeHint().width()-20,sizeHint().height()-110);
  redial_number_list->setFont(list_font);
  redial_number_list->setAllColumnsShowFocus(true);
  redial_number_list->setItemMargin(5);
  redial_number_list->setSorting(-1);
  connect(redial_number_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  redial_number_list->addColumn("NUMBERS");
  redial_number_list->setColumnAlignment(0,Qt::AlignHCenter);

  //
  // Dial Button
  //
  QPushButton *button=new QPushButton(this,"dial_button");
  button->setGeometry(10,sizeHint().height()-90,sizeHint().width()/2-15,80);
  button->setFont(button_font);
  button->setText("&Dial");
  connect(button,SIGNAL(clicked()),this,SLOT(dialData()));

  //
  // Cancel
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()/2+5,sizeHint().height()-90,
		      sizeHint().width()/2-15,80);
  button->setFont(button_font);
  button->setText("&Cancel");
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  RefreshList();
}


QSize RedialDialog::sizeHint() const
{
  return QSize(200,450);
}


QSizePolicy RedialDialog::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void RedialDialog::doubleClickedData(Q3ListViewItem *item,const QPoint &pt,
				     int col)
{
  dialData();
}


void RedialDialog::dialData()
{
  Q3ListViewItem *item=redial_number_list->selectedItem();
  if(item==NULL) {
    return;
  }
  *redial_number=item->text(0);
  done(0);
}


void RedialDialog::cancelData()
{
  done(-1);
}


void RedialDialog::RefreshList()
{
  redial_number_list->clear();
  Q3ListViewItem *item;
  for(int i=(number_list->size()-1);i>=0;i--) {
    item=new Q3ListViewItem(redial_number_list);
    item->setText(0,number_list->number(i));
  }  
}
