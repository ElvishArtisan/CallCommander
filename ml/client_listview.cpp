//   client_listview.cpp
//
//   The Client ListView widget for CallCommander.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: client_listview.cpp,v 1.4 2010/11/28 19:50:04 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
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

#include <Qt3Support/q3header.h>
#include <Qt3Support/Q3PopupMenu>
#include <QtGui/QMouseEvent>

#include "globals.h"
#include "client_listview.h"

ClientListView::ClientListView(QWidget *parent,const char *name)
  : Q3ListView(parent,name)
{
  client_parent=parent;

  //
  // Right Button Menu
  //
  client_menu=new Q3PopupMenu(this,"client_menu");
  connect(client_menu,SIGNAL(aboutToShow()),this,SLOT(aboutToShowData()));
  client_menu->
    insertItem(tr("Kill Remote Client"),this,SLOT(killClientData()),0,0);
}


void ClientListView::aboutToShowData()
{
  client_menu->setItemChecked(0,false); 
  client_menu->setItemEnabled(0,(client_menu_item->text(7)!=tr("Unknown"))&&
			      (client_menu_item->line()!=0));
}


void ClientListView::killClientData()
{
  telos_driver->sendString(BusDriver::ClientKillGlobalElement,0,
			   client_menu_item->text(3));
  delete client_menu_item;
  client_menu_item=NULL;
}


void ClientListView::contentsMousePressEvent(QMouseEvent *e)
{
  Q3ListView::contentsMousePressEvent(e);
  if(((client_menu_item=(ClientListViewItem *)selectedItem())==NULL)||
     (!ml_config->allowClientKill())) {
    e->ignore();
    return;
  }
  switch(e->button()) {
      case Qt::RightButton:
	client_menu->setGeometry(client_parent->geometry().x()+
				 geometry().x()+e->pos().x()+2,
				 client_parent->geometry().y()+
				 geometry().y()+e->pos().y()+
				 header()->geometry().height()+2,
				 client_menu->sizeHint().width(),
				 client_menu->sizeHint().height());
	client_menu->exec();
	break;

      default:
	e->ignore();
	break;
  }
}


void ClientListView::contentsMouseDoubleClickEvent(QMouseEvent *e)
{
/*
  QListView::contentsMouseDoubleClickEvent(e);
  client_menu_item=selectedItem();
  if(client_menu_item==NULL) {
    return;
  }
  if(client_log->
     logLine(client_menu_item->text(6).toInt())->type()==RDLogLine::Marker) {
    editNoteMenuData();
  }
*/
}
