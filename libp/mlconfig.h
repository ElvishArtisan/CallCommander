// mlconfig.h
//
// A container class for CallCommander ml(1) Configuration
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

#ifndef MLCONFIG_H
#define MLCONFIG_H

#include <vector>

#include <QtCore/QString>
#include <QtGui/QPalette>

#include <mlwavefile.h>
#include <mlsegclock.h>
#include <mlprofile.h>

#include <ml.h>
#include <mldconfig.h>

using namespace std;

//
// Default Configuration Values
//
#define DEFAULT_ML_CONFIG_FILE "/etc/ml.conf"
#define DEFAULT_WATCHDOG_INTERVAL 10
#define DEFAULT_TCP_PORT 5001
#define DEFAULT_SWITCH_PROTOCOL ""
#define DEFAULT_CALLERID_PROTOCOL "native"


class MlConfig
{
 public:
  enum ColorRole {IdleColor=0,InUseColor=1,OnAirColor=2,OffHookColor=3,
		  OnHoldColor=4,ScreenedHoldColor=5,ReadyNextColor=6,
		  RingingColor=7,BusyColor=8,CallerBoxColor=9,
		  MessageBoxColor=10,DumpButtonColor=11,RecordButtonColor=12,
		  NextButtonColor=13,KeypadButtonColor=14,ScreenButtonColor=15,
		  MuteButtonColor=16,ConfButtonColor=17,VipButtonColor=18,
		  ClientButtonColor=19,ColorRoleSize=20};
  enum ButtonRole {BusyButton=0,DumpButton=1,RecordButton=2,NextButton=3,
		   KeypadButton=4,FlashButton=5,ScreenButton=6,
		   MuteButton=7,ConfButton=8,VipButton=9,ClientButton=10,
		   ButtonRoleSize=11};
  enum CallerFields {CallerName=0,CallerAge=1,CallerCityState=2,
		     CallerZip=3,CallerPhone=4,CallerStation=5,
		     CallerGender=6,CallerComment=7,CallerCell=8,
		     CallerQuality=9,CallerTimer=10,CallerCallCounter=11,
		     CallerSize=12};
  enum AudioInputType {Analog=0,AesEbu=1};
  enum AudioFileType {Wave=0,Raw=1};
  MlConfig();
  MlConfig(QString filename);
  QString filename() const;
  void setFilename(QString filename);

  //
  // [Global] Section
  //
  QString connectionName() const;
  bool animateIcons() const;
  QRect window() const;
  bool keepOnTop() const;
  bool allowClientKill() const;
  QString spawnProcess() const;
  bool spawnOnce() const;

  //
  // [ConnectionManager] Section
  //
  bool disableNewButton() const;
  bool disableEditButton() const;
  bool disableDeleteButton() const;
  bool disableOpenButton() const;

  //
  // [Controls]
  //
  bool disableLineButtons() const;
  bool disableLineBoxes() const;

  //
  // [Skimmer] Section
  //
  QString skimmerAudioRoot() const;
  int skimmerSampleRate() const;
  int skimmerChannels() const;
  int skimmerBitrate() const;
  MLWaveFile::Format skimmerFormat() const;
  int skimmerCardNum(int bank) const;
  int skimmerPortNum(int bank) const;
  QString skimmerFilename(int bank) const;
  MlConfig::AudioFileType skimmerFileType(int bank) const;
  QString skimmerPostCommand(int bank) const;
  int skimmerMaxLength(int bank) const;
  int skimmerInputLevel(int bank) const;
  AudioInputType skimmerInputType(int bank) const;
  bool skimmerEnable(int bank,int line) const;
  void setSkimmerEnable(int bank,int line,bool state);

  //
  // [Colors] Section
  //
  QPalette palette(ColorRole role) const;
  QColor color(ColorRole role) const;

  //
  // [MessageBox] Section
  //
  bool msgboxShow() const;
  QRect msgboxWindow() const;
  QString msgboxNickname() const;
  int msgboxSendLines() const;
  int msgboxScrollbackLines() const;
  int msgboxHangTime() const;
  bool msgboxPreserveText() const;
  QFont msgboxFont() const;

  //
  // [Clock] Section
  //
  Qt::Orientation clockOrientation() const;
  bool clockShowClock() const;
  int clockTimeOffset() const;
  bool clockSendTimeOffset() const;
  QPoint clockPosition() const;
  bool clockShowHour() const;
  bool clockMilitaryFormat() const;

  //
  // [Segments] Format
  //
  MLSegList *segmentList();
  std::vector<MLSegment *> *segments();

  //
  // Button Sections
  //
  bool buttonShow(ButtonRole role) const;
  QPoint buttonPosition(ButtonRole role) const;
  QString buttonKey(ButtonRole role) const;
  bool buttonShowDelay(ButtonRole role) const;
  int buttonDelayThreshold(ButtonRole role) const;

  //
  // [MainBank] and [ConfBank] Sections
  //
  bool bankShowHoldButton(int bank) const;
  QPoint bankHoldPosition(int bank) const;
  QString bankHoldKey(int bank) const;
  bool bankShowDropButton(int bank) const;
  QPoint bankDropPosition(int bank) const;
  QString bankDropKey(int bank) const;

  //
  // [CallerBox] Section
  //
  QSize callerSize() const;
  bool callerShowFrame() const;
  bool callerAllowScrolling() const;
  bool callerEnableWarningMessages() const;
  bool callerShow(CallerFields field) const;
  QRect callerWindow(CallerFields field) const;
  QFont callerFont(CallerFields field) const;
  int callerLines(CallerFields field) const;

  //
  // [LineN] Sections
  //
  bool lineShow(int bank,int line) const;
  QPoint linePosition(int bank,int line) const;
  QString lineKey(int bank,int line) const;
  QString lineLabel(int line) const;
  int lineFontSize(int line) const;
  bool lineShowCallerBox(int line) const;
  QRect lineCallerBoxWindow(int line) const;
  QString lineCallerKey(int line) const;

  //
  // [Debug] Section
  //
  bool disableWatchdog() const;
  bool disableStatusSync() const;
  bool disableCallerSync() const;
  bool disableMessageSync() const;
  bool disableAutoDetect() const;

  void dumpConfig(FILE *stream);
  void load();
  void clear();

 private:
  void LoadCallInfoField(MLProfile *p,CallerFields field,const QString &tag);
  QString SetFontWeight(int weight);
  QString ml_filename;
  QString ml_connection_name;
  bool ml_animate_icons;
  bool ml_ask_host;
  QString ml_caption;
  QRect ml_window;
  bool ml_keep_on_top;
  bool ml_allow_client_kill;
  QString ml_spawn_process;
  bool ml_spawn_once;

  bool ml_disable_new_button;
  bool ml_disable_edit_button;
  bool ml_disable_delete_button;
  bool ml_disable_open_button;

  bool ml_disable_line_buttons;
  bool ml_disable_line_boxes;

  QString ml_skimmer_audio_root;
  int ml_skimmer_sample_rate;
  int ml_skimmer_channels;
  int ml_skimmer_bitrate;
  MLWaveFile::Format ml_skimmer_format;
  int ml_skimmer_card[MAX_BANKS];
  int ml_skimmer_port[MAX_BANKS];
  QString ml_skimmer_filename[MAX_BANKS];
  MlConfig::AudioFileType ml_skimmer_audio_file_type[MAX_BANKS];
  QString ml_skimmer_post_command[MAX_BANKS];
  int ml_skimmer_max_length[MAX_BANKS];
  int ml_skimmer_level[MAX_BANKS];
  MlConfig::AudioInputType ml_skimmer_input_type[MAX_BANKS];
  bool ml_skimmer_enable[MAX_BANKS][MAX_LINES];

  QPalette ml_palette[MlConfig::ColorRoleSize];
  QColor ml_color[MlConfig::ColorRoleSize];

  bool ml_msgbox_show;
  QRect ml_msgbox_window;
  QString ml_msgbox_nickname;
  int ml_msgbox_send_lines;
  int ml_msgbox_scrollback_lines;
  int ml_msgbox_hangtime;
  bool ml_msgbox_preserve_text;
  QFont ml_msgbox_font;

  Qt::Orientation ml_clock_orientation;
  bool ml_clock_show_clock;
  int ml_clock_time_offset;
  bool ml_clock_send_time_offset;
  QPoint ml_clock_position;
  bool ml_clock_show_hour;
  bool ml_clock_military_format;

  MLSegList *ml_seglist;

  bool ml_button_show[MlConfig::ButtonRoleSize];
  QPoint ml_button_position[MlConfig::ButtonRoleSize];
  QString ml_button_key[MlConfig::ButtonRoleSize];
  bool ml_button_show_delay[MlConfig::ButtonRoleSize];
  int ml_button_delay_threshold[MlConfig::ButtonRoleSize];

  bool ml_bank_show_hold_button[MAX_BANKS];
  QPoint ml_bank_hold_position[MAX_BANKS];
  QString ml_bank_hold_key[MAX_BANKS];
  bool ml_bank_show_drop_button[MAX_BANKS];
  QPoint ml_bank_drop_position[MAX_BANKS];
  QString ml_bank_drop_key[MAX_BANKS];

  QSize ml_caller_size;
  bool ml_caller_show_frame;
  bool ml_caller_enable_warning_messages;
  bool ml_caller_allow_scrolling;
  bool ml_caller_show[MlConfig::CallerSize];
  QRect ml_caller_window[MlConfig::CallerSize];
  QFont ml_caller_font[MlConfig::CallerSize];
  int ml_caller_lines[MlConfig::CallerSize];

  bool ml_line_show[MAX_BANKS][MAX_LINES];
  QPoint ml_line_position[MAX_BANKS][MAX_LINES];
  QString ml_line_key[MAX_BANKS][MAX_LINES];
  QString ml_line_label[MAX_LINES];
  int ml_line_font_size[MAX_LINES];
  bool ml_line_show_caller_box[MAX_LINES];
  QRect ml_line_caller_box_window[MAX_LINES];
  QString ml_line_caller_key[MAX_LINES];

  bool ml_disable_watchdog;
  bool ml_disable_status_sync;
  bool ml_disable_caller_sync;
  bool ml_disable_message_sync;
  bool ml_disable_auto_detect;
};


#endif  // MLCONFIG_H
