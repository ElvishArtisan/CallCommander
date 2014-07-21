// callinfo.h
//
// Caller Information widget for CallCommander. 
//
//   (C) Copyright 2010 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef CALLINFO_H
#define CALLINFO_H

#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QtGui/QPixmap>

#include <call_meta_data.h>

#include "callcomment.h"
#include "globals.h"

class CallInfo : public QWidget
{
  Q_OBJECT;
 public:
  CallInfo(QWidget *parent=0);
  /*
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  */
  void setMetadata(CallMetaData *data);
  void setTimer(int value);
  void clear();

 private:
  QLabel *call_label[MlConfig::CallerSize];
  CallComment *call_comment;
  QPixmap call_cellphone_icon;
  QPixmap call_quality_icons[6];
};


#endif  // CALLINFO_H
