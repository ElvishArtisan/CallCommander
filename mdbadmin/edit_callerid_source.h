// edit_callerid_source.h
//
// Edit a CallCommander CallerId Source.
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

#ifndef EDIT_CALLERID_SOURCE_H
#define EDIT_CALLERID_SOURCE_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QComboBox>

#include "edit_ttysettings.h"

class EditCallerIdSource : public QDialog
{
 Q_OBJECT
 public:
  EditCallerIdSource(const QString &hostname,int host_id,
		     QWidget *parent=0,const char *name=0);
  ~EditCallerIdSource();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void settingsData();
  void okData();
  void cancelData();

 private:
  QLineEdit *edit_ttyport_edit;
  QString edit_hostname;
  int edit_host_id;
  TtySettings edit_settings;
};


#endif  // EDIT_CALLERID_SOURCE_H
