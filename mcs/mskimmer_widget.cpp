// mcs_widget.cpp
//
// A Qt-based application for recording telephone calls
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

#include <QtGui/QApplication>
#include <QtCore/QSignalMapper>
#include <QtGui/QMessageBox>
#include <QtCore/QRegExp>
#include <QtGui/QLabel>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mlconf.h>

#include <askdata.h>
#include <askbox.h>

#include <globals.h>
#include <mskimmer_widget.h>

McsWidget::McsWidget(int bank,ConnectionData *data,BusDriver *driver,
		     QWidget *parent,const char *name)
  :QWidget(parent,name)
{
  QFont font;

  mcs_bank=bank;
  mcs_connection=data;
  mcs_meter_height=0;
  mcs_manual_active=false;
  for(int i=0;i<MAX_LINES;i++) {
    mcs_line_active[i]=false;
  }
  Init();

  //
  // Generate Font
  //
  font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Line Selector Buttons
  //
  QSignalMapper *mapper=new QSignalMapper(this,"mapper");
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(mapperData(int)));
  for(int i=0;i<(MAX_LINES/6);i++) {
    for(int j=0;j<6;j++) {
      mcs_label[6*i+j]=new QLabel(this);
      mcs_label[6*i+j]->setGeometry(20+45*j,5+60*i,20,5);
      mcs_label[6*i+j]->setPalette(QColor(IDLE_COLOR));

      mcs_main_button[6*i+j]=new QPushButton(this);
      mcs_main_button[6*i+j]->setGeometry(10+45*j,15+60*i,40,40);
      mcs_main_button[6*i+j]->setFont(font);
      mcs_main_button[6*i+j]->setText(QString().sprintf("%d",6*i+j+1));
      mapper->setMapping(mcs_main_button[6*i+j],6*i+j);
      connect(mcs_main_button[6*i+j],SIGNAL(clicked()),mapper,SLOT(map()));
      mcs_main_button[6*i+j]->setOn(ml_config->skimmerEnable(mcs_bank,6*i+j));
      if(ml_config->skimmerEnable(mcs_bank,6*i+j)) {
	mcs_main_button[6*i+j]->
	  setPalette(QPalette(QColor(Qt::green),backgroundColor()));
      }      
    }
  }

  //
  // The Recorder
  //
  mcs_soundcard=new MLHPISoundCard(this,"mcs_soundcard");
  mcs_record=new MLHPIRecordStream(mcs_soundcard,this,"mcs_record");
  mcs_record->setCard(ml_config->skimmerCardNum(mcs_bank));
  mcs_record->setStream(ml_config->skimmerPortNum(mcs_bank));
  switch(ml_config->skimmerInputType(mcs_bank)) {
      case MlConfig::Analog:
	mcs_soundcard->setInputLevel(ml_config->skimmerCardNum(mcs_bank),
				     ml_config->skimmerPortNum(mcs_bank),
				     ml_config->skimmerInputLevel(mcs_bank)+1600);
	mcs_soundcard->
	  setInputPortMux(ml_config->skimmerCardNum(mcs_bank),
			  ml_config->skimmerPortNum(mcs_bank),
			  MLHPISoundCard::LineIn);
	break;

      case MlConfig::AesEbu:
	mcs_soundcard->
	  setInputPortMux(ml_config->skimmerCardNum(mcs_bank),
			  ml_config->skimmerPortNum(mcs_bank),
			  MLHPISoundCard::AesEbuIn);
	break;
  }

  connect(mcs_record,SIGNAL(recording()),this,SLOT(recordedData()));
  connect(mcs_record,SIGNAL(recording()),this,SLOT(recordedData()));
  connect(mcs_record,SIGNAL(paused()),this,SLOT(stoppedData()));
  connect(mcs_record,SIGNAL(stopped()),this,SLOT(stoppedData()));

  //
  // Meters
  //
  mcs_meter_height+=20;
  mcs_left_meter=new MLPlayMeter(MLSegMeter::Right,this,"mcs_left_meter");
  mcs_left_meter->setGeometry(10,125,265,10);
  mcs_left_meter->setRange(-3600,-800);
  mcs_left_meter->setSegmentSize(5);
  mcs_left_meter->setHighThreshold(-1600);
  mcs_left_meter->setClipThreshold(-1100);
  if(ml_config->skimmerChannels()==2) {
    mcs_left_meter->setLabel("L");
    mcs_meter_height+=10;
    mcs_right_meter=new MLPlayMeter(MLSegMeter::Right,this,"mcs_right_meter");
    mcs_right_meter->setGeometry(10,135,265,10);
    mcs_right_meter->setRange(-3600,-800);
    mcs_right_meter->setSegmentSize(5);
    mcs_right_meter->setHighThreshold(-1600);
    mcs_right_meter->setClipThreshold(-1100);
    mcs_right_meter->setLabel("R");
  }
  mcs_meter_timer=new QTimer(this,"mcs_meter_timer");
  connect(mcs_meter_timer,SIGNAL(timeout()),this,SLOT(meterData()));

  //
  // Transport Buttons
  //
  mcs_record_button=new MLTransportButton(MLTransportButton::Record,
					 this,"mcs_record_button");
  mcs_record_button->setGeometry(10,125+mcs_meter_height,60,40);
  mcs_record_button->off();
  connect(mcs_record_button,SIGNAL(clicked()),this,SLOT(recordData()));

  mcs_stop_button=new MLTransportButton(MLTransportButton::Stop,
					 this,"mcs_stop_button");
  mcs_stop_button->setGeometry(80,125+mcs_meter_height,60,40);
  mcs_stop_button->on();
  connect(mcs_stop_button,SIGNAL(clicked()),this,SLOT(stopData()));

  //
  // Filename
  //
  mcs_filename_edit=new QLineEdit(this,"mcs_filename_edit");
  mcs_filename_edit->setFont(font);
  mcs_filename_edit->setGeometry(150,143+mcs_meter_height,120,22);
  mcs_filename_edit->setText("<idle>");
  QLabel *label=new QLabel(mcs_filename_edit,"Filename",this);
  label->setGeometry(152,125+mcs_meter_height,100,18);
  label->setFont(font);

  //
  // Record Timer
  //
  mcs_timer_label=new QLabel(this,"mcs_timer_label");
  mcs_timer_label->setFont(font);
  mcs_timer_label->setGeometry(220,125+mcs_meter_height,100,18);
  mcs_timer_timer=new QTimer(this,"mcs_timer_timer");
  connect(mcs_timer_timer,SIGNAL(timeout()),this,SLOT(recordTimerData()));

  //
  // Caller Data
  //
  mcs_caller_check_data=new CallMetaData();
  for(int i=0;i<MAX_LINES;i++) {
    mcs_caller_data[i]=new CallMetaData();
    ClearCaller(i);
  }

  //
  // The Bus Driver
  //
  mcs_driver=driver;
  connect(mcs_driver,
	  SIGNAL(currentLineState(unsigned,BusDriver::LineState,unsigned,
				  int,int)),this,
	  SLOT(lineStateData(unsigned,BusDriver::LineState,unsigned,int,int)));
  connect(mcs_driver,SIGNAL(receivedString(unsigned,BusDriver::Element,
					   unsigned,const QString &)),
	  this,SLOT(receivedStringData(unsigned,BusDriver::Element,unsigned,
				       const QString &)));
}


McsWidget::~McsWidget()
{
  for(int i=0;i<MAX_LINES;i++) {
    DeckStop(i,mcs_bank);
  }
}


QSize McsWidget::sizeHint() const
{
  return QSize(285,170+mcs_meter_height);
}


QSizePolicy McsWidget::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void McsWidget::mapperData(int line)
{
  if(ml_config->skimmerEnable(mcs_bank,line)) {
    mcs_main_button[line]->setPalette(palette());
    ml_config->setSkimmerEnable(mcs_bank,line,false);
  }
  else {
    mcs_main_button[line]->
      setPalette(QPalette(QColor(Qt::green),backgroundColor()));
    ml_config->setSkimmerEnable(mcs_bank,line,true);
  }
}


void McsWidget::recordData()
{
  DeckStart(-1,-1);
}


void McsWidget::stopData()
{
  DeckStop(-1,-1);
}


void McsWidget::recordedData()
{
  mcs_record_button->on();
  mcs_stop_button->off();
  mcs_meter_timer->start(METER_UPDATE_INTERVAL);
}


void McsWidget::stoppedData()
{
  mcs_record_button->off();
  mcs_stop_button->on();
  mcs_meter_timer->stop();
  mcs_left_meter->setSolidBar(-10000);
  mcs_right_meter->setSolidBar(-10000);  
  mcs_record->closeWave();
  mcs_filename_edit->setText("<idle>");
  mcs_timer_label->setText("");
  mcs_timer_timer->stop();
  if(!mcs_line_cmd.isEmpty()) {
    if(fork()==0) {
      system((const char *)mcs_line_cmd);
      exit(0);
    }
  }
}


void McsWidget::lineStateData(unsigned id,BusDriver::LineState lstate,
			      unsigned line,int bank,int console)
{
  switch(lstate) {
      case BusDriver::LineInactive:
	mcs_main_button[line]->setDisabled(true);
	mcs_main_button[line]->setPalette(palette());
	mcs_label[line]->setPalette(UNAVAIL_COLOR);
	break;
	
      case BusDriver::LineIdle:
	DeckStop(line,bank);
	ClearCaller(line);
	mcs_label[line]->setPalette(IDLE_COLOR);
	break;
	
      case BusDriver::LineElsewhere:
      case BusDriver::LineSpeaker:
      case BusDriver::LineHandset:
	DeckStop(line,bank);
	mcs_label[line]->setPalette(OFF_HOOK_COLOR);
	break;
	
      case BusDriver::LineOnAir:
      case BusDriver::LineLocked:
	if(bank==mcs_bank) {
	  DeckStart(line,bank);
	  mcs_label[line]->setPalette(ON_AIR_COLOR);
	}
	break;
	
      case BusDriver::LineHold:
      case BusDriver::LineScreenedHold:
      case BusDriver::LineNext:
	DeckStop(line,bank);
	mcs_label[line]->setPalette(ON_HOLD_COLOR);
	break;
	
      case BusDriver::LineRinging:
	DeckStop(line,bank);
	mcs_label[line]->setPalette(RINGING_COLOR);
	break;
	
      case BusDriver::LineBusied:   // Busy Out
	DeckStop(line,bank);
	mcs_label[line]->setPalette(BUSYOUT_COLOR);
	break;
  }
}


void McsWidget::receivedStringData(unsigned id,BusDriver::Element elem,
				   unsigned line,const QString &str)
{
  mcs_caller_data[line]->update(elem,str);
}


void McsWidget::recordTimerData()
{
  mcs_timer_label->
    setText(MLGetTimeLength(1000*(++mcs_record_time),true,false));
}


void McsWidget::meterData()
{
  short levels[2];
  mcs_soundcard->inputStreamMeter(ml_config->skimmerCardNum(mcs_bank),
				  ml_config->skimmerPortNum(mcs_bank),levels);
  mcs_left_meter->setSolidBar(levels[0]);
  mcs_right_meter->setSolidBar(levels[1]);
}


void McsWidget::DeckStart(int line,int bank)
{
  if(((bank==mcs_bank)&&ml_config->skimmerEnable(mcs_bank,line))||(line==-1)) {
    if((bank==mcs_bank)&&ml_config->skimmerEnable(mcs_bank,line)) {
      mcs_line_active[line]=true;
    }
    if(line==-1) {
      mcs_manual_active=true;
    }
    if(mcs_record->getState()!=MLHPIRecordStream::Recording) {
      QDateTime datetime(QDate::currentDate(),QTime::currentTime());
      QString filename=
	WildcardDecode(ml_config->skimmerFilename(mcs_bank),
		       line,datetime,mcs_manual_active).
	replace(QRegExp("/"),"_");
      QString pathname=ml_config->skimmerAudioRoot()+filename;
      mcs_line_cmd=WildcardDecode(ml_config->skimmerPostCommand(mcs_bank),
				  line,datetime,mcs_manual_active);
      mcs_record->nameWave(pathname);
      mcs_record->setSamplesPerSec(ml_config->skimmerSampleRate());
      mcs_record->setChannels(ml_config->skimmerChannels());
      mcs_record->setBextChunk(true);
      switch(ml_config->skimmerFormat()) {
	  case MLWaveFile::Pcm16:
	    mcs_record->setFormatTag(WAVE_FORMAT_PCM);
	    mcs_record->setBitsPerSample(16);
	    break;
	  case MLWaveFile::MpegL2:
	    mcs_record->setMextChunk(true);
	    mcs_record->setFormatTag(WAVE_FORMAT_MPEG);
	    mcs_record->setBitsPerSample(16);
	    mcs_record->setHeadLayer(2);
	    mcs_record->setHeadBitRate(ml_config->skimmerBitrate());
	    mcs_record->setHeadFlags(ACM_MPEG_ID_MPEG1);
	    if(ml_config->skimmerChannels()==1) {
	      mcs_record->setHeadMode(ACM_MPEG_SINGLECHANNEL);
	    }
	    else {
	      mcs_record->setHeadMode(ACM_MPEG_DUALCHANNEL);
	    }
	    break;
	  case MLWaveFile::MpegL3:
	    mcs_record->setMextChunk(true);
	    mcs_record->setFormatTag(WAVE_FORMAT_MPEG);
	    mcs_record->setBitsPerSample(16);
	    mcs_record->setHeadLayer(3);
	    mcs_record->setHeadBitRate(ml_config->skimmerBitrate());
	    mcs_record->setHeadFlags(ACM_MPEG_ID_MPEG1);
	    if(ml_config->skimmerChannels()==1) {
	      mcs_record->setHeadMode(ACM_MPEG_SINGLECHANNEL);
	    }
	    else {
	      mcs_record->setHeadMode(ACM_MPEG_DUALCHANNEL);
	    }
	    break;

	  default:
	    return;
      }
      mcs_record->setRecordLength(ml_config->skimmerMaxLength(mcs_bank));
      mcs_record->createWave();
      if(!mcs_record->recordReady()) {
	mcs_record->closeWave();
	return;
      }
      mcs_filename_edit->setText(filename);
      mcs_record->record();
      mcs_record_time=0;
      mcs_timer_label->setText(MLGetTimeLength(0,true,false));
      mcs_timer_timer->start(1000);
    }
  }
}


void McsWidget::DeckStop(int line,int bank)
{
  bool state=false;

  if(line>=0) {
    if(ml_config->skimmerEnable(mcs_bank,line)) {
      mcs_line_active[line]=false;
      for(int i=0;i<MAX_LINES;i++) {
	state=state||mcs_line_active[i];
      }
      state=state||mcs_manual_active;
    }
  }
  else {
    mcs_manual_active=false;
  }
  if((!state)&&(mcs_record->getState()==4)) {  // STOP
    mcs_record->stop();
  }
}


void McsWidget::Init()
{
  for(int i=0;i<MAX_LINES;i++) {
    mcs_line_active[i]=false;
  }
}


void McsWidget::ClearCaller(int line)
{
  mcs_caller_data[line]->clear();
  mcs_caller_data[line]->setElement(BusDriver::NameElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::AgeElement,UNKNOWN_STRING);
  mcs_caller_data[line]->
    setElement(BusDriver::CellphoneElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::QualityElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::Addr1Element,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::Addr2Element,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::CityElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::StateElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::ZipElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::PhoneElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::EmailElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::GenderElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::CommentElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::StationElement,UNKNOWN_STRING);
  mcs_caller_data[line]->setElement(BusDriver::AgeElement,UNKNOWN_STRING);
}


QString McsWidget::WildcardDecode(QString spec,int line,QDateTime datetime,
				  bool manual)
{
  QString str;
/*
  int age;
  int cell;
  int quality;

  if(line>-1) {
    sscanf((const char *)mcs_caller_data[line]->street(),"%d:%d:%d",
	   &age,&cell,&quality);
  }
*/
  for(unsigned i=0;i<spec.length();i++) {
    if(spec.at(i)!='%') {
      str+=spec.at(i);
    }
    else {
      i++;
      switch(((const char *)spec)[i]) {
	  case 'd':   // Day (01 - 31)
	    str+=QString().sprintf("%02d",datetime.date().day());
	    break;

	  case 'm':   // Month (01 - 12)
	    str+=QString().sprintf("%02d",datetime.date().month());
	    break;
	    
	  case 'Y':   // Year (yyyy)
	    str+=QString().sprintf("%04d",datetime.date().year());
	    break;

	  case 'k':   // Hour (00 - 23)
	    str+=QString().sprintf("%02d",datetime.time().hour());
	    break;

	  case 'M':   // Minute (00 - 59)
	    str+=QString().sprintf("%02d",datetime.time().minute());
	    break;

	  case 'S':   // Second (00 - 60)
	    str+=QString().sprintf("%02d",datetime.time().second());
	    break;

	  case 'L':   // Line Number
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=QString().sprintf("%d",line+1);
	    }
	    break;

	  case 'n':   // Caller Name
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::NameElement).toString();
	    }
	    break;

	  case 'C':   // Cellphone Flag
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::CellphoneElement).toString();
	    }
	    break;

	  case 'q':   // Quality Rating
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::QualityElement).toString();
	    }
	    break;

	  case 'A':   // Caller Age
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::AgeElement).toString();
	    }
	    break;

	  case 'c':   // Caller City
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::CityElement).toString();
	    }
	    break;

	  case 't':   // Caller State
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::StateElement).toString();
	    }
	    break;

	  case 'z':   // Caller ZIP Code
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::ZipElement).toString();
	    }
	    break;

	  case 'w':   // Caller Station
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::StationElement).toString();
	    }
	    break;

	  case 'g':   // Caller Gender
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::GenderElement).toString();
	    }
	    break;

	  case 'p':   // Caller Phone Number
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::PhoneElement).toString();
	    }
	    break;

	  case 'e':   // Caller Comment
	    if(manual) {
	      str+=BUTTON_STRING;
	      break;
	    }
	    if(line>=0) {
	      str+=mcs_caller_data[line]->
		element(BusDriver::CommentElement).toString();
	    }
	    break;

	  case '%':   // Literal '%'
	    str+=QString("%");
	    break;
      }
    }
  }
  return str;
}
