// list_systems.cpp
//
// List CallCommander CallerID Sources.
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

#include <math.h>

#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>
#include <QtGui/QResizeEvent>
#include <QtSql/QSqlQuery>

#include <mldconfig.h>

#include "list_systems.h"
#include "add_system.h"
#include "edit_system.h"
#include "globals.h"

ListSystems::ListSystems(QWidget *parent,const char *name)
  : QDialog(parent,name,true)
{
  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());

  setCaption("Call Administrator - Virtual Systems");

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",10,QFont::Normal);
  small_font.setPixelSize(10);

  //
  // Systems List
  //
  list_systems_list=new Q3ListView(this,"list_systems_list");
  list_systems_list->setItemMargin(5);
  list_systems_list->setAllColumnsShowFocus(true);
  connect(list_systems_list,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(Q3ListViewItem *,const QPoint &,int)));
  list_systems_list->addColumn("Show Name");
  list_systems_list->setColumnAlignment(0,Qt::AlignLeft|Qt::AlignVCenter);
  list_systems_list->addColumn("Studio");
  list_systems_list->setColumnAlignment(1,Qt::AlignLeft|Qt::AlignVCenter);
  list_systems_list->addColumn("Connection");
  list_systems_list->setColumnAlignment(2,Qt::AlignLeft|Qt::AlignVCenter);
  list_systems_list->addColumn("Server");
  list_systems_list->setColumnAlignment(3,Qt::AlignLeft|Qt::AlignVCenter);

  //
  //  Add Button
  //
  list_add_button=new QPushButton(this,"list_add_button");
  list_add_button->setFont(font);
  list_add_button->setText("&Add");
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  //
  //  Edit Button
  //
  list_edit_button=new QPushButton(this,"list_edit_button");
  list_edit_button->setFont(font);
  list_edit_button->setText("&Edit");
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  //  Delete Button
  //
  list_delete_button=new QPushButton(this,"list_delete_button");
  list_delete_button->setFont(font);
  list_delete_button->setText("&Delete");
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  //  Close Button
  //
  list_close_button=new QPushButton(this,"list_close_button");
  list_close_button->setDefault(true);
  list_close_button->setFont(font);
  list_close_button->setText("&Close");
  connect(list_close_button,SIGNAL(clicked()),this,SLOT(closeData()));

  RefreshList();
}


ListSystems::~ListSystems()
{
}


QSize ListSystems::sizeHint() const
{
  return QSize(400,300);
} 


QSizePolicy ListSystems::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListSystems::addData()
{
  QString showname;
  QString sql;
  QSqlQuery *q;

  AddSystem *add=new AddSystem(&showname,this,"add");
  if(add->exec()<0) {
    delete add;
    return;
  }
  sql=QString().sprintf("insert into VIRTUAL_SYSTEMS set SHOW_NAME=\"%s\"",
			  (const char *)showname);
  q=new QSqlQuery(sql);
  delete q;
  EditSystem *edit=new EditSystem(showname,this,"edit");
  if(edit->exec()<0) {
    sql=QString().sprintf("delete from VIRTUAL_SYSTEMS where SHOW_NAME=\"%s\"",
			  (const char *)showname);
    q=new QSqlQuery(sql);
    delete q;
    delete edit;
    delete add;
    return;
  }
  Q3ListViewItem *item=new Q3ListViewItem(list_systems_list);
  item->setText(0,showname);
  UpdateItem(item);
  list_systems_list->setSelected(item,true);
  list_systems_list->ensureItemVisible(item);
  delete edit;
  delete add;
}


void ListSystems::editData()
{
  Q3ListViewItem *item=list_systems_list->selectedItem();
    if(item==NULL) {
    return;
  }
  EditSystem *edit=new EditSystem(item->text(0),this,"edit");
  if(edit->exec()==0) {
    UpdateItem(item);
  }
  delete edit;
}


void ListSystems::deleteData()
{
  QString sql;
  QSqlQuery *q;
  Q3ListViewItem *item=list_systems_list->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,"Delete Virtual System",
	      tr("Are you sure you want to delete this Virtual System?"),
			   QMessageBox::Yes,
			   QMessageBox::No)==QMessageBox::No) {
    return;
  }
  sql=QString().sprintf("delete from VIRTUAL_SYSTEMS where SHOW_NAME=\"%s\"",
			(const char *)item->text(0));
  q=new QSqlQuery(sql);
  delete q;
  delete item;
}


void ListSystems::doubleClickedData(Q3ListViewItem *item,
					    const QPoint &pt,int c)
{
  editData();
}


void ListSystems::closeData()
{
  done(0);
}


void ListSystems::resizeEvent(QResizeEvent *e)
{
  list_systems_list->setGeometry(10,10,size().width()-20,size().height()-80);
  list_add_button->setGeometry(10,size().height()-60,80,50);
  list_edit_button->setGeometry(100,size().height()-60,80,50);
  list_delete_button->setGeometry(190,size().height()-60,80,50);
  list_close_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void ListSystems::RefreshList()
{
  QSqlQuery *q;
  Q3ListViewItem *item;

  list_systems_list->clear();
  q=new QSqlQuery("select SHOW_NAME,LOCATION,CONNECTION,HOSTNAME \
                   from VIRTUAL_SYSTEMS");
  while (q->next()) {
    item=new Q3ListViewItem(list_systems_list);
    item->setText(0,q->value(0).toString());
    item->setText(1,q->value(1).toString());
    item->setText(2,q->value(2).toString());
    item->setText(3,q->value(3).toString());
  }
  delete q;
}


void ListSystems::UpdateItem(Q3ListViewItem *item)
{
  QSqlQuery *q=new 
    QSqlQuery(QString().sprintf("select LOCATION,CONNECTION,HOSTNAME from \
                                 VIRTUAL_SYSTEMS where SHOW_NAME=\"%s\"",
				(const char *)item->text(0)));
  if(q->first()) {
    item->setText(1,q->value(0).toString());
    item->setText(2,q->value(1).toString());
    item->setText(3,q->value(2).toString());
  }
  delete q;
}
