//   mlhpirecordstream.cpp
//
//   A class for recording Microsoft WAV files.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlhpirecordstream.cpp,v 1.7 2012/06/26 21:30:02 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "mlhpirecordstream.h"

MLHPIRecordStream::MLHPIRecordStream(MLHPISoundCard *card,
			     QWidget *parent,const char *name) 
  :QObject(parent,name),MLWaveFile()
{ 
  int quan;
  uint16_t type[HPI_MAX_ADAPTERS];

  if(getenv(DEBUG_VAR)==NULL) {
    debug=false;
  }
  else {
    debug=true;
    printf("MLHPIRecordStream: debugging enabled\n");
  }
  if(getenv(XRUN_VAR)==NULL) {
    xrun=false;
  }
  else {
    xrun=true;
    printf("MLHPIRecordStream: xrun notification enabled\n");
  }

  sound_card=card;

  card_number=-1;
  stream_number=-1;
  is_ready=false;
  is_recording=false;
  is_paused=false;
  stopping=false;
  record_started=false;
  record_length=0;
  is_open=false;
  pdata=NULL;

  //
  // Get Adapter Indices
  //
#if HPI_VER < 0x00030600
  for(unsigned i=0;i<HPI_MAX_ADAPTERS;i++) {
    card_index[i]=i;
  }
#else
  LogHpi(HPI_SubSysGetNumAdapters(NULL,&quan));
  for(int i=0;i<quan;i++) {
    LogHpi(HPI_SubSysGetAdapter(NULL,i,card_index+i,type+i));
  }
#endif  // HPI_VER

  clock=new QTimer(this,"clock");
  connect(clock,SIGNAL(timeout()),this,SLOT(tickClock()));

  length_timer=new QTimer(this,"length_timer");
  connect(length_timer,SIGNAL(timeout()),this,SLOT(pause()));
}


MLHPIRecordStream::~MLHPIRecordStream()
{
  if(pdata!=NULL) {
    delete pdata;
  }
} 


QString MLHPIRecordStream::errorString(MLHPIRecordStream::Error err)
{
  QString str;

  switch(err) {
      case MLHPIRecordStream::Ok:
	return QString(tr("Ok"));
	break;

      case MLHPIRecordStream::NoFile:
	return QString(tr("Unable to create/open file"));
	break;

      case MLHPIRecordStream::NoStream:
	return QString(tr("Input stream unavailable"));
	break;

      case MLHPIRecordStream::AlreadyOpen:
	return QString(tr("Stream is already open"));
	break;

      default:
	str=QString(tr("Unknown Error:"));
	return QString().sprintf("%s %d\n",(const char *)str,err);
	break;
  }
}


MLHPIRecordStream::Error MLHPIRecordStream::createWave()
{
  if(is_open) {
    return MLHPIRecordStream::AlreadyOpen;
  }
  if(!MLWaveFile::createWave()) {
    return MLHPIRecordStream::NoFile;
  }
  if(!GetStream()) {
    closeWave();
    return MLHPIRecordStream::NoStream;
  }
  is_open=true;
  return MLHPIRecordStream::Ok;
}


MLHPIRecordStream::Error MLHPIRecordStream::createWave(QString filename)
{
  if(is_open) {
    return MLHPIRecordStream::AlreadyOpen;
  }
  setName(filename);
  return createWave();
}


void MLHPIRecordStream::closeWave()
{
  if(!is_open) {
    return;
  }
  if(getState()!=MLHPIRecordStream::Stopped) {
    stop();
  }
  MLWaveFile::closeWave(samples_recorded);
  FreeStream();
  is_open=false;
}


bool MLHPIRecordStream::formatSupported(MLWaveFile::Format format)
{
#if HPI_VER < HPI_VERSION_CONSTRUCTOR(3L,10,0)
  HPI_FORMAT hformat;
#else
  struct hpi_format hformat;
#endif
  hpi_handle_t histream;
  bool found=false;

  if(card_number<0) {
    return false;
  }
  if(format==MLWaveFile::Vorbis) {
#ifdef HAVE_VORBIS
    return true;
#endif  // HAVE_VORBIS
    return false;
  }
  if(!is_open) {
    for(int i=0;i<sound_card->getCardInputStreams(card_number);i++) {
      if(HPI_InStreamOpen(NULL,card_index[card_number],i,&histream)==0) {
	found=true;
	break;
      }
    }
    if(!found) {
      return false;
    }
  }
  else {
    histream=hpi_stream;
  }
  switch(format) {
      case MLWaveFile::Pcm8:
	LogHpi(HPI_FormatCreate(&hformat,getChannels(),
				HPI_FORMAT_PCM8_UNSIGNED,
				getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case MLWaveFile::Pcm16:
	LogHpi(HPI_FormatCreate(&hformat,getChannels(),
				HPI_FORMAT_PCM16_SIGNED,
				getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case MLWaveFile::MpegL1:
	LogHpi(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L1,
				getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case MLWaveFile::MpegL2:
	LogHpi(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L2,
				getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      case MLWaveFile::MpegL3:
	LogHpi(HPI_FormatCreate(&hformat,getChannels(),HPI_FORMAT_MPEG_L3,
				getSamplesPerSec(),getHeadBitRate(),0));
	state=HPI_InStreamQueryFormat(NULL,histream,&hformat);
	break;

      default:
	state=1;
	break;
  }
  if(!is_open) {
    LogHpi(HPI_InStreamClose(NULL,histream));
  }
  if(state!=0) {
    return false;
  }
  return true;
}


bool MLHPIRecordStream::formatSupported()
{
  switch(getFormatTag()) {
      case WAVE_FORMAT_PCM:
	switch(getBitsPerSample()) {
	    case 8:
	      return formatSupported(MLWaveFile::Pcm8);
	      break;

	    case 16:
	      return formatSupported(MLWaveFile::Pcm16);
	      break;

	    default:
	      return false;
	}
	break;

      case WAVE_FORMAT_MPEG:
	switch(getHeadLayer()) {
	    case 1:
	      return formatSupported(MLWaveFile::MpegL1);
	      break;

	    case 2:
	      return formatSupported(MLWaveFile::MpegL2);
	      break;

	    case 3:
	      return formatSupported(MLWaveFile::MpegL3);
	      break;

	    default:
	      return false;
	}
	break;

      default:
	return false;
  }
}


int MLHPIRecordStream::getCard() const
{
  return card_number;
}


void MLHPIRecordStream::setCard(int card)
{
  if(!is_recording) {
    card_number=card;
    if(debug) {
      printf("MLHPIRecordStream: using card %d\n",card_number);
    }
  }
}


int MLHPIRecordStream::getStream() const
{
  return stream_number;
}


void MLHPIRecordStream::setStream(int stream)
{
  stream_number=stream;
}


bool MLHPIRecordStream::haveInputVOX() const
{
  return sound_card->haveInputStreamVOX(card_number,stream_number);
}


MLHPIRecordStream::RecordState MLHPIRecordStream::getState()
{
  if(is_recording) {
    if(record_started) {
      return MLHPIRecordStream::RecordStarted;
    }
    return MLHPIRecordStream::Recording;
  }
  if(is_paused) {
    return MLHPIRecordStream::Paused;
  }
  if(is_ready) {
    return MLHPIRecordStream::RecordReady;
  }
  return MLHPIRecordStream::Stopped;
}


int MLHPIRecordStream::getPosition() const
{
  if((!is_recording)&&(!is_ready)&&(!is_paused)) {
    return 0;
  }
  return samples_recorded;
}


unsigned MLHPIRecordStream::samplesRecorded() const
{
  return samples_recorded;
}


bool MLHPIRecordStream::recordReady()
{
  if(debug) {
    printf("MLHPIRecordStream: received recordReady()\n");
  }
  if(!is_open) {
    return false;
  }
  if((!is_recording)&&(!is_paused)) {
    resetWave();
    if(HPI_InStreamGetInfoEx(NULL,hpi_stream,
			     &state,&buffer_size,&data_recorded,
			     &samples_recorded,&reserved)!=0) {
      if(debug) {
	printf("MLHPIRecordStream: HPI_InStreamGetInfoEx() failed\n");
      }
      return false;
    }
    fragment_size=buffer_size/4;
    if(fragment_size>192000) {  // ALSA Compatibility Limitation
      fragment_size=192000;
    }
    fragment_time=(1000*fragment_size)/(getAvgBytesPerSec());
    if(pdata!=NULL) {
      delete pdata;
    }
    pdata=(uint8_t *)malloc(fragment_size);
    if(pdata==NULL) {
      if(debug) {
	printf("MLHPIRecordStream: couldn't allocate buffer\n");
      }
      return false;
    }
    switch(getFormatTag()) {
	case WAVE_FORMAT_PCM:
	  if(debug) {
	    printf("MLHPIRecordStream: using PCM%d format\n",
		   getBitsPerSample());
	  }
	  switch(getBitsPerSample()) {
	      case 8:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_PCM8_UNSIGNED,
					getSamplesPerSec(),0,0));
		break;
	      case 16:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_PCM16_SIGNED,
					getSamplesPerSec(),0,0));
		break;
	      case 32:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_PCM32_SIGNED,
					getSamplesPerSec(),0,0));
		break;
	      default:
		if(debug) {
		  printf("MLHPIRecordStream: unsupported sample size\n");
		}
		return false;
	  }
	  break;

	case WAVE_FORMAT_MPEG:
	  if(debug) {
	    printf("MLHPIRecordStream: using MPEG-1 Layer %d\n",getHeadLayer());
	  }
	  switch(getHeadLayer()) {
	      case 1:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_MPEG_L1,getSamplesPerSec(),
					getHeadBitRate(),getHeadFlags()));
		break;
	      case 2:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_MPEG_L2,getSamplesPerSec(),
					getHeadBitRate(),getHeadFlags()));
		break;
	      case 3:
		LogHpi(HPI_FormatCreate(&format,getChannels(),
					HPI_FORMAT_MPEG_L3,getSamplesPerSec(),
					getHeadBitRate(),getHeadFlags()));
		break;
	      default:
		LogHpi(HPI_AdapterClose(NULL,card_index[card_number]));
		if(debug) {
		  printf("MLHPIRecordStream: invalid MPEG-1 layer\n");
		}
		return false;
	  }
	  if(getMextChunk()) {
	    setMextHomogenous(true);
	    setMextPaddingUsed(false);
	    setMextHackedBitRate(true);
	    setMextFreeFormat(false);
	    setMextFrameSize(144*getHeadBitRate()/getSamplesPerSec());
	    setMextAncillaryLength(5);
	    setMextLeftEnergyPresent(true);
	    if(getChannels()>1) {
	      setMextRightEnergyPresent(true);
	    }
	    else {
	      setMextRightEnergyPresent(false);
	    }
	    setMextPrivateDataPresent(false);
	  }
	  break;

	case WAVE_FORMAT_VORBIS:
	  if(debug) {
	    printf("MLHPIRecordStream: using OggVorbis\n");
	  }
	  LogHpi(HPI_FormatCreate(&format,getChannels(),
				  HPI_FORMAT_PCM16_SIGNED,getSamplesPerSec(),
				  0,0));
	  break;

	default:
	  if(debug) {
	    printf("MLHPIRecordStream: invalid format tag\n");
	  }
	  return false;
	  break;
    }
    if(LogHpi(HPI_InStreamQueryFormat(NULL,hpi_stream,&format))!=0) {
      return false;
    }
  }
#if HPI_VER < 0x00030500
  HPI_DataCreate(&hpi_data,&format,pdata,fragment_size);
#endif
  LogHpi(HPI_InStreamSetFormat(NULL,hpi_stream,&format));
  LogHpi(HPI_InStreamStart(NULL,hpi_stream));
  clock->start(100);
  is_ready=true;
  is_recording=false;
  is_paused=false;
  stopping=false;
  emit isStopped(false);
  emit ready();
  emit stateChanged(card_number,stream_number,1);  // RecordReady
  if(debug) {
    printf("MLHPIRecordStream: emitted isStopped(false)\n");
    printf("MLHPIRecordStream: emitted ready()\n");
    printf("MLHPIRecordStream: emitted stateChanged(%d,%d,MLHPIRecordStream::RecordReady)\n",card_number,stream_number);
  }

  return true;
}


void MLHPIRecordStream::record()
{
  if(debug) {
    printf("MLHPIRecordStream: received record()\n");
  }
  if(!is_open) {
    return;
  }
  if(!is_ready) {
    recordReady();
  }
  record_started=false;
  LogHpi(HPI_InStreamReset(NULL,hpi_stream));
  LogHpi(HPI_InStreamStart(NULL,hpi_stream));
  is_recording=true;
  is_paused=false;
  emit isStopped(false);
  emit recording();
  emit stateChanged(card_number,stream_number,0);  // Recording
  if(debug) {
    printf("MLHPIRecordStream: emitted isStopped(false)\n");
    printf("MLHPIRecordStream: emitted recording()\n");
    printf("MLHPIRecordStream: emitted stateChanged(%d,%d,MLHPIRecordStream::Recording)\n",card_number,stream_number);
  }
  tickClock();
}


void MLHPIRecordStream::pause()
{
  if(debug) {
    printf("MLHPIRecordStream: received pause()\n");
  }
  if(!is_recording) {
    return;
  }
  LogHpi(HPI_InStreamStop(NULL,hpi_stream));
  tickClock();
  LogHpi(HPI_InStreamGetInfoEx(NULL,hpi_stream,&state,&buffer_size,
			       &data_recorded,&samples_recorded,&reserved));
  is_recording=false;
  is_paused=true;
  LogHpi(HPI_InStreamStart(NULL,hpi_stream));
  emit paused();
  emit stateChanged(card_number,stream_number,2);  // Paused
  if(debug) {
    printf("MLHPIRecordStream: emitted paused()\n");
    printf("MLHPIRecordStream: emitted stateChanged(%d,%d,MLHPIRecordStream::Paused)\n",card_number,stream_number);
  }
}


void MLHPIRecordStream::stop()
{
  if(debug) {
    printf("MLHPIRecordStream: received stop()\n");
  }
  if(is_ready|is_recording|is_paused) {
    LogHpi(HPI_InStreamStop(NULL,hpi_stream));
    tickClock();
    clock->stop();
    is_recording=false;
    is_paused=false;
    is_ready=false;
    if(pdata!=NULL) {
      delete pdata;
      pdata=NULL;
    }
    emit isStopped(true);
    emit stopped();
    emit stateChanged(card_number,stream_number,MLHPIRecordStream::Stopped);
    emit position(0);
    if(debug) {
      printf("MLHPIRecordStream: emitted isStopped(true)\n");
      printf("MLHPIRecordStream: emitted stopped()\n");
      printf("MLHPIRecordStream: emitted stateChanged(%d,%d,MLHPIRecordStream::Stopped)\n",card_number,stream_number);
      printf("MLHPIRecordStream: emitted position(0)\n");
    }
  }
}


void MLHPIRecordStream::setInputVOX(int gain)
{
  sound_card->setInputStreamVOX(card_number,stream_number,gain);
}


void MLHPIRecordStream::setRecordLength(int length)
{
  record_length=length;
}


void MLHPIRecordStream::tickClock()
{
  LogHpi(HPI_InStreamGetInfoEx(NULL,hpi_stream,
			       &state,&buffer_size,&data_recorded,
			       &samples_recorded,&reserved));
  if((!record_started)&&(is_recording)) {
    if(samples_recorded>0) {
      if(record_length>0) {
        length_timer->start(record_length,true);
      }
      emit recordStart();
      emit stateChanged(card_number,stream_number,4);  // RecordStarted
      if(debug) {
	printf("MLHPIRecordStream: emitted recordStart()\n");
	printf("MLHPIRecordStream: emitted stateChanged(%d,%d,MLHPIRecordStream::RecordStarted)\n",card_number,stream_number);
      }
      record_started=true;
    }
  }
  while(data_recorded>fragment_size) {
#if HPI_VER > 0x00030500
    LogHpi(HPI_InStreamReadBuf(NULL,hpi_stream,pdata,fragment_size));
#else
    LogHpi(HPI_InStreamRead(NULL,hpi_stream,&hpi_data));
#endif
    if(is_recording) {
      writeWave(pdata,fragment_size);
    }
    LogHpi(HPI_InStreamGetInfoEx(NULL,hpi_stream,
				 &state,&buffer_size,&data_recorded,
				 &samples_recorded,&reserved));
  }
  if(state==HPI_STATE_STOPPED) {
#if HPI_VER > 0x00030500
    LogHpi(HPI_InStreamReadBuf(NULL,hpi_stream,pdata,data_recorded));
#else
    LogHpi(HPI_DataCreate(&hpi_data,&format,pdata,data_recorded));
    LogHpi(HPI_InStreamRead(NULL,hpi_stream,&hpi_data));
#endif
    if(is_recording) {
      writeWave(pdata,data_recorded);
    }
  }
  emit position(samples_recorded);
  if(debug) {
    printf("MLHPIRecordStream: emitted position(%u)\n",
	   (unsigned)samples_recorded);
  }
}


bool MLHPIRecordStream::GetStream()
{
  if(LogHpi(HPI_InStreamOpen(NULL,card_index[card_number],stream_number,
			     &hpi_stream))!=0) {
    return false;
  }
  return true;
}


void MLHPIRecordStream::FreeStream()
{
  LogHpi(HPI_InStreamClose(NULL,hpi_stream));
}


hpi_err_t MLHPIRecordStream::LogHpi(hpi_err_t err)
{
  char err_txt[200];

  if(err!=0) {
    HPI_GetErrorText(err,err_txt);
    syslog(LOG_NOTICE,"HPI Error: %s",err_txt);
  }
  return err;
}
