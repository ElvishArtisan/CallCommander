// askbox.h
//
// Start-up dialog for the Mithlond Project. 
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef ASKBOX_H
#define ASKBOX_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3multilineedit.h>
#include <QtGui/QPixmap>
#include <QtGui/QComboBox>
#include <QtGui/QSpinBox>
#include <QtGui/QPaintEvent>
#include <askdata.h>

#define MAX_DESKTOP_DIRECTORS 8
#define MAX_VIRTUAL_DIRECTORS 8



class AskBox : public QDialog
{
  Q_OBJECT
 public:
  AskBox(AskData *data,QWidget *parent=0,const char *name=0);
  ~AskBox();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 protected:
  void paintEvent(QPaintEvent *);

 private slots:
  void cancelData();
  void okData();

 private:
  AskData *ask_data;
  QLineEdit *ask_hostname_edit;
  QLineEdit *ask_password_edit;
  QSpinBox *ask_port_spin;
  QComboBox *ask_group_box;
  QComboBox *ask_console_box;
};


#endif

