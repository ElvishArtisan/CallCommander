//   mlhpiplaystream.h
//
//   A class for playing Microsoft WAV files.
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlhpiplaystream.h,v 1.4 2012/06/26 21:30:02 cvs Exp $
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

#ifndef MLHPIPLAYSTREAM_H
#define MLHPIPLAYSTREAM_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QtCore/QObject>
#include <QtGui/QWidget>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QTimer>
#include <QtCore/QDateTime>

#include <mlwavefile.h>
#include <mlhpisoundcard.h>

#include <asihpi/hpi.h>
#ifndef HPI_VER
#include <asihpi/hpi_version.h>
#endif

#define MAX_FRAGMENT_SIZE 192000
#define FRAGMENT_TIME 50
#define TIMESCALE_LOW_LIMIT 83300
#define TIMESCALE_HIGH_LIMIT 125000
#define ML_TIMESCALE_DIVISOR 100000.0
//#define RPLAYSTREAM_SHOW_SLOTS yes

class MLHPIPlayStream : public QObject,public MLWaveFile
{
  Q_OBJECT

 public:
  enum State {Stopped=0,Playing=1,Paused=2};
  enum Error {Ok=0,NoFile=1,NoStream=2,AlreadyOpen=3};
  MLHPIPlayStream(MLHPISoundCard *card,QWidget *parent=0,const char *name=0);
  ~MLHPIPlayStream();
  QString errorString(MLHPIPlayStream::Error err);
  bool formatSupported(MLWaveFile::Format format);
  bool formatSupported();
  MLHPIPlayStream::Error openWave();
  MLHPIPlayStream::Error openWave(QString filename);
  void closeWave();
  int getCard() const;
  int getStream() const;
  int getSpeed() const;
  bool setSpeed(int speed,bool pitch=false,bool rate=false);
  MLHPIPlayStream::State getState() const;
  
 signals:
  void isStopped(bool state);
  void played();
  void paused();
  void stopped();
  void position(int samples);
  void stateChanged(int card,int stream,int state);

 public slots:
  void setCard(int card);
  bool play();
  void pause();
  void stop();
  int currentPosition();
  bool setPosition(unsigned samples);
  void setPlayLength(int length);
  void tickClock();
  
 private:
  void Drained();
  int GetStream();
  void FreeStream();
  MLHPISoundCard *sound_card;
  MLHPIPlayStream::State stream_state;
  QString wave_name;
  QTimer *clock;
  uint32_t card_index[HPI_MAX_ADAPTERS];
  int card_number;
  int stream_number;
  bool is_open;
  bool playing;
  bool is_paused;
  bool repositioned;
  bool stopping;
  unsigned audio_ptr;
  unsigned left_to_write;
  unsigned read_bytes;
  unsigned samples_skipped;
  int play_length;
  QTimer *play_timer;
  QTime start_time;
  int play_speed;
  bool pitch_can_vary;
  bool rate_can_vary;
  hpi_handle_t hpi_stream;
  uint16_t state;
  uint32_t buffer_size;
  uint32_t data_to_play;
  uint32_t samples_played;
  uint32_t reserved;
  int fragment_time;
  uint8_t *pdata;
#if HPI_VER < 0x030a00
  HPI_FORMAT format;
#else
  struct hpi_format format;
#endif
#if HPI_VER < 0x00030500
  HPI_DATA hpi_data;
#endif
  uint32_t fragment_size;
  bool restart_transport;
  int samples_pending;
  unsigned current_position;
};


#endif  // MLHPIPLAYSTREAM_H
