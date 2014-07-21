// mithlond.cpp
//
// A Qt-based application for controling broadcast telephone interfaces.
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
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

#include <QtGui/QMessageBox>
#include <QtGui/QPainter>
#include <QtGui/QComboBox>
#include <Qt3Support/q3listbox.h>
#include <Qt3Support/q3filedialog.h>
#include <QtGui/qfont.h>
#include <Qt3Support/q3pointarray.h>
#include <QtCore/QEvent>
#include <Qt3Support/q3accel.h>
#include <QtCore/qsignalmapper.h>
#include <QtCore/QSettings>
#include <QtGui/QApplication>
#include <QtCore/QStringList>

#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mlconf.h>
#include <mlprofile.h>

#include <client_data.h>
#include <askbox.h>
#include <cmdswitch.h>

#include "defaults.h"
#include "globals.h"
#include "mithlond.h"

#ifdef WIN32
#include <windows.h>
#else
#include <sys/wait.h>
#endif  // WIN32

//
// Globals
//
MlConfig *ml_config;
NumberList *number_list;
BusDriver *telos_driver;
QString spawn_cmd;
#ifndef WIN32
pid_t spawn_pid;
#endif  // WIN32

//
// Icons
//
#include "../icons/callcommander-22x22.xpm"


QString GetConfFile(int argc,char *argv[])
{
  QString config;
  bool file_found=false;

  //
  // Determine Which Configuration File to Use
  //
  // First, the command line argument
  //
  if(argc>1) {
    QFile *file=new QFile(QString(qApp->argv()[qApp->argc()-1]));
    if(file->exists()) {
      config=argv[argc-1];
      file_found=true;
    }
    delete file;
  }

  //
  // Next, the .mlrc File
  //
  if(!file_found) {
    bool found;
    QString home=MLGetHomeDir(&found);
    if(found) {
      if(QFile(QString().sprintf("%s/.mlrc",(const char *)home)).exists()) {
	config=QString().sprintf("%s/.mlrc",(const char *)home);
	file_found=true;
      }
    }
  }

  //
  // Next, the global configuration
  //
  if(!file_found) {
#ifdef WIN32
    QSettings settings;
    settings.setPath("SalemRadioLabs","CallCommander",QSettings::SystemScope);
    settings.insertSearchPath(QSettings::Windows,ML_REGISTRY_PATH);
    config=
      QString().sprintf("%s\\ml.ini",
			(const char *)settings.
			readEntry(ML_REGISTRY_INSTALL_DIR,"C:\\etc"));
    file_found=true;
#else
    config=DEFAULT_CONFIG_FILE;
#endif  // WIN32
  }

  //
  // Finally, look in the bundle (OS X only)
  //
#ifdef OS_X
  if(!file_found) {
    QString filename=
      QDir::currentPath()+"/CallScreener.app/Contents/Resources/ml.conf";
    if(QFile::exists(filename)) {
      config=filename;
      file_found=true;
    }
  }
#endif  // OS_X

  return config;
}


MainWidget::MainWidget(QWidget *parent,const char *name,Qt::WFlags f)
  :Q3MainWindow(parent,name,f)
{
  QFont font;
  QString label;
  telos_metadata_timer=NULL;
  telos_connection=new ConnectionData(ConnectionData::StorageFilesystem);
  telos_dial_line=-1;
  telos_frame=0;
  telos_dynamic_seglist=new MLSegList();
  telos_dynamic_seglist->addSegment(new MLSegment());
  telos_dump_strings=false;

  //
  // Process Command Line
  //
  CmdSwitch *cmd=new CmdSwitch(qApp->argc(),qApp->argv(),"ml",ML_USAGE);
  for(unsigned i=0;i<cmd->keys();i++) {
    if(cmd->key(i)=="--dump-strings") {
      telos_dump_strings=true;
      cmd->setProcessed(i,true);
    }
    if((!cmd->processed(i))&&(i<(cmd->keys()-1))) {
      fprintf(stderr,"ml: unknown option\n");
      exit(256);
    }
  }

  //
  // Create And Set Icon
  //
  telos_callcommander_map=new QPixmap(callcommander_xpm);
  setIcon(*telos_callcommander_map);

  //
  // Load the Redialer Number List
  //
  number_list=new NumberList();

  //
  // The Line Warning Dialogs
  //
  for(int i=0;i<MAX_LINES;i++) {
    telos_warning_box[i]=new LineWarning(this,"telos_warning_box");
  }

  LoadConfig();
  DrawCaps();
  telos_screen_state=false;
  telos_mute_state=false;
  telos_conf_state=false;
  telos_vip_state=false;
  telos_record_frame=0;
  telos_record_running=false;
  telos_director_mode=ConsoleData::Talent;

  setBackgroundColor(QColor(Qt::black));
  SetCaption();

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumWidth(sizeHint().width());
  setMaximumHeight(sizeHint().height());

  if(ml_config->keepOnTop()) {
    //setWFlags(Qt::WStyle_StaysOnTop);
  }

  //
  // The Bus Driver
  //
  switch(telos_connection->systemType()) {
      case ConnectionData::TypeAsterisk:
	telos_driver=new AsteriskDriver(0,this,"telos_driver");
	break;

      case ConnectionData::TypeTelos2101:
	telos_driver=new Telos2101Driver(0,this,"telos_driver");
	break;

      case ConnectionData::TypeTelos100Key:
	telos_driver=
	  new Telos100Driver(Telos100Driver::KeyType,0,this,"telos_driver");
	break;

      case ConnectionData::TypeTelos100Dim:
	telos_driver=
	  new Telos100Driver(Telos100Driver::DimType,0,this,"telos_driver");
	break;

      case ConnectionData::TypeTelos100OneXSix:
	telos_driver=
	  new Telos100Driver(Telos100Driver::OneXSixType,0,this,
			     "telos_driver");
	break;

      case ConnectionData::TypeGentnerTs612:
	telos_driver=new Ts612Driver(0,this,"telos_driver");
	break;

      case ConnectionData::TypeVirtual:
	telos_driver=new VirtualDriver(0,this,"telos_driver");
	break;

      case ConnectionData::TypeUnknown:
	QMessageBox::warning(this,tr("System Error"),
			     tr("Unknown System Type!"));
	exit(256);
  }
  telos_driver->setClientType(ClientData::TypeUser);
  telos_driver->setSegmentSource(ml_config->segmentList()->size()>0);
  telos_driver->setDumpStrings(telos_dump_strings);
  connect(telos_driver,SIGNAL(connected(unsigned,bool)),
	  this,SLOT(connectedData(unsigned,bool)));
  connect(telos_driver,SIGNAL(connectionClosed(unsigned)),
	  this,SLOT(connectionClosedData(unsigned)));
  connect(telos_driver,SIGNAL(currentShow(unsigned,const QString &,bool)),
	  this,SLOT(currentShowData(unsigned,const QString &,bool)));
  connect(telos_driver,SIGNAL(currentConsole(unsigned,int,ConsoleData::Mode)),
	  this,SLOT(currentConsoleData(unsigned,int,ConsoleData::Mode)));
					     
  connect(telos_driver,SIGNAL(error(unsigned,BusDriver::Error)),
	  this,SLOT(errorData(unsigned,BusDriver::Error)));
  connect(telos_driver,SIGNAL(currentLineState(unsigned,BusDriver::LineState,
					       unsigned,int,int)),this,
	  SLOT(lineStateData(unsigned,BusDriver::LineState,unsigned,int,int)));
  connect(telos_driver,SIGNAL(currentBusyAllState(unsigned,bool)),
	  this,SLOT(busyAllStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentDumpDelayState(unsigned,bool)),
	  this,SLOT(dumpDelayStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentRecordState(unsigned,bool)),
	  this,SLOT(recordStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentVipState(unsigned,bool)),
	  this,SLOT(vipStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentScreenState(unsigned,bool)),
	  this,SLOT(screenStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentConferenceState(unsigned,bool)),
	  this,SLOT(conferenceStateData(unsigned,bool)));
  connect(telos_driver,SIGNAL(currentMuteState(unsigned,bool)),
	  this,SLOT(muteStateData(unsigned,bool)));
  connect(telos_driver,
	  SIGNAL(receivedString(unsigned,BusDriver::Element,unsigned,
				const QString &)),
	  this,SLOT(receivedStringData(unsigned,BusDriver::Element,unsigned,
				       const QString &)));
  connect(telos_driver,
	  SIGNAL(metadataUpdateRequested(unsigned,ClientData::ClientType)),
	  this,
	  SLOT(metadataUpdateRequestedData(unsigned,ClientData::ClientType)));

  //
  // The List Clients Dialog
  //
  telos_list_clients=new ListClients(telos_driver,this);

  //
  // Line Buttons
  //
  QSignalMapper *mapper=new QSignalMapper(this,"mapper");
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(lineButtonData(int)));
  for(int i=0;i<MAX_BANKS;i++) {
    for(int j=0;j<MAX_LINES;j++) {
      telos_line[i][j]=NULL;
      if(ml_config->lineShow(i,j)) {
	telos_line[i][j]=new LineButton(j,i,ml_config,this);
	telos_line[i][j]->setGeometry(ml_config->linePosition(i,j).x(),
				      ml_config->linePosition(i,j).y(),80,80);
	telos_line[i][j]->setAccel(QKeySequence(ml_config->lineKey(i,j)));
	mapper->setMapping(telos_line[i][j],MAX_LINES*i+j);
	if(!ml_config->disableLineButtons()) {
	  connect(telos_line[i][j],SIGNAL(clicked()),mapper,SLOT(map()));
	}
      }
      else {
	telos_line[i][j]=NULL;
      }
    }
  }

  //
  // Hold Buttons
  //
  mapper=new QSignalMapper(this,"hold_button_mapper");
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(holdData(int)));
  for(int i=0;i<MAX_BANKS;i++) {
    if(ml_config->bankShowHoldButton(i)) {
      telos_hold_button[i]=new QPushButton(this,"telos_hold");
      telos_hold_button[i]->
	setGeometry(ml_config->bankHoldPosition(i).x(),
		    ml_config->bankHoldPosition(i).y(),80,80);
      telos_hold_button[i]->
	setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
      telos_hold_button[i]->setAutoFillBackground(true);
      telos_hold_button[i]->
	setStyleSheet(QString("background-color: ")+
		      ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
      telos_hold_button[i]->setPixmap(telos_hold_icon);
      telos_hold_button[i]->setAccel(QKeySequence(ml_config->bankHoldKey(i)));
      if(!ml_config->disableLineButtons()) {
	mapper->setMapping(telos_hold_button[i],i);
	connect(telos_hold_button[i],SIGNAL(clicked()),mapper,SLOT(map()));
      }
    }
    else {
      telos_hold_button[i]=NULL;
    }
  }

  //
  // Drop Buttons
  //
  mapper=new QSignalMapper(this,"drop_button_mapper");
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(dropData(int)));
  for(int i=0;i<MAX_BANKS;i++) {
    if(ml_config->bankShowDropButton(i)) {
      telos_drop_button[i]=new QPushButton(this,"telos_drop");
      telos_drop_button[i]->
	setGeometry(ml_config->bankDropPosition(i).x(),
		    ml_config->bankDropPosition(i).y(),80,80);
      telos_drop_button[i]->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
      telos_drop_button[i]->setAutoFillBackground(true);
      telos_drop_button[i]->
	setStyleSheet(QString("background-color: ")+
		      ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
      telos_drop_button[i]->setPixmap(telos_drop_icon);
      telos_drop_button[i]->setAccel(QKeySequence(ml_config->bankDropKey(i)));
      if(!ml_config->disableLineButtons()) {
	mapper->setMapping(telos_drop_button[i],i);
	connect(telos_drop_button[i],SIGNAL(clicked()),mapper,SLOT(map()));
      }
    }
    else {
      telos_drop_button[i]=NULL;
    }
  }

  //
  // Caller Data Boxes
  //
  Q3Accel *accel=new Q3Accel(this,"accel");
  if(!ml_config->disableLineBoxes()) {
    connect(accel,SIGNAL(activated(int)),this,SLOT(callerBoxData(int)));
  }
  for(int i=0;i<MAX_LINES;i++) {
    telos_caller_data[i]=new CallMetaData();
    telos_caller_info[i]=new CallInfo(this);
    telos_caller_info[i]->setGeometry(ml_config->lineCallerBoxWindow(i));
    if(!ml_config->lineShowCallerBox(i)) {
      telos_caller_info[i]->hide();
    }
  }
  telos_caller_check_data=new CallMetaData();

  //
  // The Message Box
  //
  if(ml_config->msgboxShow()) {
    telos_message_edit=new Message(ml_config->color(MlConfig::MessageBoxColor),
				   ml_config->msgboxHangTime(),
				   this,"telos_message_edit");
    telos_message_edit->setGeometry(ml_config->msgboxWindow());
    telos_message_edit->
      setPalette(ml_config->color(MlConfig::MessageBoxColor));
    telos_message_edit->setFont(ml_config->msgboxFont());
    telos_message_edit->setSendLines(ml_config->msgboxSendLines());
    telos_message_edit->setScrollback(ml_config->msgboxScrollbackLines());
    telos_message_edit->setNickname(ml_config->msgboxNickname());
    connect(telos_message_edit,SIGNAL(message(QString)),
	    this,SLOT(messageData(QString)));
    connect(telos_message_edit,SIGNAL(touched()),this,SLOT(touchedData()));
    connect(telos_message_edit,SIGNAL(cleared()),this,SLOT(clearedData()));
    telos_message_data=new MessageData();
    telos_message_id="";
  }
  else {
    telos_message_edit=NULL;
    telos_message_data=NULL;
  }

  //
  // Clock Widget
  //
  if(ml_config->clockShowClock()) {
    telos_clock_segclock=new MLSegClock(ml_config->clockOrientation(),this,
				       "telos_clock_segclock");
    if(ml_config->clockOrientation()==Qt::Horizontal) {
      telos_clock_segclock->
	setGeometry(ml_config->clockPosition().x(),
		    ml_config->clockPosition().y(),347,95);
    }
    else {
      telos_clock_segclock->setGeometry(ml_config->clockPosition().x(),
					ml_config->clockPosition().y(),
					172,170);
    }
    telos_delay_offset=ml_config->clockTimeOffset();
    telos_clock_segclock->setOffset(telos_delay_offset);
    telos_clock_segclock->loadSegList(ml_config->segmentList(),true);
    telos_clock_segclock->setShowHour(ml_config->clockShowHour());
    if(ml_config->clockMilitaryFormat()) {
      telos_clock_segclock->set24Hour();
    }
    else {
      telos_clock_segclock->set12Hour();
    }
    connect(telos_clock_segclock,SIGNAL(segStarted(int)),
	    this,SLOT(segStartedData(int)));
  }

  //
  // The Busy Button
  //
  if(ml_config->buttonShow(MlConfig::BusyButton)) {
    telos_busy_button=new QPushButton(this,"telos_busy_button");
    telos_busy_button->setGeometry(ml_config->
				   buttonPosition(MlConfig::BusyButton).x(),
				   ml_config->
				   buttonPosition(MlConfig::BusyButton).y(),
				   80,80);
    telos_busy_button->setPixmap(telos_busy_icon[0]);
    telos_busy_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_busy_button->setAutoFillBackground(true);
    telos_busy_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_busy_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::DumpButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_busy_button,SIGNAL(clicked()),this,SLOT(busyData()));
    }
  }
  else {
    telos_busy_button=NULL;
  }

  //
  // The Dump Button
  //
  if(ml_config->buttonShow(MlConfig::DumpButton)) {
    telos_dump_button=new MLPushButton(this,"telos_dump_button");
    telos_dump_button->
      setGeometry(ml_config->buttonPosition(MlConfig::DumpButton).x(),
		  ml_config->buttonPosition(MlConfig::DumpButton).y(),80,80);
    telos_dump_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_dump_button->setAutoFillBackground(true);
    telos_dump_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_dump_button->setFlashColor(Qt::red);
    telos_dump_button->setPixmap(telos_dump_icon);
    telos_dump_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::DumpButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_dump_button,SIGNAL(pressed()),
	      this,SLOT(dumpPressedData()));
      connect(telos_dump_button,SIGNAL(released()),
	      this,SLOT(dumpReleasedData()));
    }
  }
  else {
    telos_dump_button=NULL;
  }

  //
  // The Record Button
  //
  if(ml_config->buttonShow(MlConfig::RecordButton)) {
    telos_record_button=new QPushButton(this,"telos_record_button");
    telos_record_button->
      setGeometry(ml_config->buttonPosition(MlConfig::RecordButton).x(),
		  ml_config->buttonPosition(MlConfig::RecordButton).y(),80,80);
    telos_record_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_record_button->setAutoFillBackground(true);
    telos_record_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_record_button->setPixmap(telos_record_icon[0]);
    telos_record_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::RecordButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_record_button,SIGNAL(clicked()),this,SLOT(recordData()));
    }
  }
  else {
    telos_record_button=NULL;
  }

  //
  // The Next Button
  //
  if(ml_config->buttonShow(MlConfig::NextButton)) {
    telos_next_button=new QPushButton(this,"telos_next_button");
    telos_next_button->
      setGeometry(ml_config->buttonPosition(MlConfig::NextButton).x(),
		  ml_config->buttonPosition(MlConfig::NextButton).y(),80,80);
    telos_next_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_next_button->setAutoFillBackground(true);
    telos_next_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_next_button->setPixmap(telos_next_icon);
    telos_next_button->
      setPalette(ml_config->palette(MlConfig::IdleColor));
    telos_next_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::NextButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_next_button,SIGNAL(clicked()),this,SLOT(nextData()));
    }
  }
  else {
    telos_next_button=NULL;
  }

  //
  // The Keypad Button
  //
  if(ml_config->buttonShow(MlConfig::KeypadButton)) {
    telos_keypad_button=new QPushButton(this,"telos_keypad_button");
    telos_keypad_button->
      setGeometry(ml_config->buttonPosition(MlConfig::KeypadButton).x(),
		  ml_config->buttonPosition(MlConfig::KeypadButton).y(),80,80);
    telos_keypad_button->setPixmap(telos_keypad_icon);
#ifndef LINUX
    telos_keypad_button->setAutoFillBackground(true);
    telos_keypad_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_keypad_button->setPalette(ml_config->palette(MlConfig::IdleColor));
    telos_keypad_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::KeypadButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_keypad_button,SIGNAL(clicked()),this,SLOT(keypadData()));
    }
  }
  else {
    telos_keypad_button=NULL;
  }

  //
  // Generate the font for Gentner Buttons
  //
  font=QFont("Helvetica",18,QFont::Bold);
  font.setPixelSize(18);

  //
  // The Flash Button
  //
  if(ml_config->buttonShow(MlConfig::FlashButton)) {
    telos_flash_button=new QPushButton(this,"telos_flash_button");
    telos_flash_button->
      setGeometry(ml_config->buttonPosition(MlConfig::FlashButton).x(),
		  ml_config->buttonPosition(MlConfig::FlashButton).y(),80,80);
#ifndef LINUX
    telos_flash_button->setAutoFillBackground(true);
    telos_flash_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_flash_button->setPalette(ml_config->palette(MlConfig::IdleColor));
    telos_flash_button->setFont(font);
    telos_flash_button->setText("FLSH");
    telos_flash_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::FlashButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_flash_button,SIGNAL(clicked()),this,SLOT(flashData()));
    }
  }
  else {
    telos_flash_button=NULL;
  }

  //
  // The Screen Button
  //
  if(ml_config->buttonShow(MlConfig::ScreenButton)) {
    telos_screen_button=new QPushButton(this,"telos_screen_button");
    telos_screen_button->
      setGeometry(ml_config->buttonPosition(MlConfig::ScreenButton).x(),
		  ml_config->buttonPosition(MlConfig::ScreenButton).y(),80,80);
    telos_screen_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_screen_button->setAutoFillBackground(true);
    telos_screen_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_screen_button->setFont(font);
    telos_screen_button->setText("SCRN");
    telos_screen_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::ScreenButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_screen_button,SIGNAL(clicked()),this,SLOT(screenData()));
    }
  }
  else {
    telos_screen_button=NULL;
  }

  //
  // The Mute Button
  //
  if(ml_config->buttonShow(MlConfig::MuteButton)) {
    telos_mute_button=new QPushButton(this,"telos_mute_button");
    telos_mute_button->
      setGeometry(ml_config->buttonPosition(MlConfig::MuteButton).x(),
		  ml_config->buttonPosition(MlConfig::MuteButton).y(),80,80);
    telos_mute_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_mute_button->setAutoFillBackground(true);
    telos_mute_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_mute_button->setFont(font);
    telos_mute_button->setText("MUTE");
    telos_mute_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::MuteButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_mute_button,SIGNAL(clicked()),this,SLOT(muteData()));
    }
  }
  else {
    telos_mute_button=NULL;
  }

  //
  // The Conf Button
  //
  if(ml_config->buttonShow(MlConfig::ConfButton)) {
    telos_conf_button=new QPushButton(this,"telos_conf_button");
    telos_conf_button->
      setGeometry(ml_config->buttonPosition(MlConfig::ConfButton).x(),
		  ml_config->buttonPosition(MlConfig::ConfButton).y(),80,80);
    telos_conf_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_conf_button->setAutoFillBackground(true);
    telos_conf_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_conf_button->setFont(font);
    telos_conf_button->setText("CONF");
    telos_conf_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::ConfButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_conf_button,SIGNAL(clicked()),this,SLOT(confData()));
    }
  }
  else {
    telos_conf_button=NULL;
  }

  //
  // The Vip Button
  //
  if(ml_config->buttonShow(MlConfig::VipButton)) {
    telos_vip_button=new QPushButton(this,"telos_vip_button");
    telos_vip_button->
      setGeometry(ml_config->buttonPosition(MlConfig::VipButton).x(),
		  ml_config->buttonPosition(MlConfig::VipButton).y(),80,80);
    telos_vip_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_vip_button->setAutoFillBackground(true);
    telos_vip_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_vip_button->setFont(font);
    telos_vip_button->setText("VIP");
    telos_vip_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::VipButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_vip_button,SIGNAL(clicked()),this,SLOT(vipData()));
    }
  }
  else {
    telos_vip_button=NULL;
  }

  //
  // The Client (Who's Here) Button
  //
  if(ml_config->buttonShow(MlConfig::ClientButton)) {
    telos_client_button=new QPushButton(this,"telos_client_button");
    telos_client_button->
      setGeometry(ml_config->buttonPosition(MlConfig::ClientButton).x(),
		  ml_config->buttonPosition(MlConfig::ClientButton).y(),80,80);
    telos_client_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
    telos_client_button->setAutoFillBackground(true);
    telos_client_button->
      setStyleSheet(QString("background-color: ")+
		    ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
    telos_client_button->setFont(font);
    telos_client_button->setText("Who's\nHere?");
    telos_client_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::ClientButton)));
    if(!ml_config->disableLineButtons()) {
      connect(telos_client_button,SIGNAL(clicked()),this,SLOT(clientData()));
    }
  }
  else {
    telos_client_button=NULL;
  }

  //
  // Initialize Line Status
  //
  for(int i=0;i<MAX_LINES;i++) {
    telos_status[i]=BusDriver::LineInactive;
  }

  setGeometry(ml_config->window());

  //
  // Spawn Process
  //
  if(ml_config->spawnProcess().isEmpty()) {
    telos_spawn_process=NULL;
  }
  else {
    telos_spawn_process=new Q3Process(this,"telos_spawn_process");
    connect(telos_spawn_process,SIGNAL(processExited()),
	    this,SLOT(spawnProcessExitedData()));
    QString cmd;
    for(int i=0;i<ml_config->spawnProcess().length();i++) {
      if(ml_config->spawnProcess().at(i)==' ') {
	if(!cmd.isEmpty()) {
          telos_spawn_process->addArgument(cmd);
        }
        cmd="";
      }
      else {
        cmd+=ml_config->spawnProcess().at(i);
      }
    }
    if(!cmd.isEmpty()) {
      telos_spawn_process->addArgument(cmd);
    }
    telos_spawn_process->launch(QString());
  }

  //
  // Start the Connection
  //
  switch(telos_connection->systemType()) {
    case ConnectionData::TypeAsterisk:
    case ConnectionData::TypeTelos2101:
    case ConnectionData::TypeVirtual:
      telos_driver->connectToHost(telos_connection->hostName(),
				  telos_connection->userName(),
				  telos_connection->userPassword(),
				  telos_connection->tcpPort());
      break;

    case ConnectionData::TypeTelos100Key:
    case ConnectionData::TypeTelos100Dim:
    case ConnectionData::TypeTelos100OneXSix:
    case ConnectionData::TypeGentnerTs612:
      telos_driver->connectToSerial(telos_connection->ttyPort());
      break;

    case BusDriver::UnknownConnection:
      QMessageBox::warning(this,tr("System Error"),
			   tr("Unknown System Type!"));
      exit(256);
  }
}


QSize MainWidget::sizeHint() const
{
  return ml_config->window().size();
}


QSizePolicy MainWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void MainWidget::closeEvent(QCloseEvent *e)
{
  if(telos_spawn_process!=NULL) {
    telos_spawn_process->tryTerminate();
  }
  telos_driver->disconnect();
  qApp->processEvents();
  exit(0);
}


void MainWidget::paintEvent(QPaintEvent *paintevent)
{
  //
  // Caller Box Background
  //
  QPainter *p=new QPainter(this);
  for(int i=0;i<MAX_LINES;i++) {
    if(ml_config->lineShowCallerBox(i)) {
      if(paintevent->rect().intersects(ml_config->lineCallerBoxWindow(i))) {
	p->setPen(QColor(Qt::black));
	p->fillRect(ml_config->lineCallerBoxWindow(i).x(),
		    ml_config->lineCallerBoxWindow(i).y(),
		    ml_config->callerSize().width(),
		    ml_config->callerSize().height(),
		    ml_config->color(MlConfig::CallerBoxColor));
	p->drawRect(ml_config->lineCallerBoxWindow(i).x(),
		    ml_config->lineCallerBoxWindow(i).y(),
		    ml_config->callerSize().width(),
		    ml_config->callerSize().height());
      }
    }
  }
  p->end();
}


void MainWidget::moveEvent(QMoveEvent *e)
{
  for(int i=0;i<MAX_LINES;i++) {
    if(telos_warning_box[i]->isVisible()) {
      telos_warning_box[i]->
	setGeometry(ml_config->lineCallerBoxWindow(i).x()+e->pos().x()+
		    frameGeometry().x()-geometry().x(),
		    ml_config->lineCallerBoxWindow(i).y()+e->pos().y()+
		    frameGeometry().y()-geometry().y(),
		    ml_config->callerSize().width(),
		    ml_config->callerSize().height());
    }
  }
  Q3MainWindow::moveEvent(e);
}


void MainWidget::errorData(unsigned id,BusDriver::Error err)
{
  switch(err) {
      case BusDriver::NoError:
	break;

      case BusDriver::HostNotFound:
	break;

      case BusDriver::ConnectionRefused:
	QMessageBox::warning(this,"Connection Refused",
			     "The connection was refused by the server!");
	qApp->quit();
	break;

      case BusDriver::ReadError:
	QMessageBox::warning(this,"Read Error","Socket Read Error!");
	break;

      default:
	QMessageBox::warning(this,"Unknown Error",
			     "Unrecoginized Socket Error!");
	break;
  }
}


void MainWidget::lineButtonData(int id)
{
  ProcessLineButton(id);
}


void MainWidget::messageData(QString msg)
{
  if(ml_config->msgboxShow()) {
    telos_message_edit->
      setMessage(telos_message_edit->nickname()+QString("> ")+msg);
    telos_message_data->clear();
    telos_message_data->setNick(telos_message_edit->nickname());
    telos_message_data->setMessage(msg);
    telos_message_data->sendData(telos_driver);
  }
}


void MainWidget::clearedData()
{
  telos_driver->sendString(BusDriver::ClearMessageGlobalElement,0,"clear");
}


void MainWidget::touchedData()
{
  telos_driver->sendString(BusDriver::LookupGlobalElement,0,"lookup");
}


void MainWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
  if(!ml_config->disableLineBoxes()) {
    for(int i=0;i<MAX_LINES;i++) {
      if(ml_config->lineShowCallerBox(i)) {
	if((e->x()>ml_config->lineCallerBoxWindow(i).x())&&
	   (e->x()<ml_config->lineCallerBoxWindow(i).x()+
	    ml_config->callerSize().width())&&
	   (e->y()>ml_config->lineCallerBoxWindow(i).y())&&
	   (e->y()<ml_config->lineCallerBoxWindow(i).y()+
	    ml_config->callerSize().height()))
	{
	  callerBoxData(i);
	}
      }
    }
  }
}


void MainWidget::callerBoxData(int line)
{
  int dialog_exit;

  if(telos_status[line]!=BusDriver::LineInactive) {
    bool cid_active=false;
    for(unsigned i=0;i<telos_driver->clientList()->size();i++) {
      if(telos_driver->clientList()->at(i)->clientType()==
	 ClientData::TypeCidMonitor) {
	cid_active=true;
      }
    }
    telos_caller_dialog=new Callers(false,cid_active,line,
				    telos_caller_data[line],telos_driver,this);
    dialog_exit=telos_caller_dialog->exec();
    if(dialog_exit!=1) {
      UpdateCallerBox(line);
      SendCallData(line);
    }
    switch(telos_status[line]) {
	case BusDriver::LineHold:
	case BusDriver::LineScreenedHold:
	case BusDriver::LineNext:
	  if(telos_status[line]!=BusDriver::LineOnAir) {
	    UpdateTimer(line,telos_caller_data[line]->
			element(BusDriver::HoldTimerElement).toInt());
	  }
	  break;
	  
	default:
	  break;
    }
    delete telos_caller_dialog;
    telos_caller_dialog=NULL;
  }
}


void MainWidget::ProcessLineButton(int line)
{
  int real_line=line%12;
  int bank=line/MAX_LINES;

  switch(telos_status[real_line]) {
      case BusDriver::LineLocked:
	if(telos_bank[real_line]==bank) {
	  telos_driver->selectLine(real_line,telos_connection->console(),bank);
	}
	break;

      case BusDriver::LineIdle:
	telos_dial_line=real_line;

      case BusDriver::LineOnAir:
      case BusDriver::LineHold:
      case BusDriver::LineScreenedHold:
      case BusDriver::LineNext:
      case BusDriver::LineRinging:
	telos_driver->selectLine(real_line,telos_connection->console(),bank);
	break;
	
      default:
	break;
  }
}


void MainWidget::holdData(int bank)
{
  telos_driver->hold(bank);
}
 

void MainWidget::dropData(int bank)
{
  telos_driver->drop(bank);
}


void MainWidget::connectedData(unsigned id,bool state)
{
  if(state) {
    telos_driver->connectToShow(telos_connection->showName(),
				telos_connection->showPassword(),
				telos_connection->location());
  }
  else {
    QMessageBox::warning(this,tr("Error"),
			 tr("Invalid Login!\nUnable to connect."),
			 1,0,0);
    qApp->quit();
  }
}


void MainWidget::connectionClosedData(unsigned id)
{
}


void MainWidget::currentShowData(unsigned id,const QString &show_name,
				 bool state)
{
  if(state) {
    telos_driver->connectToConsole(telos_connection->console(),
				   telos_connection->consoleName());
    if(!ml_config->disableStatusSync()) {
      telos_driver->requestLineState();
    }
  }
  else {
    QMessageBox::warning(this,tr("Error"),
	   tr("The specified Show does not exist!\nUnable to connect."));
    exit(256);
  }
}


void MainWidget::currentConsoleData(unsigned id,int console,
				    ConsoleData::Mode mode)
{
  if(console!=telos_connection->console()) {
    return;
  }
  if(mode==ConsoleData::Unavailable) {
    QMessageBox::warning(this,tr("Invalid Console"),
			 tr("The specified Console doesn't exist!"));
    exit(256);
  }
  telos_director_mode=mode;
  SetCaption();

  if(telos_hold_button[1]!=NULL) {
    switch(mode) {
      case ConsoleData::Talent:
      case ConsoleData::Unavailable:
	telos_hold_button[1]->setPixmap(telos_hold_icon);
	break;

      case ConsoleData::Producer:
	telos_hold_button[1]->setPixmap(telos_screen_icon);
	break;
    }
  }

  if(telos_metadata_timer==NULL) {
    telos_metadata_timer=new QTimer(this,"telos_metadata_timer");
    connect(telos_metadata_timer,SIGNAL(timeout()),
	    this,SLOT(requestMetadataData()));
    telos_metadata_timer->start(REQUEST_METADATA_DELAY,true);
  }
}


void MainWidget::lineStateData(unsigned id,BusDriver::LineState lstate,
			       unsigned line,int bank,int console)
{
  // printf("lineStateData:  lstate: %d  line: %d  bank: %d  console: %d\n",lstate,line,bank,console);
  if((telos_status[line]==lstate)&&(telos_bank[line]==bank)) {
    return;
  }
  switch(lstate) {
      case BusDriver::LineIdle:
	telos_caller_data[line]->clear();
	ClearCallerBox(line);
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineIdle);
	  }
	}
	telos_caller_data[line]->setElement(BusDriver::AirTimerElement,0);
	telos_caller_data[line]->setElement(BusDriver::HoldTimerElement,0);
	telos_caller_data[line]->setElement(BusDriver::RingTimerElement,0);
	telos_warning_box[line]->hide();
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;

      case BusDriver::LineInactive:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineInactive);
	  }
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;

      case BusDriver::LineElsewhere:
	switch(telos_status[line]) {
	    case BusDriver::LineIdle:
	    case BusDriver::LineHold:
	    case BusDriver::LineScreenedHold:
	    case BusDriver::LineRinging:
	    case BusDriver::LineNext:
	      for(int i=0;i<MAX_BANKS;i++) {
		if(telos_line[i][line]!=NULL) {
		  telos_line[i][line]->setState(BusDriver::LineElsewhere);
		}
	      }
	      break;

	    default:
	      break;
	}
	if(((int)line==telos_dial_line)&&(bank==0)) {
	  telos_dial_line=-1;
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	break;
	
      case BusDriver::LineOnAir:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    if(i==bank) {
	      telos_line[i][line]->setState(BusDriver::LineOnAir);
	    }
	    else {
	      telos_line[i][line]->setState(BusDriver::LineElsewhere);
	    }
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::AirTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	break;
	
      case BusDriver::LineLocked:
	for(int i=0;i<MAX_BANKS;i++) {
	  if((telos_line[i][line]!=NULL)&&(i==bank)) {
	    telos_line[i][line]->setState(BusDriver::LineLocked);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::AirTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	break;
	
      case BusDriver::LineSpeaker:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineSpeaker);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::HoldTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	break;
	
      case BusDriver::LineHandset:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineHandset);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::HoldTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	break;
	
      case BusDriver::LineHold:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineHold);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::HoldTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;

      case BusDriver::LineScreenedHold:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineScreenedHold);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::HoldTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;
	
      case BusDriver::LineNext:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineNext);
	  }
	}
	if(ml_config->lineShowCallerBox(line)) {
	  UpdateTimer(line,telos_caller_data[line]->
		      element(BusDriver::HoldTimerElement).toInt());
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;
	
      case BusDriver::LineRinging:
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineRinging);
	  }
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;

      case BusDriver::LineBusied:
	telos_caller_data[line]->clear();
	ClearCallerBox(line);
	for(int i=0;i<MAX_BANKS;i++) {
	  if(telos_line[i][line]!=NULL) {
	    telos_line[i][line]->setState(BusDriver::LineBusied);
	  }
	}
	telos_status[line]=lstate;
	telos_bank[line]=bank;
	if((int)line==telos_dial_line) {
	  telos_dial_line=-1;
	}
	break;
  }
  if(ml_config->lineShow(0,line)) {
    telos_line[0][line]->
      setAccel(QKeySequence(ml_config->lineKey(0,line)));
  }
  if(telos_line[1][line]) {
    telos_line[1][line]->
      setAccel(QKeySequence(ml_config->lineKey(1,line)));
  }
}


void MainWidget::busyAllStateData(unsigned id,bool state)
{
  if(state) {
    if(ml_config->buttonShow(MlConfig::BusyButton)) {
      telos_busy_button->setPalette(ml_config->palette(MlConfig::BusyColor));
#ifndef LINUX
      telos_busy_button->setAutoFillBackground(true);
      telos_busy_button->
	setStyleSheet(QString("background-color: ")+
		      ml_config->color(MlConfig::BusyColor).name());
#endif  // LINUX
      telos_busy_button->setPixmap(telos_busy_icon[1]);
      telos_busy_button->
	setAccel(QKeySequence(ml_config->buttonKey(MlConfig::DumpButton)));
    }
  }
  else {
    if(ml_config->buttonShow(MlConfig::BusyButton)) {
      telos_busy_button->setPalette(ml_config->palette(MlConfig::IdleColor));
#ifndef LINUX
      telos_busy_button->setAutoFillBackground(true);
      telos_busy_button->
	setStyleSheet(QString("background-color: ")+
		      ml_config->color(MlConfig::IdleColor).name());
#endif  // LINUX
      telos_busy_button->setPixmap(telos_busy_icon[0]);
      telos_busy_button->
	setAccel(QKeySequence(ml_config->buttonKey(MlConfig::DumpButton)));
    }
  }
}


void MainWidget::dumpDelayStateData(unsigned id,bool state)
{
  if(ml_config->buttonShow(MlConfig::DumpButton)) {
    if(state) {
      telos_dump_button->
	setPalette(ml_config->palette(MlConfig::DumpButtonColor));
    }
    else {
      telos_dump_button->
	setPalette(ml_config->palette(MlConfig::IdleColor));
    }
    telos_dump_button->
      setAccel(QKeySequence(ml_config->buttonKey(MlConfig::DumpButton)));
  }
}


void MainWidget::recordStateData(unsigned id,bool state)
{
  if(ml_config->buttonShow(MlConfig::RecordButton)) {
    telos_record_running=state;
  }
}


void MainWidget::screenStateData(unsigned id,bool state)
{
  if(state) {
    if(ml_config->buttonShow(MlConfig::ScreenButton)) {
      telos_screen_button->
	setPalette(ml_config->palette(MlConfig::ScreenButtonColor));
      telos_screen_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::ScreenButton)));
      telos_screen_state=true;
    }
  }
  else {
    if(ml_config->buttonShow(MlConfig::ScreenButton)) {
      telos_screen_button->
	setPalette(ml_config->palette(MlConfig::IdleColor));
      telos_screen_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::ScreenButton)));
      telos_screen_state=false;
    }
  }
}


void MainWidget::muteStateData(unsigned id,bool state)
{
  if(state) {
    if(ml_config->buttonShow(MlConfig::MuteButton)) {
      telos_mute_button->
	setPalette(ml_config->palette(MlConfig::MuteButtonColor));
      telos_mute_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::MuteButton)));
      telos_mute_state=true;
    }
  }
  else {
    if(ml_config->buttonShow(MlConfig::MuteButton)) {
      telos_mute_button->
	setPalette(ml_config->palette(MlConfig::IdleColor));
      telos_mute_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::MuteButton)));
      telos_mute_state=false;
    }
  }
}


void MainWidget::conferenceStateData(unsigned id,bool state)
{
  if(state) {
    if(ml_config->buttonShow(MlConfig::ConfButton)) {
      telos_conf_button->
	setPalette(ml_config->palette(MlConfig::ConfButtonColor));
      telos_conf_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::ConfButton)));
      telos_conf_state=true;
    }
  }
  else {
    if(ml_config->buttonShow(MlConfig::ConfButton)) {
      telos_conf_button->
	setPalette(ml_config->palette(MlConfig::IdleColor));
      telos_conf_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::ConfButton)));
      telos_conf_state=false;
    }
  }
}


void MainWidget::vipStateData(unsigned id,bool state)
{
  if(state) {
    if(ml_config->buttonShow(MlConfig::VipButton)) {
      telos_vip_button->
	setPalette(ml_config->palette(MlConfig::VipButtonColor));
      telos_vip_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::VipButton)));
      telos_vip_state=true;
    }
  }
  else {
    if(ml_config->buttonShow(MlConfig::VipButton)) {
      telos_vip_button->
	setPalette(ml_config->palette(MlConfig::IdleColor));
      telos_vip_button->
	setAccel(QKeySequence(ml_config->
			      buttonKey(MlConfig::VipButton)));
      telos_vip_state=false;
    }
  }
}


void MainWidget::receivedStringData(unsigned id,BusDriver::Element elem,
				    unsigned line,const QString &str)
{
  bool ok=false;
  int offset;
  QStringList fields;

  switch(elem) {
  case BusDriver::LookupGlobalElement:
    telos_message_edit->doubleClickedData();
    break;
      
  case BusDriver::MessageGlobalElement:
    if(telos_message_data->update(elem,str)) {
      if(ml_config->msgboxShow()) {
	if(telos_message_data->nick()==ML_CONTINUATION_NICK) {
	  telos_message_edit->appendMessage(telos_message_data->message());
	}
	else {
	  telos_message_edit->setMessage(telos_message_data->nick()+
					 QString("> ")+
					 telos_message_data->message());
	}
      }
    }
    break;
      
  case BusDriver::DelayOffsetGlobalElement:
    offset=str.toInt(&ok);
    if(ok&&ml_config->buttonShow(MlConfig::DumpButton)) {
      if(ml_config->buttonShowDelay(MlConfig::DumpButton)) {
	UpdateDelayButton(offset);
      }
      telos_dump_button->setFlashingEnabled(offset<
		 ml_config->buttonDelayThreshold(MlConfig::DumpButton));
    }
    telos_delay_offset=offset;
    if(ml_config->clockShowClock()) {
      telos_clock_segclock->setOffset(offset/1000);
    }
    break;
      
  case BusDriver::LineWarningGlobalElement:
    if(ml_config->lineShowCallerBox(line)&&
       (ml_config->callerEnableWarningMessages())) {
      telos_warning_box[line]->show(str);
      telos_warning_box[line]->
	setGeometry(ml_config->lineCallerBoxWindow(line).x()+
		    frameGeometry().x()+2,
		    ml_config->lineCallerBoxWindow(line).y()+
		    frameGeometry().y(),
		    ml_config->callerSize().width(),
		    ml_config->callerSize().height());
    }
    break;
      
  case BusDriver::LocalAreaCodeGlobalElement:
    telos_local_area_code=str;
    break;

  case BusDriver::SegclockGlobalElement:
    fields=str.split(",");
    for(int i=3;i<fields.size();i++) {
      fields[2]+=(","+fields[i]);
    }
    telos_dynamic_seglist->segment(0)->setFromString(str);
    telos_clock_segclock->loadSegList(telos_dynamic_seglist,false);
    break;

  case BusDriver::ClearMessageGlobalElement:
    telos_message_edit->clear();
    break;

  default:
    if(telos_caller_data[line]->update(elem,str)) {
      UpdateCallerBox(line);
      update(ml_config->lineCallerBoxWindow(line));
    }
    break;
  }
}


void MainWidget::segStartedData(int index)
{
  telos_driver->sendString(BusDriver::SegclockGlobalElement,0,
			   telos_clock_segclock->segList()->
			   segment(index)->toString());
}


void MainWidget::metadataUpdateRequestedData(unsigned id,
					     ClientData::ClientType type)
{
  if(type!=ClientData::TypeUser) {
    return;
  }
  for(unsigned i=0;i<MAX_LINES;i++) {
    telos_caller_data[i]->sendData(telos_driver,i);
  }
  if(ml_config->clockSendTimeOffset()) {
    telos_driver->sendString(BusDriver::DelayOffsetGlobalElement,0,
			     QString().sprintf("%d",1000*telos_delay_offset));
  }
}


void MainWidget::requestMetadataData()
{
  ClientData *data=NULL;

  //
  // Find the longest-running peer
  //
  for(unsigned i=1;i<telos_driver->clientList()->size();i++) {
    if(telos_driver->clientList()->at(i)->clientType()==ClientData::TypeUser) {
      if(data==NULL) {
	data=telos_driver->clientList()->at(i);
      }
      else {
	if(telos_driver->clientList()->at(i)->loginDateTime()<
	   data->loginDateTime()) {
	  data=telos_driver->clientList()->at(i);
	}
      }
    }
  }
  telos_driver->requestMetadataUpdate(data);
}


void MainWidget::TickClock()
{
  static int scale=0;
  static int ticks=0;
  bool tick_timers=false;

  //
  // Line Button Animation
  //
  if(scale++==1) {
    scale=0;
    telos_frame++;
    for(int i=0;i<MAX_BANKS;i++) {
      for(int j=0;j<MAX_LINES;j++) {
	if(telos_line[i][j]!=NULL) {
	  telos_line[i][j]->setFrame(telos_frame%3);
	}
      }
    }
  }

  //
  // Record Button Animation
  //
  if(ml_config->buttonShow(MlConfig::RecordButton)&&
     telos_record_running&&ml_config->animateIcons()) {
    if(++telos_record_frame>=telos_record_modulo) {
      telos_record_frame=0;
    }
    telos_record_button->setPixmap(telos_record_icon[telos_record_frame]);
    telos_record_button->
      setAccel(ml_config->buttonKey(MlConfig::RecordButton));
  }

  if(ticks++==9) {
    ticks=0;
    tick_timers=true;
  }

  if(tick_timers) {
    for(int j=0;j<MAX_LINES;j++) {
      switch(telos_status[j]) {
      case BusDriver::LineOnAir:
      case BusDriver::LineLocked:
	telos_caller_data[j]->
	  setElement(BusDriver::AirTimerElement,
		     telos_caller_data[j]->
		     element(BusDriver::AirTimerElement).toInt()+1);
	UpdateTimer(j,telos_caller_data[j]->
		    element(BusDriver::AirTimerElement).toInt());	  
	break;
	
      case BusDriver::LineElsewhere:
      case BusDriver::LineSpeaker:
      case BusDriver::LineHandset:
      case BusDriver::LineHold:
      case BusDriver::LineScreenedHold:
      case BusDriver::LineNext:
	telos_caller_data[j]->
	  setElement(BusDriver::HoldTimerElement,
		     telos_caller_data[j]->
		     element(BusDriver::HoldTimerElement).toInt()+1);
	UpdateTimer(j,telos_caller_data[j]->
		    element(BusDriver::HoldTimerElement).toInt());	  
	break;
	
      case BusDriver::LineRinging:
	telos_caller_data[j]->
	  setElement(BusDriver::RingTimerElement,
		     telos_caller_data[j]->
		     element(BusDriver::RingTimerElement).toInt()+1);
	UpdateTimer(j,telos_caller_data[j]->
		    element(BusDriver::RingTimerElement).toInt());	  
	break;
	
      case BusDriver::LineIdle:
      case BusDriver::LineInactive:
      case BusDriver::LineBusied:
	break;
      }
      if(ml_config->lineShow(0,j)) {
	telos_line[0][j]->setAccel(QKeySequence(ml_config->lineKey(0,j)));
      }
      if(ml_config->lineShow(1,j)) {
	telos_line[1][j]->setAccel(QKeySequence(ml_config->lineKey(1,j)));
      }
    }
  }
}


void MainWidget::busyData()
{
  telos_driver->busyAll();
}


void MainWidget::dumpPressedData()
{
  telos_driver->dumpDelay(true);
}


void MainWidget::dumpReleasedData()
{
  telos_driver->dumpDelay(false);
}


void MainWidget::recordData()
{
  telos_driver->toggleRecord();
}


void MainWidget::nextData()
{
  telos_driver->takeNext(0);
}


void MainWidget::keypadData()
{
  Keybox *keybox=new Keybox(telos_driver,telos_local_area_code,
			    telos_dial_line,this,"keybox");
  //
  // FIXME: Needed to make this size correctly under win32.  Why?
  //
  keybox->setGeometry((size().width()-keybox->sizeHint().width())/2,
		      (size().height()-keybox->sizeHint().height())/2,
		      keybox->sizeHint().width(),keybox->sizeHint().height());
  keybox->exec();
  delete keybox;
}


void MainWidget::flashData()
{
  telos_driver->flash();
}


void MainWidget::screenData()
{
  telos_driver->screen(!telos_screen_state);
}


void MainWidget::muteData()
{
  telos_driver->mute(!telos_mute_state);
}


void MainWidget::confData()
{
  telos_driver->conference(!telos_conf_state);
}


void MainWidget::vipData()
{
  telos_driver->vip(!telos_vip_state);
}


void MainWidget::clientData()
{
  telos_list_clients->show();
}


void MainWidget::demoTimeoutData()
{
  QMessageBox::information(this,tr("Demo Expired"),
			   tr("The demo time has expired!"));
  qApp->quit();
}


void MainWidget::spawnProcessExitedData()
{
  if(!ml_config->spawnOnce()) {
    telos_spawn_process->launch(QString());
  }
}


void MainWidget::UpdateTimer(int line,int value)
{
  telos_caller_info[line]->setTimer(value);
}


void MainWidget::SendCallData(int line,bool timers)
{
  telos_caller_data[line]->sendData(telos_driver,line);
}


void MainWidget::ClearCallerBox(int line)
{
  telos_caller_info[line]->clear();
  if(ml_config->lineShowCallerBox(line)) {
    update(ml_config->lineCallerBoxWindow(line));
  }
}


void MainWidget::UpdateCallerBox(int line)
{
  telos_caller_info[line]->setMetadata(telos_caller_data[line]);
}


void MainWidget::SetCaption()
{
  QString ddname=telos_connection->consoleName();
  switch(telos_director_mode) {
      case ConsoleData::Talent:
	setCaption(QString().sprintf("%s - [Mode: Talent, Console: %s]",
				     (const char *)telos_connection->name(),
				     (const char *)ddname));
	break;

      case ConsoleData::Producer:
	setCaption(QString().sprintf("%s - [Mode: Producer, Console: %s]",
				     (const char *)telos_connection->name(),
				     (const char *)ddname));
	break;

      case ConsoleData::Unavailable:
	setCaption(QString().sprintf("%s - [Mode: Invalid, Console: %s]",
				     (const char *)telos_connection->name(),
				     (const char *)ddname));
  }
}


int main(int argc,char *argv[])
{
  Qt::WFlags f=0;

  QApplication a(argc,argv);
#ifndef WIN32
  for(int i=0;i<argc;i++) {
    if((!strcmp(argv[i],"-v"))||(!strcmp(argv[i],"--version"))) {
      printf("CallCommander User Interface - version %s\n",VERSION);
      exit(0);
    }
  }
#endif  // WIN32

  MLProfile *parser=new MLProfile();
  parser->setSource(GetConfFile(argc,argv));
  if(parser->boolValue("Global","KeepOnTop",DEFAULT_ON_TOP)) {
    f=Qt::WStyle_StaysOnTop;
  }
  delete parser;

  MainWidget *w=new MainWidget(NULL,"main",f);
  a.setMainWidget(w);
  w->show();
  return a.exec();
}
