// add_callerid_source.h
//
// Add a CallCommander CallerId Source.
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

#ifndef ADD_CALLERID_SOURCE_H
#define ADD_CALLERID_SOURCE_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QSpinBox>

class AddCallerIdSource : public QDialog
{
 Q_OBJECT
 public:
  AddCallerIdSource(QString *hostname,int *host_id,
		    QWidget *parent=0,const char *name=0);
  ~AddCallerIdSource();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void okData();
  void cancelData();

 private:
  QLineEdit *add_hostname_edit;
  QSpinBox *add_hostid_spin;
  QString *add_hostname;
  int *add_host_id;
};


#endif  // ADD_CALLERID_SOURCE_H
