// keybox.h
//
// DTMF Keypad dialog for the Mithlond Project. 
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

#ifndef KEYPAD_H
#define KEYPAD_H

#include <QtGui/QDialog>
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>
#include <Qt3Support/q3multilineedit.h>
#include <QtGui/qpixmap.h>

#include <bus_driver.h>

class Keybox : public QDialog
{
  Q_OBJECT
 public:
  Keybox(BusDriver *driver,const QString &area_code,int line,
	 QWidget *parent=0,const char *name=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void sendDigitData(int id);
  void redialData();
  void redialListData();
  void closeData();
  
 private:
  void DialNumber();
  void SendNumber(QString pnum);
  BusDriver *key_driver;
  int key_line;
  QLineEdit *key_number_edit;
  QPushButton *close_button;
  QString key_area_code;
};


#endif  // KEYBOX_H

