// select_callerid_source.h
//
// Select a CallCommander CallerID Source.
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

#ifndef SELECT_CALLERID_SOURCE_H
#define SELECT_CALLERID_SOURCE_H

#include <QtGui/QDialog>
#include <Qt3Support/q3listbox.h>
#include <QtGui/QPushButton>

class SelectCallerIdSource : public QDialog
{
 Q_OBJECT
 public:
  SelectCallerIdSource(const QString &hostname,int *host_id,
		      QWidget *parent=0,const char *name=0);
  ~SelectCallerIdSource();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void doubleClickedData(Q3ListBoxItem *item);
  void okData();
  void cancelData();

 private:
  Q3ListBox *select_source_list;
  QString select_hostname;
  int *select_host_id;
};


#endif  // SELECT_CALLERID_SOURCE_H
