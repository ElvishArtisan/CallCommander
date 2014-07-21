//   mltransportbutton.h
//
//   An audio transport button widget.
//
//   (C) Copyright 2002 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mltransportbutton.h,v 1.3 2010/11/22 13:11:40 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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
//

#ifndef MLTRANSPORTBUTTON_H
#define MLTRANSPORTBUTTON_H

#include <QtCore/QtCore>
#include <QtGui/QtGui>
#include <Qt3Support/Qt3Support>

#define MLTRANSPORTBUTTON_DEFAULT_ON_COLOR Qt::green

class MLTransportButton : public QPushButton
{
  Q_OBJECT
 public:
  enum TransType {Play=0,Stop=1,Record=2,FastForward=3,Rewind=4,Eject=5,
		  Pause=6,PlayFrom=7,PlayBetween=8,Loop=9,Up=10,Down=11,
		  PlayTo=12};
  enum TransState {On=0,Off=1,Flashing=2}; 
  MLTransportButton(MLTransportButton::TransType type,QWidget *parent,
		    const char *name);
  MLTransportButton::TransType getType() const;
  void setType(MLTransportButton::TransType type);
  QColor onColor() const;
  void setOnColor(QColor color);
  QColor accentColor() const;
  void setAccentColor(QColor color);
  void setState(MLTransportButton::TransState state);
  
 public slots:
  void on();
  void off();
  void flash();

 protected:
  virtual void resizeEvent(QResizeEvent *);
  virtual void enabledChange(bool oldEnabled);
  
 private slots:
  void flashClock();

 private:
 void updateCaps();
 void drawMask(QPixmap *cap);
 void drawOnCap();
 void drawOffCap();
 void flashOn();
 void flashOff();
 bool flash_state;
 MLTransportButton::TransType button_type;
 MLTransportButton::TransState button_state;
 QColor on_color;
 QColor accent_color;
 QPixmap *on_cap;
 QPixmap *off_cap;
 QTimer *flash_timer;
};


#endif  // MLTRANSPORTBUTTON_H
