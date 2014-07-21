/*   detail_listview.cpp
 *
 *   The Detail ListView widget for MCallMan.
 *
 *   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <Qt3Support/q3header.h>
#include <Qt3Support/Q3PopupMenu>
#include <QtGui/QMouseEvent>

#include <ml_listviewitem.h>

#include "calleriddetail.h"
#include "edit_action.h"
#include "detail_listview.h"

DetailListView::DetailListView(const QString &showcode,
			       QWidget *parent,const char *name)
  : Q3ListView(parent,name)
{
  detail_parent=parent;
  detail_showcode=showcode;

  //
  // Right Button Menu
  //
  detail_menu=new Q3PopupMenu(this,"detail_menu");
  connect(detail_menu,SIGNAL(aboutToShow()),this,SLOT(aboutToShowData()));
  detail_menu->
    insertItem(tr("Edit Action Record"),this,SLOT(editActionData()),0,0);
}


void DetailListView::aboutToShowData()
{
  detail_menu->setItemEnabled(0,(detail_menu_item!=NULL)&&
			     (!detail_menu_item->text(15).isEmpty()));
}


void DetailListView::editActionData()
{
  MlListViewItem *item=(MlListViewItem *)selectedItem();
  if(item==NULL) {
    return;
  }
  EditAction *action=
    new EditAction(CallerIdDetail::simplifiedNumber(item->text(15)),
		   detail_showcode,item->id());
  action->exec();
  delete action;
}


void DetailListView::contentsMousePressEvent(QMouseEvent *e)
{
  Q3ListView::contentsMousePressEvent(e);
  detail_menu_item=selectedItem();
  switch(e->button()) {
      case Qt::RightButton:
	detail_menu->setGeometry(detail_parent->geometry().x()+
				 geometry().x()+e->pos().x()+2,
				 detail_parent->geometry().y()+
				 geometry().y()+e->pos().y()+
				 header()->geometry().height()+2,
				 detail_menu->sizeHint().width(),
				 detail_menu->sizeHint().height());
	detail_menu->exec();
	break;

      default:
	e->ignore();
	break;
  }
}
