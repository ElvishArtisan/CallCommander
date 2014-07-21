// add_map_line.h
//
// Edit a CallCommander CID Line Map.
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

#ifndef ADD_MAP_LINE_H
#define ADD_MAP_LINE_H

#include <QtGui/QDialog>
#include <QtGui/QSpinBox>
#include <QtGui/QCloseEvent>

class AddMapLine : public QDialog
{
 Q_OBJECT
 public:
 AddMapLine(int *src_line,int *dest_line,bool add_mode,
	     QWidget *parent=0,const char *name=0);
  ~AddMapLine();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 protected:
  void closeEvent(QCloseEvent *e);

 private:
  QSpinBox *add_source_spin;
  QSpinBox *add_destination_spin;
  int *add_source_line;
  int *add_destination_line;
};


#endif  // ADD_MAP_LINE_H


