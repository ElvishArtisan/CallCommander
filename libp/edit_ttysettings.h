// edit_ttysettings.h
//
// Edit TTY Parameter Settings.
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

#ifndef EDIT_TTYSETTINGS_H
#define EDIT_TTYSETTINGS_H

#include <QtGui/QDialog>
#include <QtGui/QComboBox>

#include <ttysettings.h>

class EditTtySettings : public QDialog
{
  Q_OBJECT
 public:
  EditTtySettings(TtySettings *settings,QWidget *parent=0,const char *name=0);
  ~EditTtySettings();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void okData();
  void cancelData();

 private:
  QComboBox *edit_speed_box;
  QComboBox *edit_wordlength_box;
  QComboBox *edit_stopbits_box;
  QComboBox *edit_parity_box;
  TtySettings *edit_settings;
};


#endif  // EDIT_TTYSETTINGS_H
