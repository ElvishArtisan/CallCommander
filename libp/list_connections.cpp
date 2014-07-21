// list_connections.cpp
//
// List CallCommander Connections
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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtCore/QStringList>

#include "ml.h"
#include "list_connections.h"
#include "list_devices.h"
#include "conn_view_item.h"
#include "edit_connection_data.h"

ListConnections::ListConnections(ConnectionData *data,const QString &ok_text,
				 MlConfig *config,QWidget *parent,
				 const char *name)
  : QDialog(parent,name,false)
{
  list_data=data;
  list_config=config;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  setCaption(tr("Connection Manager"));

  //
  // Create Fonts
  //
  QFont font=QFont("Helvetica",12,QFont::Normal);
  font.setPixelSize(12);
  QFont button_font=QFont("Helvetica",12,QFont::Bold);
  button_font.setPixelSize(12);
  
  QPushButton *button;

  //
  // Connection List
  //
  list_view=new Q3ListView(this,"list_view");
  list_view->setGeometry(10,10,sizeHint().width()-130,sizeHint().height()-80);
  list_view->setFont(font);
  list_view->setAllColumnsShowFocus(true);
  list_view->setItemMargin(5);
  connect(list_view,
	  SIGNAL(doubleClicked(Q3ListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(listDoubleclickedData(Q3ListViewItem *,const QPoint &,int)));
  connect(list_view,SIGNAL(clicked(Q3ListViewItem *)),
	  this,SLOT(listClickedData(Q3ListViewItem *)));
  list_view->addColumn("Name");
  list_view->setColumnAlignment(0,Qt::AlignLeft);
  list_view->addColumn("Description");
  list_view->setColumnAlignment(1,Qt::AlignLeft);

  //
  // New Button
  //
  button=new QPushButton(tr("Create New\nConnection"),this,"new_button");
  button->setGeometry(sizeHint().width()-110,10,100,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(newData()));
  if(list_config!=NULL) {
    button->setDisabled(list_config->disableNewButton());
  }

  //
  // Edit Button
  //
  list_edit_button=new QPushButton(tr("Edit\nConnection"),
				   this,"list_edit_button");
  list_edit_button->setGeometry(sizeHint().width()-110,70,100,50);
  list_edit_button->setFont(button_font);
  list_edit_button->setDisabled(true);
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  //
  // Delete Button
  //
  list_delete_button=new QPushButton(tr("Delete\nConnection"),
				   this,"list_delete_button");
  list_delete_button->setGeometry(sizeHint().width()-110,130,100,50);
  list_delete_button->setFont(button_font);
  list_delete_button->setDisabled(true);
  connect(list_delete_button,SIGNAL(clicked()),this,SLOT(deleteData()));

  //
  // Connect Button
  //
  list_connect_button=new QPushButton(ok_text,this,"list_connect_button");
  list_connect_button->
    setGeometry(sizeHint().width()-220,sizeHint().height()-60,100,50);
  list_connect_button->setFont(button_font);
  list_connect_button->setDisabled(true);
  connect(list_connect_button,SIGNAL(clicked()),this,SLOT(connectData()));

  //
  // Cancel Button
  //
  button=new QPushButton(tr("Cancel"),this,"cancel_button");
  button->setGeometry(sizeHint().width()-110,sizeHint().height()-60,100,50);
  button->setFont(button_font);
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  RefreshList();
}


ListConnections::~ListConnections()
{
}


QSize ListConnections::sizeHint() const
{
  return QSize(420,300);
}


QSizePolicy ListConnections::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListConnections::newData()
{
  ConnectionData *conn=new ConnectionData(list_data->storageType());
  *conn=*list_data;
  ListDevices *list=new ListDevices(conn,this);
  if(list->exec()<0) {
    delete list;
    return;
  }
  delete list;
  ConnViewItem *item=new ConnViewItem(list_view);
  item->setConnectionData(conn);
  item->setText(0,conn->name());
  item->setText(1,conn->description());
  list_view->setSelected(item,true);
  list_view->ensureItemVisible(item);
  listClickedData(item);
  if(conn->startConnection()) {
    *list_data=*conn;
    done(0);
  }
}


void ListConnections::editData()
{
  bool found=false;
  ConnViewItem *item=(ConnViewItem *)list_view->selectedItem();
  if(item==NULL) {
    return;
  }
  QStringList slist;
  EditConnectionData *edit=
    new EditConnectionData(item->connectionData(),&slist,this);
  edit->exec();
  QStringList::iterator it;
  for(it=slist.begin();it!=slist.end();++it) {
    found=false;
    item=(ConnViewItem *)list_view->firstChild();
    while(item!=NULL) {
      if(item->text(0)==(*it)) {
	item->setText(1,item->connectionData()->description());
	found=true;
      }
      item=(ConnViewItem *)item->nextSibling();
    }
    if(!found) {
      item=new ConnViewItem(list_view);
      item->setConnectionData(new ConnectionData(list_data->storageType()));
      item->connectionData()->setName((*it));
      item->connectionData()->load();
      item->setText(0,*it);
      item->setText(1,item->connectionData()->description());
    }
  }
  delete edit;
}


void ListConnections::deleteData()
{
  ConnViewItem *item=(ConnViewItem *)list_view->selectedItem();
  if(item==NULL) {
    return;
  }
  if(QMessageBox::question(this,tr("Delete Connection?"),
		  tr("Are you sure you want to delete this connection?"),
		  QMessageBox::Yes,QMessageBox::No)==QMessageBox::No) { 
    return;
  }
  item->connectionData()->remove();
  delete item;
  listClickedData(list_view->selectedItem());
}


void ListConnections::connectData()
{
  ConnViewItem *item=(ConnViewItem *)list_view->selectedItem();
  if(item==NULL) {
    done(-1);
    return;
  }
  *list_data=*(item->connectionData());
  done(0);
}


void ListConnections::cancelData()
{
  done(-1);
}


void ListConnections::listClickedData(Q3ListViewItem *item)
{
  if(list_config==NULL) {
    list_edit_button->setDisabled(item==NULL);
    list_delete_button->setDisabled(item==NULL);
    list_connect_button->setDisabled(item==NULL);
  }
  else {
    list_edit_button->
      setDisabled((item==NULL)||list_config->disableEditButton());
    list_delete_button->
      setDisabled((item==NULL)||list_config->disableDeleteButton());
    list_connect_button->
      setDisabled((item==NULL)||(list_config->disableOpenButton()));
  }
}


void ListConnections::listDoubleclickedData(Q3ListViewItem *item,
					   const QPoint &pt,int col)
{
  connectData();
}


void ListConnections::RefreshList()
{
  ConnectionData *conn=NULL;
  ConnViewItem *item=NULL;
  QStringList::iterator it;
  QStringList filelist=ConnectionData::connections(list_data->storageType());

  for(it=filelist.begin();it!=filelist.end();++it) {
    conn=new ConnectionData(list_data->storageType());
    conn->setName(*it);
    if(conn->load()) {
      item=(ConnViewItem *)new ConnViewItem(list_view);
      item->setConnectionData(conn);
      item->setText(0,conn->name());
      item->setText(1,conn->description());
      if(conn->name()==list_data->name()) {
	list_view->setSelected(item,true);
	list_view->ensureItemVisible(item);
	listClickedData(item);
      }
    }
    else {
      delete conn;
    }
  }
}
