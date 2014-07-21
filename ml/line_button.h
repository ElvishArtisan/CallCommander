// line_button.h
//
// Line button for CallCommander
//
//   (C) Copyright 2002-2011 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef LINE_BUTTON_H
#define LINE_BUTTON_H

#include <QtGui/QPushButton>
#include <QtGui/QBitmap>
#include <QtGui/QPixmap>

#include <bus_driver.h>
#include <mlconfig.h>

class LineButton : public QPushButton
{
  Q_OBJECT
 public:
  LineButton(int line,int bank,MlConfig *config,QWidget *parent=0);
  void setState(BusDriver::LineState state);
  void setFrame(int frame);

 private:
  void UpdateCap();
  void InitIcons();
  void MakeIcon(QPainter *p,QBitmap *map,int offset,int frame,
		const QColor &color);
  BusDriver::LineState line_state;
  int line_line;
  int line_bank;
  MlConfig *line_config;
  QPixmap *line_icons[13][3];
  int line_index;
  int line_frame;
  QFont line_font;
};


#endif  // LINE_BUTTON_H
