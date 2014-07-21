// mcs_widget.h
//
// A Qt-based application for automatic call recording
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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



#ifndef MSC_WIDGET_H
#define MSC_WIDGET_H

#include <QtGui/QWidget>
#include <QtGui/QPushButton>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtCore/QTimer>

#include <mlhpirecordstream.h>
#include <mlplaymeter.h>
#include <mltransportbutton.h>

#include <call_meta_data.h>
#include <telos2101_driver.h>
#include <connection_data.h>

/*
 * Application Defaults
 */
#define DEFAULT_CONSOLE 8
#define DEFAULT_MODE "Talent"
#define DEFAULT_GROUP 0
#define DEFAULT_DISABLE_WATCHDOG false
#define DEFAULT_ASK_HOST true
#define DEFAULT_AUDIO_ROOT "/var/snd"
#define DEFAULT_ALSA_DEVICE "plughw"
#define DEFAULT_SAMPLE_RATE 44100
#define DEFAULT_CHANNELS 2
#define DEFAULT_FORMAT "pcm16"
#define DEFAULT_MAIN_PATTERN "main-%d-%m-%Y-%k-%M-%S.wav"
#define DEFAULT_CONF_PATTERN "conf-%d-%m-%Y-%k-%M-%S.wav"
#define DEFAULT_CARD 0
#define DEFAULT_STREAM 0
#define DEFAULT_PORT 0
#define DEFAULT_MAIN_POSTCOMMAND ""
#define DEFAULT_CONF_POSTCOMMAND ""
#define DEFAULT_MAIN_MAXLENGTH "00:00:00"
#define DEFAULT_CONF_MAXLENGTH "00:00:00"
#define DEFAULT_MAIN_LEVEL 400
#define DEFAULT_CONF_LEVEL 400
#define DEFAULT_MAIN_INPUT_TYPE "analog"
#define DEFAULT_CONF_INPUT_TYPE "analog"

/*
 * Global Values
 */
#define MAX_LINES 12
#define UNAVAIL_COLOR Qt::lightGray
#define IDLE_COLOR Qt::darkGray
#define OFF_HOOK_COLOR Qt::yellow
#define ON_AIR_COLOR Qt::red
#define ON_HOLD_COLOR Qt::green
#define RINGING_COLOR Qt::blue
#define BUSYOUT_COLOR Qt::magenta
#define UNKNOWN_STRING "unknown"
#define BUTTON_STRING "manual"
#define METER_UPDATE_INTERVAL 50

class McsWidget : public QWidget
{
  Q_OBJECT
 public:
  McsWidget(int bank,ConnectionData *data,BusDriver *driver,
	    QWidget *parent=0,const char *name=0);
  ~McsWidget();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;

 private slots:
  void mapperData(int);
  void lineStateData(unsigned id,BusDriver::LineState lstate,unsigned line,
		     int bank,int console);
  void receivedStringData(unsigned id,BusDriver::Element elem,unsigned line,
			  const QString &str);
  void recordData();
  void stopData();
  void recordedData();
  void stoppedData();
  void recordTimerData();
  void meterData();
  
 private:
  enum Format {PCM16=0,Layer2=2,Layer3=3};
  enum InputType {Analog=0,Aes3=1};
  void DeckStart(int line,int bank);
  void DeckStop(int line,int bank);
  void Init();
  QString WildcardDecode(QString spec,int line,QDateTime datetime,
			 bool manual=false);
  void ClearCaller(int);
  MLHPISoundCard *mcs_soundcard;
  MLHPIRecordStream *mcs_record;
  BusDriver *mcs_driver;
  MLTransportButton *mcs_record_button;
  MLTransportButton *mcs_stop_button;
  CallMetaData *mcs_caller_data[MAX_LINES];
  CallMetaData *mcs_caller_check_data;
  MLPlayMeter *mcs_left_meter;
  MLPlayMeter *mcs_right_meter;
  ConnectionData *mcs_connection;
  QPushButton *mcs_main_button[MAX_LINES];
  QLabel *mcs_label[MAX_LINES];
  int mcs_meter_height;
  QLineEdit *mcs_filename_edit;
  QLabel *mcs_timer_label;
  QTimer *mcs_timer_timer;
  bool mcs_line_active[MAX_LINES];
  bool mcs_manual_active;
  int mcs_bank;
  int mcs_record_time;
  QTimer *mcs_meter_timer;
  QString mcs_line_cmd;
};


#endif 

