/*   winner_listview.cpp
 *
 *   The Winner ListView widget for MCallMan.
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

#include "show_detail.h"
#include "calleriddetail.h"
#include "edit_action.h"
#include "winner_listview.h"

WinnerListView::WinnerListView(const QString &showcode,QWidget *parent)
  : Q3ListView(parent)
{
  winner_parent=parent;
  winner_showcode=showcode;

  //
  // Right Button Menu
  //
  winner_menu=new Q3PopupMenu(this,"winner_menu");
  connect(winner_menu,SIGNAL(aboutToShow()),this,SLOT(aboutToShowData()));
  winner_menu->
    insertItem(tr("Edit Action Record"),this,SLOT(editActionData()),0,0);
  winner_menu->
    insertItem(tr("Show Call Detail"),this,SLOT(showDetailData()),0,0);
}


void WinnerListView::aboutToShowData()
{
  winner_menu->setItemEnabled(0,(winner_menu_item!=NULL)&&
			      (!winner_menu_item->pnum().isEmpty()));
  winner_menu->setItemEnabled(1,(winner_menu_item!=NULL)&&
			      (winner_menu_item->detailId()!=0));
}


void WinnerListView::editActionData()
{
  WinnerListViewItem *item=(WinnerListViewItem *)selectedItem();
  if(item==NULL) {
    return;
  }
  EditAction *action=
    new EditAction(CallerIdDetail::simplifiedNumber(item->pnum()),
		   winner_showcode,item->detailId());
  action->exec();
  delete action;
}


void WinnerListView::showDetailData()
{
  WinnerListViewItem *item=(WinnerListViewItem *)selectedItem();
  if(item==NULL) {
    return;
  }
  ShowDetail *detail=new ShowDetail(winner_showcode,item->detailId());
  detail->exec();
  delete detail;
}


void WinnerListView::contentsMousePressEvent(QMouseEvent *e)
{
  Q3ListView::contentsMousePressEvent(e);
  winner_menu_item=(WinnerListViewItem *)selectedItem();
  switch(e->button()) {
      case Qt::RightButton:
	winner_menu->setGeometry(winner_parent->geometry().x()+
				 geometry().x()+e->pos().x()+2,
				 winner_parent->geometry().y()+
				 geometry().y()+e->pos().y()+
				 header()->geometry().height()+2,
				 winner_menu->sizeHint().width(),
				 winner_menu->sizeHint().height());
	winner_menu->exec();
	break;

      default:
	e->ignore();
	break;
  }
}
