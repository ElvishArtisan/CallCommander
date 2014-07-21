/*   winner_listviewitem.cpp
 *
 *   A QListViewItem class for the List Winners dialog
 *
 *   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QPainter>

#include "winner_listviewitem.h"

WinnerListViewItem::WinnerListViewItem(Q3ListView *parent) 
  : MlListViewItem(parent)
{
  item_detail_id=0;
  item_pnum="";
}


WinnerListViewItem::~WinnerListViewItem()
{
}


unsigned WinnerListViewItem::detailId() const
{
  return item_detail_id;
}


void WinnerListViewItem::setDetailId(unsigned id)
{
  item_detail_id=id;
}


QString WinnerListViewItem::pnum() const
{
  return item_pnum;
}


void WinnerListViewItem::setPnum(const QString &pnum)
{
  item_pnum=pnum;
}
