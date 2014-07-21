// edit_module.h
//
// Edit a CallCommander Logic Module Configuration.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_MODULE_H
#define EDIT_MODULE_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QCheckBox>

class EditModule : public QDialog
{
 Q_OBJECT
 public:
  EditModule(int id,QWidget *parent=0,const char *name=0);
  ~EditModule();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void hostnameTextChangedData(const QString &str);
  void connectionSelectedData();
  void modulePathSelectedData();
  void logPathSelectedData();
  void okData();
  void cancelData();

 private:
  QString GetCanonicalName() const;
  QString edit_local_hostname;
  QLineEdit *edit_description_edit;
  QLineEdit *edit_hostname_edit;
  QCheckBox *edit_realtime_check;
  QLineEdit *edit_connection_edit;
  QLineEdit *edit_module_path_edit;
  QLineEdit *edit_module_args_edit;
  QPushButton *edit_module_path_button;
  QLineEdit *edit_log_path_edit;
  QPushButton *edit_log_path_button;
  int edit_id;
};


#endif  // EDIT_MODULE_H
