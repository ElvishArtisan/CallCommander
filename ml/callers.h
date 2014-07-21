// callers.h
//
// Caller dialog for the Mithlond Project. 
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

#ifndef CALLERS_H
#define CALLERS_H

#include <QtGui/QDialog>
#include <QtGui/QLineEdit>
#include <QtGui/QPixmap>
#include <QtGui/QRadioButton>
#include <QtGui/QPaintEvent>

#include <bus_driver.h>
#include <call_meta_data.h>

#include "comment.h"

class Callers : public QDialog
{
  Q_OBJECT
  public:
   enum Action {Hold=0,Screened=1,Next=2}; 
   Callers(bool extra,bool cid_active,int line,CallMetaData *data,
	   BusDriver *driver,QWidget *parent=0);
   QSize sizeHint() const;
   QSizePolicy sizePolicy() const;

  protected:
   void paintEvent(QPaintEvent *);

  private slots:
   void cancelData();
   void okData();
   void screenData();
   void holdData();
   void awardData();

  private:
   void WriteCallData(CallMetaData *data);
   QString DateTimeString(const QDateTime &datetime,const QDateTime &now);
   BusDriver *call_driver;
   int call_line;
   CallMetaData *call_data;
   QLineEdit *call_name;
   QLineEdit *call_age;
   QLineEdit *call_city;
   QLineEdit *call_state;
   QLineEdit *call_zip;
   QLineEdit *call_phone;
   QLineEdit *call_station;
   QRadioButton *call_gender_male;
   QRadioButton *call_gender_female;
   Comment *call_comment;
   QRadioButton *call_cell_yes;
   QRadioButton *call_quality_verypoor;
   QRadioButton *call_quality_poor;
   QRadioButton *call_quality_fair;
   QRadioButton *call_quality_good;
   QRadioButton *call_quality_excellent;
};


#endif

