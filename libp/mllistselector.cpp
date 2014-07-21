//   mllistselector.cpp
//
//   A List Selector Widget.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mllistselector.cpp,v 1.4 2011/07/20 17:12:52 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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
//

#include <stdlib.h>
#include <stdio.h>
#include <QtGui/QWidget>
#include <Qt3Support/q3hbox.h>
#include <Qt3Support/q3vbox.h>
#include <Qt3Support/q3listview.h>
#include <QtCore/QString>
#include <QtGui/QPixmap>
#include <QtGui/QLabel>

#include "mllistselector.h"

MLListSelector::MLListSelector(QWidget *parent,const char *name)
  : Q3HBox(parent,name)
{
  QFont font;

  //
  // Generate Font
  //
  font=QFont("Helvetica",10,QFont::Bold);
  font.setPixelSize(10);

  setSpacing(10);

  Q3VBox *source_box=new Q3VBox(this,"source_box");
  list_source_label=new QLabel(source_box,"list_source_label");
  list_source_label->setFont(font);
  list_source_label->setText(tr("Available Services"));
  list_source_label->setAlignment(Qt::AlignCenter);
  list_source_box=new Q3ListBox(source_box,"list_source_box");

  Q3VBox *button_box=new Q3VBox(this,"button_box");
  list_add_button=new QPushButton(button_box,"list_add_button");
  list_add_button->setText(tr("Add >>"));
  list_add_button->setDisabled(true);
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));
  list_remove_button=new QPushButton(button_box,"list_add_button");
  list_remove_button->setText(tr("<< Remove"));
  list_remove_button->setDisabled(true);
  connect(list_remove_button,SIGNAL(clicked()),this,SLOT(removeData()));

  Q3VBox *dest_box=new Q3VBox(this,"dest_box");
  list_dest_label=new QLabel(dest_box,"list_dest_label");
  list_dest_label->setFont(font);
  list_dest_label->setText(tr("Active Services"));
  list_dest_label->setAlignment(Qt::AlignCenter);
  list_dest_box=new Q3ListBox(dest_box,"list_dest_box");
}


uint MLListSelector::sourceCount() const
{
  return list_source_box->count();
}


uint MLListSelector::destCount() const
{
  return list_dest_box->count();
}


void MLListSelector::sourceSetLabel(QString label)
{
  list_source_label->setText(label);
}


void MLListSelector::destSetLabel(QString label)
{
  list_dest_label->setText(label);
}


void MLListSelector::sourceInsertItem(const QString &text,int index)
{
  list_source_box->insertItem(text,index);
  list_source_box->sort();
  CheckButtons();
}


void MLListSelector::destInsertItem(const QString &text,int index)
{
  list_dest_box->insertItem(text,index);
  list_dest_box->sort();
  CheckButtons();
}


void MLListSelector::sourceRemoveItem(int index)
{
  list_source_box->removeItem(index);
  CheckButtons();
}


void MLListSelector::destRemoveItem(int index)
{
  list_dest_box->removeItem(index);
  CheckButtons();
}


QString MLListSelector::sourceText(int index) const
{
  return list_source_box->text(index);
}


QString MLListSelector::destText(int index) const
{
  return list_dest_box->text(index);
}


void MLListSelector::sourceChangeItem(const QString &text,int index)
{
  list_source_box->changeItem(text,index);
  list_source_box->sort();
}


void MLListSelector::destChangeItem(const QString &text,int index)
{
  list_dest_box->changeItem(text,index);
  list_dest_box->sort();
}


int MLListSelector::sourceNumItemsVisible() const
{
  return list_source_box->numItemsVisible();
}


int MLListSelector::destNumItemsVisible() const
{
  return list_dest_box->numItemsVisible();
}


int MLListSelector::sourceCurrentItem() const
{
  return list_source_box->currentItem();
}


int MLListSelector::destCurrentItem() const
{
  return list_dest_box->currentItem();
}


QString MLListSelector::sourceCurrentText() const
{
  return list_source_box->currentText();
}


QString MLListSelector::destCurrentText() const
{
  return list_dest_box->currentText();
}


void MLListSelector::sourceSetCurrentItem(int item)
{
  list_source_box->setCurrentItem(item);
}


void MLListSelector::destSetCurrentItem(int item)
{
  list_dest_box->setCurrentItem(item);
}


Q3ListBoxItem *MLListSelector::sourceFindItem(const QString &text,
					      Q3ListView::ComparisonFlags compare) const
{
  return list_source_box->findItem(text,compare);
}


Q3ListBoxItem *MLListSelector::destFindItem(const QString &text,
					    Q3ListView::ComparisonFlags compare) const
{
  return list_dest_box->findItem(text,compare);
}


void MLListSelector::clear()
{
  list_source_box->clear();
  list_dest_box->clear();
}


void MLListSelector::addData()
{
  if(list_source_box->currentItem()>=0) {
    list_dest_box->
      insertItem(list_source_box->currentText());
    list_source_box->removeItem(list_source_box->currentItem());
    list_dest_box->sort();
    if(list_source_box->count()==0) {
      list_add_button->setDisabled(true);
    }
    list_remove_button->setEnabled(true);
    list_source_box->setCurrentItem(-1);
  }
}


void MLListSelector::removeData()
{
  if(list_dest_box->currentItem()>=0) {
    list_source_box->
      insertItem(list_dest_box->currentText());
    list_dest_box->removeItem(list_dest_box->currentItem());
    list_source_box->sort();
    if(list_dest_box->count()==0) {
      list_remove_button->setDisabled(true);
    }
    list_add_button->setEnabled(true);
    list_dest_box->setCurrentItem(-1);
  }
}


void MLListSelector::CheckButtons()
{
  if(list_source_box->count()==0) {
    list_add_button->setDisabled(true);
  }
  else {
    list_add_button->setEnabled(true);
  }
  if(list_dest_box->count()==0) {
    list_remove_button->setDisabled(true);
  }
  else {
    list_remove_button->setEnabled(true);
  }
}
