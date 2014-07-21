/*   winner_listviewitem.h
 *
 *   A QListViewItem class for the List Winners dialog
 *
 *   (C) Copyright 2002-2006,2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef WINNER_LISTVIEWITEM_H
#define WINNER_LISTVIEWITEM_H

#include <ml_listviewitem.h>

class WinnerListViewItem : public MlListViewItem
{
 public:
  WinnerListViewItem(Q3ListView *parent);
  ~WinnerListViewItem();
  unsigned detailId() const;
  void setDetailId(unsigned id);
  QString pnum() const;
  void setPnum(const QString &pnum);

 private:
  unsigned item_detail_id;
  QString item_pnum;
};


#endif  // WINNER_LISTVIEWITEM_H
