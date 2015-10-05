// mlconfig.cpp
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

#include <stdlib.h>
#include <stdio.h>

#include "mlconf.h"
#include "mlconfig.h"

MlConfig::MlConfig()
{
  ml_seglist=new MLSegList();
  clear();
}


MlConfig::MlConfig(QString filename)
{
  ml_seglist=new MLSegList();
  clear();
  ml_filename=filename;
}


QString MlConfig::filename() const
{
  return ml_filename;
}


void MlConfig::setFilename(QString filename)
{
  ml_filename=filename;
}


QString MlConfig::connectionName() const
{
  return ml_connection_name;
}


bool MlConfig::animateIcons() const
{
  return ml_animate_icons;
}


QRect MlConfig::window() const
{
  return ml_window;
}


bool MlConfig::keepOnTop() const
{
  return ml_keep_on_top;
}


bool MlConfig::allowClientKill() const
{
  return ml_allow_client_kill;
}


QString MlConfig::spawnProcess() const
{
  return ml_spawn_process;
}


bool MlConfig::spawnOnce() const
{
  return ml_spawn_once;
}


bool MlConfig::disableNewButton() const
{
  return ml_disable_new_button;
}


bool MlConfig::disableEditButton() const
{
  return ml_disable_edit_button;
}


bool MlConfig::disableDeleteButton() const
{
  return ml_disable_delete_button;
}


bool MlConfig::disableOpenButton() const
{
  return ml_disable_open_button;
}


bool MlConfig::disableLineButtons() const
{
  return ml_disable_line_buttons;
}


bool MlConfig::disableLineBoxes() const
{
  return ml_disable_line_boxes;
}


QString MlConfig::skimmerAudioRoot() const
{
  return ml_skimmer_audio_root;
}


int MlConfig::skimmerSampleRate() const
{
  return ml_skimmer_sample_rate;
}


int MlConfig::skimmerChannels() const
{
  return ml_skimmer_channels;
}


int MlConfig::skimmerBitrate() const
{
  return ml_skimmer_bitrate;
}


MLWaveFile::Format MlConfig::skimmerFormat() const
{
  return ml_skimmer_format;
}


int MlConfig::skimmerCardNum(int bank) const
{
  return ml_skimmer_card[bank];
}


int MlConfig::skimmerPortNum(int bank) const
{
  return ml_skimmer_port[bank];
}


QString MlConfig::skimmerFilename(int bank) const
{
  return ml_skimmer_filename[bank];
}


MlConfig::AudioFileType MlConfig::skimmerFileType(int bank) const
{
  return ml_skimmer_audio_file_type[bank];
}


QString MlConfig::skimmerPostCommand(int bank) const
{
  return ml_skimmer_post_command[bank];
}


int MlConfig::skimmerMaxLength(int bank) const
{
  return ml_skimmer_max_length[bank];
}


int MlConfig::skimmerInputLevel(int bank) const
{
  return ml_skimmer_level[bank];
}


MlConfig::AudioInputType MlConfig::skimmerInputType(int bank) const
{
  return ml_skimmer_input_type[bank];
}


bool MlConfig::skimmerEnable(int bank,int line) const
{
  return ml_skimmer_enable[bank][line];
}


void MlConfig::setSkimmerEnable(int bank,int line,bool state)
{
  ml_skimmer_enable[bank][line]=state;
}


QColor MlConfig::color(ColorRole role) const
{
  return ml_color[role];
}


QPalette MlConfig::palette(ColorRole role) const
{
  return ml_palette[role];
}


bool MlConfig::msgboxShow() const
{
  return ml_msgbox_show;
}


QRect MlConfig::msgboxWindow() const
{
  return ml_msgbox_window;
}


QString MlConfig::msgboxNickname() const
{
  return ml_msgbox_nickname;
}


int MlConfig::msgboxSendLines() const
{
  return ml_msgbox_send_lines;
}


int MlConfig::msgboxScrollbackLines() const
{
  return ml_msgbox_scrollback_lines;
}


int MlConfig::msgboxHangTime() const
{
  return ml_msgbox_hangtime;
}


bool MlConfig::msgboxPreserveText() const
{
  return ml_msgbox_preserve_text;
}


QFont MlConfig::msgboxFont() const
{
  return ml_msgbox_font;
}


Qt::Orientation MlConfig::clockOrientation() const
{
  return ml_clock_orientation;
}


bool MlConfig::clockShowClock() const
{
  return ml_clock_show_clock;
}


int MlConfig::clockTimeOffset() const
{
  return ml_clock_time_offset;
}


bool MlConfig::clockSendTimeOffset() const
{
  return ml_clock_send_time_offset;
}


QPoint MlConfig::clockPosition() const
{
  return ml_clock_position;
}


bool MlConfig::clockShowHour() const
{
  return ml_clock_show_hour;
}


bool MlConfig::clockMilitaryFormat() const
{
  return ml_clock_military_format;
}


MLSegList *MlConfig::segmentList()
{
  return ml_seglist;
}


bool MlConfig::buttonShow(ButtonRole role) const
{
  return ml_button_show[role];
}


QPoint MlConfig::buttonPosition(ButtonRole role) const
{
  return ml_button_position[role];
}


QString MlConfig::buttonKey(ButtonRole role) const
{
  return ml_button_key[role];
}


bool MlConfig::buttonShowDelay(ButtonRole role) const
{
  return ml_button_show_delay[role];
}


int MlConfig::buttonDelayThreshold(ButtonRole role) const
{
  return ml_button_delay_threshold[role];
}


bool MlConfig::bankShowHoldButton(int bank) const
{
  return ml_bank_show_hold_button[bank];
}


QPoint MlConfig::bankHoldPosition(int bank) const
{
  return ml_bank_hold_position[bank];
}


QString MlConfig::bankHoldKey(int bank) const
{
  return ml_bank_hold_key[bank];
}


bool MlConfig::bankShowDropButton(int bank) const
{
  return ml_bank_show_drop_button[bank];
}


QPoint MlConfig::bankDropPosition(int bank) const
{
  return ml_bank_drop_position[bank];
}


QString MlConfig::bankDropKey(int bank) const
{
  return ml_bank_drop_key[bank];
}


QSize MlConfig::callerSize() const
{
  return ml_caller_size;
}


bool MlConfig::callerShowFrame() const
{
  return ml_caller_show_frame;
}


bool MlConfig::callerEnableWarningMessages() const
{
  return ml_caller_enable_warning_messages;
}


bool MlConfig::callerAllowScrolling() const
{
  return ml_caller_allow_scrolling;
}


bool MlConfig::callerShow(CallerFields field) const
{
  return ml_caller_show[field];
}


QRect MlConfig::callerWindow(CallerFields field) const
{
  return ml_caller_window[field];
}


QFont MlConfig::callerFont(CallerFields field) const
{
  return ml_caller_font[field];
}


int MlConfig::callerLines(CallerFields field) const
{
  return ml_caller_lines[field];
}


bool MlConfig::lineShow(int bank,int line) const
{
  return ml_line_show[bank][line];
}


QPoint MlConfig::linePosition(int bank,int line) const
{
  return ml_line_position[bank][line];
}


QString MlConfig::lineKey(int bank,int line) const
{
  return ml_line_key[bank][line];
}


QString MlConfig::lineLabel(int bank) const
{
  return ml_line_label[bank];
}


int MlConfig::lineFontSize(int line) const
{
  return ml_line_font_size[line];
}


bool MlConfig::lineShowCallerBox(int line) const
{
  return ml_line_show_caller_box[line];
}


QRect MlConfig::lineCallerBoxWindow(int line) const
{
  return ml_line_caller_box_window[line];
}


QString MlConfig::lineCallerKey(int line) const
{
  return ml_line_caller_key[line];
}


bool MlConfig::disableWatchdog() const
{
  return ml_disable_watchdog;
}


bool MlConfig::disableStatusSync() const
{
  return ml_disable_status_sync;
}


bool MlConfig::disableCallerSync() const
{
  return ml_disable_caller_sync;
}


bool MlConfig::disableMessageSync() const
{
  return ml_disable_message_sync;
}


bool MlConfig::disableAutoDetect() const
{
  return ml_disable_auto_detect;
}


void MlConfig::dumpConfig(FILE *stream)
{
}


void MlConfig::load()
{
  int dy=0;
  QString str;
  QString face;
  QString weight;
  QString section;
  int size;
  QString filename=ml_filename;
  QString bankname;
  QString temp;

  clear();
  ml_filename=filename;
  
  //
  // Load the Parser
  //
  MLProfile *parser=new MLProfile();
  parser->setSource(ml_filename);

  //
  // [Global] Section
  //
  ml_connection_name=parser->stringValue("Global","ConnectionName");
  ml_animate_icons=
    parser->boolValue("Global","AnimateIcons",ml_animate_icons);
#ifdef WIN32
  dy=20;
#endif
  ml_ask_host=parser->boolValue("Global","AskHost",true);
  ml_window=QRect(parser->intValue("Global","WindowX",ml_window.x()),
		  parser->intValue("Global","WindowY",ml_window.y())+dy,
		  parser->intValue("Global","SizeX",ml_window.width()),
		  parser->intValue("Global","SizeY",ml_window.height()));
  ml_keep_on_top=parser->boolValue("Global","KeepOnTop",ml_keep_on_top);
  ml_allow_client_kill=parser->boolValue("Global","AllowClientKill",
					 ml_allow_client_kill);
  ml_spawn_process=parser->stringValue("Global","SpawnProcess",
				       ml_spawn_process);
  ml_spawn_once=parser->boolValue("Global","SpawnOnce",ml_spawn_once);

  //
  // [ConnectionManager] Section
  //
  ml_disable_new_button=
    parser->boolValue("ConnectionManager","DisableNewButton",
		      ml_disable_new_button);
  ml_disable_edit_button=
    parser->boolValue("ConnectionManager","DisableEditButton",
		      ml_disable_edit_button);
  ml_disable_delete_button=
    parser->boolValue("ConnectionManager","DisableDeleteButton",
		      ml_disable_delete_button);
  ml_disable_open_button=
    parser->boolValue("ConnectionManager","DisableOpenButton",
		      ml_disable_open_button);

  //
  // [Debug] Section
  //
  ml_disable_watchdog=
    parser->boolValue("Debug","DisableWatchdog",ml_disable_watchdog);
  ml_disable_status_sync=
    parser->boolValue("Debug","DisableStatusSync",ml_disable_status_sync);
  ml_disable_caller_sync=
    parser->boolValue("Debug","DisableCallerSync",ml_disable_caller_sync);
  ml_disable_message_sync=
    parser->boolValue("Debug","DisableMessageSync",ml_disable_message_sync);
  ml_disable_auto_detect=
    parser->boolValue("Debug","DisableAutoDetect",ml_disable_auto_detect);

  //
  // [Controls] Section
  //
  ml_disable_line_buttons=
    parser->boolValue("Controls","DisableLineButtons",ml_disable_line_buttons);
  ml_disable_line_boxes=
    parser->boolValue("Controls","DisableLineBoxes",ml_disable_line_boxes);

  //
  // Skimmer Section
  //
  for(int i=0;i<MAX_BANKS;i++) {
    if(i==0) {
      bankname="Main";
    }
    else {
      bankname="Conf";
    }
    ml_skimmer_card[i]=parser->
      intValue("Skimmer",bankname+"Card",ml_skimmer_card[i]);
    ml_skimmer_port[i]=parser->
      intValue("Skimmer",bankname+"Stream",i);
    ml_skimmer_port[i]=parser->
      intValue("Skimmer",bankname+"Port",ml_skimmer_port[i]);
    ml_skimmer_filename[i]=parser->
      stringValue("Skimmer",bankname+"Filename",ml_skimmer_filename[i]);
    ml_skimmer_post_command[i]=parser->
      stringValue("Skimmer",bankname+"PostCommand",ml_skimmer_post_command[i]);
    ml_skimmer_max_length[i]=
      MLSetTimeLength(parser->stringValue("Skimmer",bankname+"MaxLength",
					 "00:00"));
    ml_skimmer_level[i]=parser->intValue("Skimmer",bankname+"Level",
					 ml_skimmer_level[i]);
    if(i==0) {
      temp=parser->
	stringValue("Skimmer","MainFileType","wave").lower();
    }
    else {
      temp=parser->
	stringValue("Skimmer","ConfFileType","wave").lower();
    }
    if(temp=="raw") {
      ml_skimmer_audio_file_type[i]=MlConfig::Raw;
    }
    else {
      ml_skimmer_audio_file_type[i]=MlConfig::Wave;
    }
    if(i==0) {
      temp=parser->
	stringValue("Skimmer","MainInputType","digital").lower();
    }
    else {
      temp=parser->
	stringValue("Skimmer","ConfInputType","digital").lower();
    }
    if(temp=="digital") {
      ml_skimmer_input_type[i]=MlConfig::AesEbu;
    }
    else {
      ml_skimmer_input_type[i]=MlConfig::Analog;
    }
    for(int j=0;j<MAX_LINES;j++) {
      temp=QString().sprintf("Enable%s%d",(const char *)bankname,j+1);
      ml_skimmer_enable[i][j]=parser->boolValue("Skimmer",temp,false);
    }
  }
  temp=parser->
    stringValue("Skimmer","AudioRoot",ml_skimmer_audio_root);
  if(temp.left(1)=="/") {
    ml_skimmer_audio_root=temp;
  }
  else {
    if(getenv("HOME")!=NULL) {
      ml_skimmer_audio_root=QString(getenv("HOME"))+QString("/")+temp;
    }
    else {
      ml_skimmer_audio_root=QString("/")+temp;
    }
  }
  if(ml_skimmer_audio_root.right(1)!="/") {
    ml_skimmer_audio_root+="/";
  }
  ml_skimmer_sample_rate=parser->intValue("Skimmer","SampleRate",
					  ml_skimmer_sample_rate);
  ml_skimmer_channels=parser->intValue("Skimmer","Channels",
				       ml_skimmer_channels);
  ml_skimmer_bitrate=parser->intValue("Skimmer","BitRate",
				      ml_skimmer_bitrate);
  temp=parser->stringValue("Skimmer","Format","pcm16");
  if(temp.lower()=="pcm16") {
    ml_skimmer_format=MLWaveFile::Pcm16;
  }
  if(temp.lower()=="layer2") {
    ml_skimmer_format=MLWaveFile::MpegL2;
  }
  if(temp.lower()=="layer3") {
    ml_skimmer_format=MLWaveFile::MpegL3;
  }

  //
  // Colors Section
  //
  str=parser->stringValue("Colors","Idle","lightGray");
  ml_palette[MlConfig::IdleColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::IdleColor].
    setColor(QPalette::ButtonText,QColor("black"));
  ml_palette[MlConfig::IdleColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","InUse","darkGray");
  ml_palette[MlConfig::InUseColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::InUseColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::InUseColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","OnAir","red");
  ml_palette[MlConfig::OnAirColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::OnAirColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::OnAirColor].
    setColor(QPalette::Highlight,QColor("black"));

  ml_palette[MlConfig::OnAirColor].setColor(QPalette::Base,Qt::red);


  str=parser->stringValue("Colors","OffHook","yellow");
  ml_palette[MlConfig::OffHookColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::OffHookColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::OffHookColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","OnHold","green");
  ml_palette[MlConfig::OnHoldColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::OnHoldColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::OnHoldColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","ScreenedHold","green");
  ml_palette[MlConfig::ScreenedHoldColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::ScreenedHoldColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ScreenedHoldColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","ReadyNext","green");
  ml_palette[MlConfig::ReadyNextColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::ReadyNextColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ReadyNextColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","Ringing","blue");
  ml_palette[MlConfig::RingingColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::RingingColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::RingingColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","Busy","yellow");
  ml_palette[MlConfig::BusyColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::BusyColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::BusyColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","CallerBox","white");
  ml_palette[MlConfig::CallerBoxColor]=QColor(str);
  ml_palette[MlConfig::CallerBoxColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","MessageBox","white");
  ml_palette[MlConfig::MessageBoxColor]=QColor(str);
  ml_palette[MlConfig::MessageBoxColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","DumpButton","red");
  ml_palette[MlConfig::DumpButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::DumpButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::DumpButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","RecordButton","gray");
  ml_palette[MlConfig::RecordButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::RecordButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::RecordButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","NextButton","gray");
  ml_palette[MlConfig::NextButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::NextButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::NextButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","KeypadButton","gray");
  ml_palette[MlConfig::KeypadButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::KeypadButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::KeypadButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","ScreenButton","red");
  ml_palette[MlConfig::ScreenButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::ScreenButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ScreenButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","MuteButton","red");
  ml_palette[MlConfig::MuteButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::MuteButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::MuteButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","ConfButton","red");
  ml_palette[MlConfig::ConfButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::ConfButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ConfButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","VipButton","red");
  ml_palette[MlConfig::VipButtonColor]=QPalette(QColor(str),QColor("black"));
  ml_palette[MlConfig::VipButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::VipButtonColor].
    setColor(QPalette::Highlight,QColor("black"));
  str=parser->stringValue("Colors","ClientButton","red");
  ml_palette[MlConfig::ClientButtonColor]=QPalette(QColor(str),
						   QColor("black"));
  ml_palette[MlConfig::ClientButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ClientButtonColor].
    setColor(QPalette::Highlight,QColor("black"));

  //
  // MessageBox Section
  //
  ml_msgbox_show=
    parser->boolValue("MessageBox","ShowMessageBox",ml_msgbox_show);
  ml_msgbox_window=
    QRect(parser->intValue("MessageBox","MessageBoxX",ml_msgbox_window.x()),
	  parser->intValue("MessageBox","MessageBoxY",ml_msgbox_window.y()),
	  parser->intValue("MessageBox","SizeX",ml_msgbox_window.width()),
	  parser->intValue("MessageBox","SizeY",ml_msgbox_window.height()));
  ml_msgbox_send_lines=parser->intValue("MessageBox","SendLines",
					ml_msgbox_send_lines);
  ml_msgbox_scrollback_lines=parser->intValue("MessageBox","ScrollBackLines",
					      ml_msgbox_scrollback_lines);
  str=parser->stringValue("MessageBox","Nickname","");
  if(str.isEmpty()) {
    if(getenv("USER")!=NULL) {
      ml_msgbox_nickname=QString(getenv("USER"));
    }
    else {
      ml_msgbox_nickname="user";
    }
  }
  else {
    ml_msgbox_nickname=str;
  }
  ml_msgbox_hangtime=
    parser->intValue("MessageBox","HangTime",ml_msgbox_hangtime);
  ml_msgbox_preserve_text=parser->boolValue("MessageBox","PreserveText");
  face=parser->stringValue("MessageBox","FontFace",ml_msgbox_font.family());
  size=parser->intValue("MessageBox","FontSize",12);
  weight=parser->stringValue("MessageBox","FontWeight",
			     SetFontWeight(ml_msgbox_font.weight()));
  ml_msgbox_font=QFont(face,size,MLGetFontWeight(weight));
  ml_msgbox_font.setPixelSize(size);
  
  //
  // Clock Section
  //
  ml_clock_show_clock=
    parser->boolValue("Clock","ShowClock",ml_clock_show_clock);
  ml_clock_position=
    QPoint(parser->intValue("Clock","ClockX",ml_clock_position.x()),
	   parser->intValue("Clock","ClockY",ml_clock_position.y()));
  ml_clock_time_offset=
    parser->intValue("Clock","TimeOffset",ml_clock_time_offset);
  ml_clock_send_time_offset=
    parser->boolValue("Clock","SendTimeOffset",ml_clock_send_time_offset);
  ml_clock_show_hour=parser->boolValue("Clock","ShowHour",ml_clock_show_hour);
  ml_clock_military_format=
    parser->boolValue("Clock","MilitaryFormat",ml_clock_military_format);
  str=parser->stringValue("Clock","Orientation");
  if(str.lower()=="horizontal") {
    ml_clock_orientation=Qt::Horizontal;
  }

  //
  // Busy Button Section
  //
  ml_button_show[MlConfig::BusyButton]=
    parser->boolValue("BusyButton","ShowButton",
		      ml_button_show[MlConfig::BusyButton]);
  ml_button_position[MlConfig::BusyButton]=
    QPoint(parser->intValue("BusyButton","ButtonX",
			    ml_button_position[MlConfig::BusyButton].x()),
	   parser->intValue("BusyButton","ButtonY",
			    ml_button_position[MlConfig::BusyButton].y()));
  ml_button_key[MlConfig::BusyButton]=
    parser->stringValue("BusyButton","Key",
			ml_button_key[MlConfig::BusyButton]);

  //
  // Dump Button Section
  //
  ml_button_show[MlConfig::DumpButton]=
    parser->boolValue("DumpButton","ShowButton",
		      ml_button_show[MlConfig::DumpButton]);
  ml_button_position[MlConfig::DumpButton]=
    QPoint(parser->intValue("DumpButton","ButtonX",
			    ml_button_position[MlConfig::DumpButton].x()),
	   parser->intValue("DumpButton","ButtonY",
			    ml_button_position[MlConfig::DumpButton].y()));
  ml_button_key[MlConfig::DumpButton]=
    parser->stringValue("DumpButton","Key",
			ml_button_key[MlConfig::DumpButton]);
  ml_button_show_delay[MlConfig::DumpButton]=
    parser->boolValue("DumpButton","ShowDelay",
		      ml_button_show_delay[MlConfig::DumpButton]);
  ml_button_delay_threshold[MlConfig::DumpButton]=
    parser->intValue("DumpButton","DelayThreshold",
		      ml_button_delay_threshold[MlConfig::DumpButton]);

  //
  // Record Button Section
  //
  ml_button_show[MlConfig::RecordButton]=
    parser->boolValue("RecordButton","ShowButton",
		      ml_button_show[MlConfig::RecordButton]);
  ml_button_position[MlConfig::RecordButton]=
    QPoint(parser->intValue("RecordButton","ButtonX",
			    ml_button_position[MlConfig::RecordButton].x()),
	   parser->intValue("RecordButton","ButtonY",
			    ml_button_position[MlConfig::RecordButton].y()));
  ml_button_key[MlConfig::RecordButton]=
    parser->stringValue("RecordButton","Key",
			ml_button_key[MlConfig::RecordButton]);

  //
  // Next Button Section
  //
  ml_button_show[MlConfig::NextButton]=
    parser->boolValue("NextButton","ShowButton",
		      ml_button_show[MlConfig::NextButton]);
  ml_button_position[MlConfig::NextButton]=
    QPoint(parser->intValue("NextButton","ButtonX",
			    ml_button_position[MlConfig::NextButton].x()),
	   parser->intValue("NextButton","ButtonY",
			    ml_button_position[MlConfig::NextButton].y()));
  ml_button_key[MlConfig::NextButton]=
    parser->stringValue("NextButton","Key",
			ml_button_key[MlConfig::NextButton]);

  //
  // Keypad Button Section
  //
  ml_button_show[MlConfig::KeypadButton]=
    parser->boolValue("KeypadButton","ShowButton",
		      ml_button_show[MlConfig::KeypadButton]);
  ml_button_position[MlConfig::KeypadButton]=
    QPoint(parser->intValue("KeypadButton","ButtonX",
			    ml_button_position[MlConfig::KeypadButton].x()),
	   parser->intValue("KeypadButton","ButtonY",
			    ml_button_position[MlConfig::KeypadButton].y()));
  ml_button_key[MlConfig::KeypadButton]=
    parser->stringValue("KeypadButton","Key",
			ml_button_key[MlConfig::KeypadButton]);

  //
  // Flash Button Section
  //
  ml_button_show[MlConfig::FlashButton]=
    parser->boolValue("FlashButton","ShowButton",
		      ml_button_show[MlConfig::FlashButton]);
  ml_button_position[MlConfig::FlashButton]=
    QPoint(parser->intValue("FlashButton","ButtonX",
			    ml_button_position[MlConfig::FlashButton].x()),
	   parser->intValue("FlashButton","ButtonY",
			    ml_button_position[MlConfig::FlashButton].y()));
  ml_button_key[MlConfig::FlashButton]=
    parser->stringValue("FlashButton","Key",
			ml_button_key[MlConfig::FlashButton]);

  //
  // Screen Button Section
  //
  ml_button_show[MlConfig::ScreenButton]=
    parser->boolValue("ScreenButton","ShowButton",
		      ml_button_show[MlConfig::ScreenButton]);
  ml_button_position[MlConfig::ScreenButton]=
    QPoint(parser->intValue("ScreenButton","ButtonX",
			    ml_button_position[MlConfig::ScreenButton].x()),
	   parser->intValue("ScreenButton","ButtonY",
			    ml_button_position[MlConfig::ScreenButton].y()));
  ml_button_key[MlConfig::ScreenButton]=
    parser->stringValue("ScreenButton","Key",
			ml_button_key[MlConfig::ScreenButton]);

  //
  // Mute Button Section
  //
  ml_button_show[MlConfig::MuteButton]=
    parser->boolValue("MuteButton","ShowButton",
		      ml_button_show[MlConfig::MuteButton]);
  ml_button_position[MlConfig::MuteButton]=
    QPoint(parser->intValue("MuteButton","ButtonX",
			    ml_button_position[MlConfig::MuteButton].x()),
	   parser->intValue("MuteButton","ButtonY",
			    ml_button_position[MlConfig::MuteButton].y()));
  ml_button_key[MlConfig::MuteButton]=
    parser->stringValue("MuteButton","Key",
			ml_button_key[MlConfig::MuteButton]);

  //
  // Conf Button Section
  //
  ml_button_show[MlConfig::ConfButton]=
    parser->boolValue("ConfButton","ShowButton",
		      ml_button_show[MlConfig::ConfButton]);
  ml_button_position[MlConfig::ConfButton]=
    QPoint(parser->intValue("ConfButton","ButtonX",
			    ml_button_position[MlConfig::ConfButton].x()),
	   parser->intValue("ConfButton","ButtonY",
			    ml_button_position[MlConfig::ConfButton].y()));
  ml_button_key[MlConfig::ConfButton]=
    parser->stringValue("ConfButton","Key",
			ml_button_key[MlConfig::ConfButton]);

  //
  // Vip Button Section
  //
  ml_button_show[MlConfig::VipButton]=
    parser->boolValue("VipButton","ShowButton",
		      ml_button_show[MlConfig::VipButton]);
  ml_button_position[MlConfig::VipButton]=
    QPoint(parser->intValue("VipButton","ButtonX",
			    ml_button_position[MlConfig::VipButton].x()),
	   parser->intValue("VipButton","ButtonY",
			    ml_button_position[MlConfig::VipButton].y()));
  ml_button_key[MlConfig::VipButton]=
    parser->stringValue("VipButton","Key",
			ml_button_key[MlConfig::VipButton]);

  //
  // Client Button Section
  //
  ml_button_show[MlConfig::ClientButton]=
    parser->boolValue("ClientButton","ShowButton",
		      ml_button_show[MlConfig::ClientButton]);
  ml_button_position[MlConfig::ClientButton]=
    QPoint(parser->intValue("ClientButton","ButtonX",
			    ml_button_position[MlConfig::ClientButton].x()),
	   parser->intValue("ClientButton","ButtonY",
			    ml_button_position[MlConfig::ClientButton].y()));
  ml_button_key[MlConfig::ClientButton]=
    parser->stringValue("ClientButton","Key",
			ml_button_key[MlConfig::ClientButton]);

  //
  // Legacy MainBank Section (deprecated)
  //
  ml_bank_show_hold_button[0]=
    parser->boolValue("MainBank","ShowHoldButton",ml_bank_show_hold_button[0]);
  ml_bank_hold_position[0]=
    QPoint(parser->intValue("MainBank","HoldButtonX",
			    ml_bank_hold_position[0].x()),
	   parser->intValue("MainBank","HoldButtonY",
			    ml_bank_hold_position[0].y()));
  ml_bank_hold_key[0]=
    parser->stringValue("MainBank","HoldKey",ml_bank_hold_key[0]);
  ml_bank_show_drop_button[0]=
    parser->boolValue("MainBank","ShowDropButton",ml_bank_show_drop_button[0]);
  ml_bank_drop_position[0]=
    QPoint(parser->intValue("MainBank","DropButtonX",
			    ml_bank_drop_position[0].x()),
	   parser->intValue("MainBank","DropButtonY",
			    ml_bank_drop_position[0].y()));
  ml_bank_drop_key[0]=
    parser->stringValue("MainBank","DropKey",ml_bank_drop_key[0]);

  //
  // Legacy ConfBank Section (deprecated)
  //
  ml_bank_show_hold_button[1]=
    parser->boolValue("ConfBank","ShowHoldButton",ml_bank_show_hold_button[1]);
  ml_bank_hold_position[1]=
    QPoint(parser->intValue("ConfBank","HoldButtonX",
			    ml_bank_hold_position[1].x()),
	   parser->intValue("ConfBank","HoldButtonY",
			    ml_bank_hold_position[1].y()));
  ml_bank_hold_key[1]=
    parser->stringValue("ConfBank","HoldKey",ml_bank_hold_key[1]);
  ml_bank_show_drop_button[1]=
    parser->boolValue("ConfBank","ShowDropButton",ml_bank_show_drop_button[1]);
  ml_bank_drop_position[1]=
    QPoint(parser->intValue("ConfBank","DropButtonX",
			    ml_bank_drop_position[1].x()),
	   parser->intValue("ConfBank","DropButtonY",
			    ml_bank_drop_position[1].y()));
  ml_bank_drop_key[1]=
    parser->stringValue("ConfBank","DropKey",ml_bank_drop_key[1]);

  //
  // Bank<n> Sections
  //
  for(int i=0;i<MAX_BANKS;i++) {
    section=QString().sprintf("Bank%d",i+1);
    ml_bank_show_hold_button[i]=
      parser->boolValue(section,"ShowHoldButton",ml_bank_show_hold_button[i]);
    ml_bank_hold_position[i]=
      QPoint(parser->intValue(section,"HoldButtonX",
			      ml_bank_hold_position[i].x()),
	     parser->intValue(section,"HoldButtonY",
			      ml_bank_hold_position[i].y()));
    ml_bank_hold_key[i]=
      parser->stringValue(section,"HoldKey",ml_bank_hold_key[i]);
    ml_bank_show_drop_button[i]=
      parser->boolValue(section,"ShowDropButton",ml_bank_show_drop_button[i]);
    ml_bank_drop_position[i]=
      QPoint(parser->intValue(section,"DropButtonX",
			      ml_bank_drop_position[i].x()),
	     parser->intValue(section,"DropButtonY",
			      ml_bank_drop_position[i].y()));
    ml_bank_drop_key[i]=
      parser->stringValue(section,"DropKey",ml_bank_drop_key[i]);
  }

  //
  // CallerBox Section
  //
  ml_caller_size=
    QSize(parser->intValue("CallerBox","SizeX",ml_caller_size.width()),
	  parser->intValue("CallerBox","SizeY",ml_caller_size.height()));
  ml_caller_show_frame=
    parser->boolValue("CallerBox","ShowFrame",ml_caller_show_frame);
  ml_caller_enable_warning_messages=
    parser->boolValue("CallerBox","EnableWarningMessages",
		      ml_caller_enable_warning_messages);
  ml_caller_allow_scrolling=
    parser->boolValue("CallerBox","AllowScrolling",ml_caller_allow_scrolling);
  LoadCallInfoField(parser,MlConfig::CallerName,"Name");
  LoadCallInfoField(parser,MlConfig::CallerAge,"Age");
  LoadCallInfoField(parser,MlConfig::CallerCityState,"CityState");
  LoadCallInfoField(parser,MlConfig::CallerZip,"Zip");
  LoadCallInfoField(parser,MlConfig::CallerPhone,"Phone");
  LoadCallInfoField(parser,MlConfig::CallerStation,"Station");
  LoadCallInfoField(parser,MlConfig::CallerCallCounter,"CallCounter");
  LoadCallInfoField(parser,MlConfig::CallerGender,"Gender");
  LoadCallInfoField(parser,MlConfig::CallerComment,"Comment");
  LoadCallInfoField(parser,MlConfig::CallerCell,"Cell");
  LoadCallInfoField(parser,MlConfig::CallerTimer,"Timer");
  LoadCallInfoField(parser,MlConfig::CallerQuality,"Quality");

  //
  // Line Section
  //
  for(int i=0;i<MAX_LINES;i++) {
    section=QString().sprintf("Line%d",i+1);
    //
    // Legacy Key Names (deprecated)
    //
    ml_line_show[0][i]=
      parser->boolValue(section,"ShowMainButton",ml_line_show[0][i]);
    ml_line_show[1][i]=
      parser->boolValue(section,"ShowConfButton",ml_line_show[1][i]);
    ml_line_show_caller_box[i]=
      parser->boolValue(section,"ShowCallerBox",ml_line_show_caller_box[i]);
    ml_line_position[0][i]=QPoint(parser->intValue(section,"MainButtonX",
				      ml_line_position[0][i].x()),
			parser->intValue(section,"MainButtonY",
				      ml_line_position[0][i].y()));
    ml_line_key[0][i]=parser->stringValue(section,"MainKey",ml_line_key[0][i]);
    ml_line_position[1][i]=QPoint(parser->intValue(section,"ConfButtonX",
				      ml_line_position[1][i].x()),
			parser->intValue(section,"ConfButtonY",
				      ml_line_position[1][i].y()));
    ml_line_key[1][i]=parser->stringValue(section,"ConfKey",ml_line_key[1][i]);

    ml_line_caller_box_window[i]=
      QRect(parser->intValue(section,"CallerBoxX",
			     ml_line_caller_box_window[i].x()),
	    parser->intValue(section,"CallerBoxY",
			     ml_line_caller_box_window[i].y()),
	    ml_caller_size.width(),ml_caller_size.height());
    ml_line_label[i]=parser->stringValue(section,"Label",ml_line_label[i]);
    ml_line_font_size[i]=
      parser->intValue(section,"FontSize",ml_line_font_size[i]);
    ml_line_caller_key[i]=
      parser->stringValue(section,"CallerKey",ml_line_caller_key[i]);

    //
    // Bank-Number Based Key Names
    //
    for(int j=0;j<MAX_BANKS;j++) {
      ml_line_show[j][i]=
	parser->boolValue(section,QString().sprintf("ShowBank%dButton",j+1),
			  ml_line_show[j][i]);
      ml_line_position[j][i]=
	QPoint(parser->intValue(section,QString().sprintf("Bank%dButtonX",j+1),
				ml_line_position[j][i].x()),
	       parser->intValue(section,QString().sprintf("Bank%dButtonY",j+1),
				ml_line_position[j][i].y()));
      ml_line_key[j][i]=
	parser->stringValue(section,QString().sprintf("Bank%dKey",j+1),
			    ml_line_key[j][i]);
    }
  }

  //
  // Segments Section
  //
  if(ml_clock_show_clock) {
    ml_seglist->setOffset(ml_clock_time_offset);
    ml_seglist->loadSegFile(ml_filename);
  }

  delete parser;
}


void MlConfig::clear()
{
  ml_connection_name="";
  ml_animate_icons=true;
  ml_ask_host=true;
  ml_window=QRect(0,0,1000,696);
  ml_keep_on_top=false;
  ml_allow_client_kill=false;
  ml_spawn_process="";
  ml_spawn_once=true;

  ml_disable_new_button=false;
  ml_disable_edit_button=false;
  ml_disable_delete_button=false;
  ml_disable_open_button=false;

  ml_disable_line_buttons=false;
  ml_disable_line_boxes=false;

  ml_skimmer_audio_root="";
  ml_skimmer_sample_rate=48000;
  ml_skimmer_channels=2;
  ml_skimmer_bitrate=128000;
  ml_skimmer_bitrate=0;
  ml_skimmer_format=MLWaveFile::Pcm16;
  ml_skimmer_card[0]=0;
  ml_skimmer_card[1]=0;
  ml_skimmer_port[0]=0;
  ml_skimmer_port[1]=1;
  ml_skimmer_filename[0]="main_bank_audio.wav";
  ml_skimmer_filename[1]="conf_bank_audio.wav";
  ml_skimmer_post_command[0]="";
  ml_skimmer_max_length[0]=0;
  ml_skimmer_max_length[1]=0;
  ml_skimmer_level[0]=400;
  ml_skimmer_level[1]=400;
  ml_skimmer_input_type[0]=MlConfig::Analog;
  ml_skimmer_input_type[1]=MlConfig::Analog;
  
  for(int i=0;i<MAX_BANKS;i++) {
    for(int j=0;j<MAX_LINES;j++) {
      ml_skimmer_enable[i][j]=false;
    }
  }
  ml_color[MlConfig::IdleColor]=QColor("lightGray");
  ml_palette[MlConfig::IdleColor]=
    QPalette(ml_color[MlConfig::IdleColor],QColor("black"));
  ml_palette[MlConfig::IdleColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::InUseColor]=QColor("yellow");
  ml_palette[MlConfig::InUseColor]=
    QPalette(ml_color[MlConfig::InUseColor],QColor("black"));
  ml_palette[MlConfig::InUseColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::OnAirColor]=QColor("red");
  ml_palette[MlConfig::OnAirColor]=
    QPalette(ml_color[MlConfig::OnAirColor],QColor("black"));
  ml_palette[MlConfig::OnAirColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::OffHookColor]=QColor("yellow");
  ml_palette[MlConfig::OffHookColor]=
    QPalette(ml_color[MlConfig::OffHookColor],QColor("black"));
  ml_palette[MlConfig::OffHookColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::OnHoldColor]=QColor("green");
  ml_palette[MlConfig::OnHoldColor]=
    QPalette(ml_color[MlConfig::OnHoldColor],QColor("black"));
  ml_palette[MlConfig::OnHoldColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::ScreenedHoldColor]=QColor("green");
  ml_palette[MlConfig::ScreenedHoldColor]=
    QPalette(ml_color[MlConfig::ScreenedHoldColor],QColor("black"));
  ml_palette[MlConfig::ScreenedHoldColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::ReadyNextColor]=QColor("green");
  ml_palette[MlConfig::ReadyNextColor]=
    QPalette(ml_color[MlConfig::ReadyNextColor],QColor("black"));
  ml_palette[MlConfig::ReadyNextColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::RingingColor]=QColor("blue");
  ml_palette[MlConfig::RingingColor]=
    QPalette(ml_color[MlConfig::RingingColor],QColor("black"));
  ml_palette[MlConfig::RingingColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::BusyColor]=QColor("yellow");
  ml_palette[MlConfig::BusyColor]=
    QPalette(ml_color[MlConfig::BusyColor],QColor("black"));
  ml_palette[MlConfig::BusyColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::CallerBoxColor]=QColor("white");
  ml_palette[MlConfig::CallerBoxColor]=ml_color[MlConfig::CallerBoxColor];
  ml_color[MlConfig::MessageBoxColor]=QColor("white");
  ml_palette[MlConfig::MessageBoxColor]=ml_color[MlConfig::MessageBoxColor];
  ml_color[MlConfig::DumpButtonColor]=QColor("red");
  ml_palette[MlConfig::DumpButtonColor]=
    QPalette(ml_color[MlConfig::DumpButtonColor],QColor("black"));
  ml_palette[MlConfig::DumpButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::RecordButtonColor]=QColor("gray");
  ml_palette[MlConfig::RecordButtonColor]=
    QPalette(ml_color[MlConfig::RecordButtonColor],QColor("black"));
  ml_palette[MlConfig::RecordButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::NextButtonColor]=QColor("gray");
  ml_palette[MlConfig::NextButtonColor]=
    QPalette(ml_color[MlConfig::NextButtonColor],QColor("black"));
  ml_palette[MlConfig::NextButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::KeypadButtonColor]=QColor("gray");
  ml_palette[MlConfig::KeypadButtonColor]=
    QPalette(ml_color[MlConfig::KeypadButtonColor],QColor("black"));
  ml_palette[MlConfig::KeypadButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::ScreenButtonColor]=QColor("red");
  ml_palette[MlConfig::ScreenButtonColor]=
    QPalette(ml_color[MlConfig::ScreenButtonColor],QColor("black"));
  ml_palette[MlConfig::ScreenButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::MuteButtonColor]=QColor("red");
  ml_palette[MlConfig::MuteButtonColor]=
    QPalette(ml_color[MlConfig::MuteButtonColor],QColor("black"));
  ml_palette[MlConfig::MuteButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::ConfButtonColor]=QColor("red");
  ml_palette[MlConfig::ConfButtonColor]=
    QPalette(ml_color[MlConfig::ConfButtonColor],QColor("black"));
  ml_palette[MlConfig::ConfButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_color[MlConfig::VipButtonColor]=QColor("red");
  ml_palette[MlConfig::VipButtonColor]=
    QPalette(ml_color[MlConfig::VipButtonColor],QColor("black"));
  ml_palette[MlConfig::VipButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));
  ml_palette[MlConfig::ClientButtonColor].
    setColor(QColorGroup::ButtonText,QColor("black"));  
  ml_msgbox_show=true;
  ml_msgbox_window=QRect(175,525,300,165);
  ml_msgbox_nickname="";
  ml_msgbox_send_lines=2;
  ml_msgbox_scrollback_lines=100;
  ml_msgbox_hangtime=5000;
  ml_msgbox_preserve_text=false;
  ml_msgbox_font=QFont("hevetica",16,QFont::Normal);
  ml_msgbox_font.setPixelSize(16);
  
  ml_clock_orientation=Qt::Vertical;
  ml_clock_show_clock=true;
  ml_clock_time_offset=0;
  ml_clock_send_time_offset=false;
  ml_clock_position=QPoint(815,525);
  ml_clock_show_hour=true;
	   ml_clock_military_format=true;

  delete ml_seglist;
  ml_seglist=new MLSegList();

  ml_button_show[MlConfig::BusyButton]=true;
  ml_button_position[MlConfig::BusyButton]=QPoint(5,525);
  ml_button_key[MlConfig::BusyButton]="Alt+B";
  ml_button_show_delay[MlConfig::BusyButton]=false;
  ml_button_delay_threshold[MlConfig::BusyButton]=0;
  ml_button_show[MlConfig::DumpButton]=true;
  ml_button_position[MlConfig::DumpButton]=QPoint(5,610);
  ml_button_key[MlConfig::DumpButton]="Alt+D";
  ml_button_show_delay[MlConfig::DumpButton]=true;
  ml_button_delay_threshold[MlConfig::DumpButton]=0;
  ml_button_show[MlConfig::RecordButton]=true;
  ml_button_position[MlConfig::RecordButton]=QPoint(90,525);
  ml_button_key[MlConfig::RecordButton]="Alt+R";
  ml_button_show_delay[MlConfig::RecordButton]=false;
  ml_button_delay_threshold[MlConfig::RecordButton]=0;
  ml_button_show[MlConfig::NextButton]=true;
  ml_button_position[MlConfig::NextButton]=QPoint(713,525);
  ml_button_key[MlConfig::NextButton]="Alt+N";
  ml_button_show_delay[MlConfig::NextButton]=false;
  ml_button_delay_threshold[MlConfig::NextButton]=0;
  ml_button_show[MlConfig::KeypadButton]=true;
  ml_button_position[MlConfig::KeypadButton]=QPoint(90,610);
  ml_button_key[MlConfig::KeypadButton]="Alt+K";
  ml_button_show_delay[MlConfig::FlashButton]=false;
  ml_button_delay_threshold[MlConfig::FlashButton]=0;
  ml_button_show[MlConfig::FlashButton]=false;
  ml_button_position[MlConfig::FlashButton]=QPoint(0,0);
  ml_button_key[MlConfig::FlashButton]="Alt+F";
  ml_button_show_delay[MlConfig::ScreenButton]=false;
  ml_button_delay_threshold[MlConfig::ScreenButton]=0;
  ml_button_show[MlConfig::ScreenButton]=false;
  ml_button_position[MlConfig::ScreenButton]=QPoint(0,0);
  ml_button_key[MlConfig::ScreenButton]="Alt+S";
  ml_button_show_delay[MlConfig::MuteButton]=false;
  ml_button_delay_threshold[MlConfig::ScreenButton]=0;
  ml_button_show[MlConfig::MuteButton]=false;
  ml_button_position[MlConfig::MuteButton]=QPoint(0,0);
  ml_button_key[MlConfig::MuteButton]="Alt+M";
  ml_button_show_delay[MlConfig::ConfButton]=false;
  ml_button_delay_threshold[MlConfig::ConfButton]=0;
  ml_button_show[MlConfig::ConfButton]=false;
  ml_button_position[MlConfig::ConfButton]=QPoint(0,0);
  ml_button_key[MlConfig::ConfButton]="Alt+C";
  ml_button_show_delay[MlConfig::VipButton]=false;
  ml_button_delay_threshold[MlConfig::VipButton]=0;
  ml_button_show[MlConfig::VipButton]=false;
  ml_button_position[MlConfig::VipButton]=QPoint(0,0);
  ml_button_key[MlConfig::VipButton]="Alt+V";
  ml_button_show_delay[MlConfig::ClientButton]=false;
  ml_button_delay_threshold[MlConfig::ClientButton]=0;
  ml_button_show[MlConfig::ClientButton]=true;
  ml_button_position[MlConfig::ClientButton]=QPoint(713,610);
  ml_button_key[MlConfig::ClientButton]="Alt+W";

  ml_bank_show_hold_button[0]=true;
  ml_bank_show_hold_button[1]=true;
  ml_bank_show_hold_button[2]=false;
  ml_bank_show_hold_button[3]=false;
  ml_bank_hold_position[0]=QPoint(526,525);
  ml_bank_hold_position[1]=QPoint(611,525);
  ml_bank_hold_key[0]="Home";
  ml_bank_hold_key[1]="Shift+Home";
  ml_bank_show_drop_button[0]=true;
  ml_bank_show_drop_button[1]=true;
  ml_bank_show_drop_button[2]=false;
  ml_bank_show_drop_button[3]=false;
  ml_bank_drop_position[0]=QPoint(526,610);
  ml_bank_drop_position[1]=QPoint(611,610);
  ml_bank_drop_key[0]="End";
  ml_bank_drop_key[1]="Shift+End";

  ml_caller_size=QSize(300,80);
  ml_caller_show_frame=false;
  ml_caller_allow_scrolling=false;
  ml_caller_enable_warning_messages=true;

  ml_caller_show[MlConfig::CallerName]=true;
  ml_caller_window[MlConfig::CallerName]=QRect(5,-2,190,19);
  ml_caller_font[MlConfig::CallerName]=QFont("helvetica",18,QFont::Bold);
  ml_caller_font[MlConfig::CallerName].setPixelSize(18);
  ml_caller_lines[MlConfig::CallerName]=1;

  ml_caller_show[MlConfig::CallerAge]=false;
  ml_caller_window[MlConfig::CallerAge]=QRect(0,0,0,0);
  ml_caller_font[MlConfig::CallerAge]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerAge].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerAge]=1;

  ml_caller_show[MlConfig::CallerCityState]=true;
  ml_caller_window[MlConfig::CallerCityState]=QRect(5,24,190,19);
  ml_caller_font[MlConfig::CallerCityState]=
    QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerCityState].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerCityState]=1;

  ml_caller_show[MlConfig::CallerZip]=false;
  ml_caller_window[MlConfig::CallerZip]=QRect(0,0,0,0);
  ml_caller_font[MlConfig::CallerZip]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerZip].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerZip]=1;

  ml_caller_show[MlConfig::CallerPhone]=false;
  ml_caller_window[MlConfig::CallerPhone]=QRect(0,0,0,0);
  ml_caller_font[MlConfig::CallerPhone]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerPhone].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerPhone]=1;

  ml_caller_show[MlConfig::CallerStation]=true;
  ml_caller_window[MlConfig::CallerStation]=QRect(200,24,60,19);
  ml_caller_font[MlConfig::CallerStation]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerStation].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerStation]=1;

  ml_caller_show[MlConfig::CallerGender]=true;
  ml_caller_window[MlConfig::CallerGender]=QRect(265,24,20,19);
  ml_caller_font[MlConfig::CallerGender]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerGender].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerGender]=1;

  ml_caller_show[MlConfig::CallerComment]=true;
  ml_caller_window[MlConfig::CallerComment]=QRect(5,40,290,33);
  ml_caller_font[MlConfig::CallerComment]=QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerComment].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerComment]=2;

  ml_caller_show[MlConfig::CallerCallCounter]=false;
  ml_caller_window[MlConfig::CallerCallCounter]=QRect(0,0,0,0);
  ml_caller_font[MlConfig::CallerCallCounter]=
    QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerCallCounter].setPixelSize(12);
  ml_caller_lines[MlConfig::CallerCallCounter]=1;

  ml_caller_show[MlConfig::CallerCell]=true;
  ml_caller_window[MlConfig::CallerCell]=QRect(220,1,20,20);
  ml_caller_font[MlConfig::CallerCell]=
    QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerCell].setPixelSize(12);

  ml_caller_show[MlConfig::CallerQuality]=true;
  ml_caller_window[MlConfig::CallerQuality]=QRect(196,1,20,20);
  ml_caller_font[MlConfig::CallerQuality]=
    QFont("helvetica",12,QFont::Normal);
  ml_caller_font[MlConfig::CallerQuality].setPixelSize(12);

  ml_caller_show[MlConfig::CallerTimer]=true;
  ml_caller_window[MlConfig::CallerTimer]=QRect(250,0,44,20);
  ml_caller_font[MlConfig::CallerTimer]=QFont("helvetica",14,QFont::Normal);
  ml_caller_font[MlConfig::CallerTimer].setPixelSize(14);
  ml_caller_lines[MlConfig::CallerTimer]=1;

  for(int j=0;j<MAX_LINES;j++) {
    ml_line_show[0][j]=true;
    ml_line_show[1][j]=true;
    ml_line_show[2][j]=false;
    ml_line_show[3][j]=false;
  }
  //
  // FIXME: We need to define default positions for banks 2 and 3!
  //
  ml_line_position[0][0]=QPoint(5,5);
  ml_line_position[1][0]=QPoint(90,5);

  ml_line_position[0][1]=QPoint(5,90);
  ml_line_position[1][1]=QPoint(90,90);

  ml_line_position[0][2]=QPoint(5,175);
  ml_line_position[1][2]=QPoint(90,175);

  ml_line_position[0][3]=QPoint(5,260);
  ml_line_position[1][3]=QPoint(90,260);

  ml_line_position[0][4]=QPoint(5,345);
  ml_line_position[1][4]=QPoint(90,345);

  ml_line_position[0][5]=QPoint(5,430);
  ml_line_position[1][5]=QPoint(90,430);

  ml_line_position[0][6]=QPoint(526,5);
  ml_line_position[1][6]=QPoint(611,5);

  ml_line_position[0][7]=QPoint(526,90);
  ml_line_position[1][7]=QPoint(611,90);

  ml_line_position[0][8]=QPoint(526,175);
  ml_line_position[1][8]=QPoint(611,175);

  ml_line_position[0][9]=QPoint(526,260);
  ml_line_position[1][9]=QPoint(611,260);

  ml_line_position[0][10]=QPoint(526,345);
  ml_line_position[1][10]=QPoint(611,345);

  ml_line_position[0][11]=QPoint(526,430);
  ml_line_position[1][11]=QPoint(611,430);

  ml_line_key[0][0]="F1";
  ml_line_key[0][1]="F2";
  ml_line_key[0][2]="F3";
  ml_line_key[0][3]="F4";
  ml_line_key[0][4]="F5";
  ml_line_key[0][5]="F6";
  ml_line_key[0][6]="F7";
  ml_line_key[0][7]="F8";
  ml_line_key[0][8]="F9";
  ml_line_key[0][9]="F10";
  ml_line_key[0][10]="F11";
  ml_line_key[0][11]="F12";
  ml_line_key[1][0]="Shift+F1";
  ml_line_key[1][1]="Shift+F2";
  ml_line_key[1][2]="Shift+F3";
  ml_line_key[1][3]="Shift+F4";
  ml_line_key[1][4]="Shift+F5";
  ml_line_key[1][5]="Shift+F6";
  ml_line_key[1][6]="Shift+F7";
  ml_line_key[1][7]="Shift+F8";
  ml_line_key[1][8]="Shift+F9";
  ml_line_key[1][9]="Shift+F10";
  ml_line_key[1][10]="Shift+F11";
  ml_line_key[1][11]="Shift+F12";
  ml_line_label[0]="1";
  ml_line_label[1]="2";
  ml_line_label[2]="3";
  ml_line_label[3]="4";
  ml_line_label[4]="5";
  ml_line_label[5]="6";
  ml_line_label[6]="7";
  ml_line_label[7]="8";
  ml_line_label[8]="9";
  ml_line_label[9]="10";
  ml_line_label[10]="11";
  ml_line_label[11]="12";
  ml_line_font_size[0]=36;
  ml_line_font_size[1]=36;
  ml_line_font_size[2]=36;
  ml_line_font_size[3]=36;
  ml_line_font_size[4]=36;
  ml_line_font_size[5]=36;
  ml_line_font_size[6]=36;
  ml_line_font_size[7]=36;
  ml_line_font_size[8]=36;
  ml_line_font_size[9]=36;
  ml_line_font_size[10]=36;
  ml_line_font_size[11]=36;
  ml_line_show_caller_box[0]=true;
  ml_line_show_caller_box[1]=true;
  ml_line_show_caller_box[2]=true;
  ml_line_show_caller_box[3]=true;
  ml_line_show_caller_box[4]=true;
  ml_line_show_caller_box[5]=true;
  ml_line_show_caller_box[6]=true;
  ml_line_show_caller_box[7]=true;
  ml_line_show_caller_box[8]=true;
  ml_line_show_caller_box[9]=true;
  ml_line_show_caller_box[10]=true;
  ml_line_show_caller_box[11]=true;
  ml_line_caller_box_window[0]=QRect(175,5,300,80);
  ml_line_caller_box_window[1]=QRect(175,90,300,80);
  ml_line_caller_box_window[2]=QRect(175,175,300,80);
  ml_line_caller_box_window[3]=QRect(175,260,300,80);
  ml_line_caller_box_window[4]=QRect(175,345,300,80);
  ml_line_caller_box_window[5]=QRect(175,430,300,80);
  ml_line_caller_box_window[6]=QRect(696,5,300,80);
  ml_line_caller_box_window[7]=QRect(696,90,300,80);
  ml_line_caller_box_window[8]=QRect(696,175,300,80);
  ml_line_caller_box_window[9]=QRect(696,260,300,80);
  ml_line_caller_box_window[10]=QRect(696,345,300,80);
  ml_line_caller_box_window[11]=QRect(696,430,300,80);
  ml_line_caller_key[0]="Ctrl+F1";
  ml_line_caller_key[1]="Ctrl+F2";
  ml_line_caller_key[2]="Ctrl+F3";
  ml_line_caller_key[3]="Ctrl+F4";
  ml_line_caller_key[4]="Ctrl+F5";
  ml_line_caller_key[5]="Ctrl+F6";
  ml_line_caller_key[6]="Ctrl+F7";
  ml_line_caller_key[7]="Ctrl+F8";
  ml_line_caller_key[8]="Ctrl+F9";
  ml_line_caller_key[9]="Ctrl+F10";
  ml_line_caller_key[10]="Ctrl+F11";
  ml_line_caller_key[11]="Ctrl+F12";

  ml_disable_watchdog=false;
  ml_disable_status_sync=false;
  ml_disable_caller_sync=false;
  ml_disable_message_sync=false;
  ml_disable_auto_detect=false;
}


void MlConfig::LoadCallInfoField(MLProfile *p,CallerFields field,
				 const QString &tag)
{
  QString face;
  QString weight;
  int size;
#ifdef OS_X
  int y_offset=4;
#else
  int y_offset=0;
#endif

  ml_caller_lines[field]=p->intValue("CallerBox",tag+"Lines",
				     ml_caller_lines[field]);
  QFontMetrics *metrics=NULL;
  ml_caller_show[field]=
    p->boolValue("CallerBox",QString("Show")+tag,ml_caller_show[field]);
  face=p->stringValue("CallerBox",tag+"FontFace",
			   ml_caller_font[field].family());
  size=p->
    intValue("CallerBox",tag+"FontSize",ml_caller_font[field].pixelSize());
  weight=p->stringValue("CallerBox",tag+"FontWeight",
			     SetFontWeight(ml_caller_font[field].weight()));
  ml_caller_font[field]=QFont(face,size,MLGetFontWeight(weight));
  ml_caller_font[field].setPixelSize(size);
  metrics=new QFontMetrics(ml_caller_font[field]);
  switch(field) {
  case MlConfig::CallerCell:
  case MlConfig::CallerQuality:
    ml_caller_window[field]=
      QRect(p->intValue("CallerBox",tag+"X",ml_caller_window[field].x()),
	    p->intValue("CallerBox",tag+"Y",ml_caller_window[field].y()),
	    25,25);
    break;

  default:
    ml_caller_window[field]=
      QRect(p->intValue("CallerBox",tag+"X",ml_caller_window[field].x()),
	    p->intValue("CallerBox",tag+"Y",ml_caller_window[field].y())+
	    y_offset,
	  p->intValue("CallerBox",tag+"SizeX",ml_caller_window[field].width()),
	    p->intValue("CallerBox",tag+"SizeY",metrics->height()*
			ml_caller_lines[field]));
    break;
  }
  delete metrics;
  metrics=NULL;
}


QString MlConfig::SetFontWeight(int weight)
{
  switch((QFont::Weight)weight) {
      case QFont::Light:
	return "Light";

      case QFont::Normal:
	return "Normal";

      case QFont::DemiBold:
	return "DemiBold";

      case QFont::Bold:
	return "Bold";

      case QFont::Black:
	return "Black";
  }
  return "Unknown";
}
