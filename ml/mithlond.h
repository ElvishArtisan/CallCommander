// mithlond.h
//
// A Qt-based application for controlling broadcast telephone interfaces.
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

#ifndef MITHLOND_H
#define MITHLOND_H

#include <QtGui/QWidget>
#include <QtGui/QSizePolicy>
#include <QtGui/QPushButton>
#include <QtGui/QColor>
#include <QtCore/QString>
#include <QtGui/QComboBox>
#include <QtGui/QPixmap>
#include <QtCore/QTimer>
#include <QtGui/QPalette>
#include <Qt3Support/q3mainwindow.h>
#include <Qt3Support/q3process.h>
#include <QtGui/QMoveEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QLabel>
#include <QtGui/QMouseEvent>

#include <mlsegclock.h>
#include <mllabel.h>

#include <connection_data.h>
#include <call_meta_data.h>
#include <message_data.h>
#include <mldconfig.h>
#include <mlpushbutton.h>

#include <telos2101_driver.h>
#include <telos100_driver.h>
#include <ts612_driver.h>
#include <virtual_driver.h>
#include <asterisk_driver.h>
#include <client_data.h>

#include "callers.h"
#include "message.h"
#include "list_clients.h"
#include "keybox.h"
#include "line_warning.h"
#include "callinfo.h"
#include "line_button.h"

/*
 * Global Values
 */
#define MAX_LINES 12
#define FRAME_RATE 100
#define MODE_CHANGE_POLL_RATE 5000
#define REQUEST_METADATA_DELAY 5000
//#define DEMO_INTERVAL 15
#define ML_USAGE "[--dump-strings]\n--dump-strings\n     Dump raw message strings to standard output (useless except for\n     debugging).\n"

QString GetConfFile(int argc,char *argv[]);

class MainWidget : public Q3MainWindow
{
  Q_OBJECT
 public:
  MainWidget(QWidget *parent=0,const char *name=0,Qt::WFlags f=0);
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 protected:
  void closeEvent(QCloseEvent *e);
  void paintEvent(QPaintEvent *);
  void moveEvent(QMoveEvent *e);

 private slots:
  void errorData(unsigned id,BusDriver::Error err);
  void lineButtonData(int id);
  void holdData(int bank);
  void dropData(int bank);
  void connectedData(unsigned id,bool state);
  void connectionClosedData(unsigned id);
  void currentShowData(unsigned id,const QString &show_name,bool state);
  void currentConsoleData(unsigned id,int console,ConsoleData::Mode mode);
  void TickClock();
  void busyData();
  void dumpPressedData();
  void dumpReleasedData();
  void recordData();
  void nextData();
  void keypadData();
  void flashData();
  void screenData();
  void muteData();
  void confData();
  void vipData();
  void clientData();
  void messageData(QString msg);
  void clearedData();
  void touchedData();
  void callerBoxData(int line);
  void lineStateData(unsigned id,BusDriver::LineState lstate,
		     unsigned line,int bank,int console);
  void busyAllStateData(unsigned id,bool state);
  void dumpDelayStateData(unsigned id,bool state);
  void recordStateData(unsigned id,bool state);
  void screenStateData(unsigned id,bool state);
  void muteStateData(unsigned id,bool state);
  void conferenceStateData(unsigned id,bool state);
  void vipStateData(unsigned id,bool state);
  void receivedStringData(unsigned id,BusDriver::Element elem,unsigned line,
			  const QString &str);
  void segStartedData(int index);
  void metadataUpdateRequestedData(unsigned id,ClientData::ClientType type);
  void requestMetadataData();
  void demoTimeoutData();
  void spawnProcessExitedData();

 private:
  void mouseDoubleClickEvent(QMouseEvent *e);
  void ProcessLineButton(int line);
  void LoadConfig();
  void DrawCaps();
  QPixmap DrawRecordCap(QPainter *p,int angle);
  void UpdateTimer(int line,int value);
  void SendCallData(int line,bool=false);
  void ClearCallerBox(int);
  void UpdateCallerBox(int);
  void SetCaption();
  void UpdateDelayButton(int offset);
  ConnectionData *telos_connection;
  ConsoleData::Mode telos_director_mode;
  LineButton *telos_line[MAX_BANKS][12];
  BusDriver::LineState telos_status[MAX_LINES];
  int telos_frame;
  int telos_bank[MAX_LINES];
  QPushButton *telos_hold_button[MAX_BANKS];
  QPushButton *telos_drop_button[MAX_BANKS];
  QString telos_config;
  QBitmap telos_hold_icon;
  QBitmap telos_screen_icon;
  QBitmap telos_drop_icon;
  QTimer *clock;
  CallMetaData *telos_caller_data[MAX_LINES];
  CallMetaData *telos_caller_check_data;
  CallInfo *telos_caller_info[MAX_LINES];
  Message *telos_message_edit;
  MessageData *telos_message_data;
  QString telos_message_id;
  MLSegClock *telos_clock_segclock;
  MLSegList *telos_dynamic_seglist;
  QPushButton *telos_busy_button;
  QPixmap telos_busy_icon[2];
  MLPushButton *telos_dump_button;
  QPixmap telos_dump_icon;
  QPushButton *telos_record_button;
  QPixmap telos_record_icon[8];
  int telos_record_modulo;
  bool telos_record_running;
  int telos_record_frame;
  QPushButton *telos_next_button;
  QPixmap telos_next_icon;
  int telos_next_bank;
  QPushButton *telos_flash_button;
  bool telos_screen_state;
  QPushButton *telos_screen_button;
  bool telos_mute_state;
  QPushButton *telos_mute_button;
  bool telos_conf_state;
  QPushButton *telos_conf_button;
  bool telos_vip_state;
  QPushButton *telos_vip_button;
  QPixmap telos_keypad_icon;
  QPushButton *telos_keypad_button;
  Keybox *telos_keypad_dialog;
  Callers *telos_caller_dialog;
  LineWarning *telos_warning_box[MAX_LINES];
  QPixmap *telos_callcommander_map;
  QPushButton *telos_client_button;
  ListClients *telos_list_clients;
  QTimer *telos_metadata_timer;
  int telos_delay_offset;
  int telos_dial_line;
  QString telos_local_area_code;
  Q3Process *telos_spawn_process;
  bool telos_dump_strings;
};


#endif  // MITHLOND_H
