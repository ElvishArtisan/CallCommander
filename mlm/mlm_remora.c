/* mlm_remora.c
 *
 * CallCommander Integration for a Logitek Remora control surface.
 *
 *   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as
 *   published by the Free Software Foundation; either version 2 of
 *   the License, or (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * This module displays call screening data on the LCD displays of
 * a Logitek Remora control surface.
 *
 * To compile this module, just do:
 * 
 *   gcc -shared -o mlm_remora.mlm mlm_remora.c
 */

#include <stdio.h>
#include <string.h>
#include <mlm/mlm.h>

#define REMORA_ONAIR_FLAG ""
#define ONAIR_FLASH_ON_TIME 600
#define ONAIR_FLASH_OFF_TIME 400

/*
 * Globals
 */
int vguest_handle;
char caller_name[12][256];
char caller_city[12][256];
char caller_state[12][256];
char caller_station[12][256];
char caller_comment[12][1024];
char caller_textline[12][2][2048];
int caller_lstate[12];
int line_bank[12];
int delay_offset;
char predelay_clock_string[20];
char postdelay_clock_string[20];
int delay_warning_on;
char vguest_hostname[1024];
uint16_t vguest_tcp_port;
char vguest_profile_name[256];
char vguest_password[256];
int vguest_engine;
int vguest_first_fader_device;
int vguest_hybrid_device[4];
int vguest_clock_device;
int vguest_clock_line;
int vguest_clock_column;
int delay_enable_clock;
int delay_low_limit;


void WriteLineEntry(int line,int lstate)
{
  char label[1024];
  char output[1024];

  /*
   * First Text Line -- Name, City, State, Station
   */
  if(strlen(caller_name[line])>0) {
    sprintf(label,"%s ",caller_name[line]);
  }
  else {
    switch(caller_lstate[line]) {
	case MLM_LINE_IDLE:
	case MLM_LINE_INACTIVE:
	  label[0]=0;
	  break;

	default:
	  sprintf(label,"Name Unknown ");
	  break;
    }
  }
  if((strlen(caller_city[line])>0)||(strlen(caller_state[line])>0)) {
    sprintf(label+strlen(label),"- %s %s ",caller_city[line],caller_state[line]);
  }
  if(strlen(caller_station[line])>0) {
    sprintf(label+strlen(label),"- %s",caller_station[line]);
  }
  if(((lstate==MLM_LINE_ONAIR)||(lstate==MLM_LINE_LOCKED))&&
     (strlen(label)>0)&&(strlen(REMORA_ONAIR_FLAG)>0)) {
    sprintf(output,"%s %s %s",REMORA_ONAIR_FLAG,label,REMORA_ONAIR_FLAG);
  }
  else {
    strcpy(output,label);
  }
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_first_fader_device+line,0,0,0,0,output);
  strncpy(caller_textline[line][0],output,2048);

  /*
   * Second Text Line -- Comment
   */
  if(((lstate==MLM_LINE_ONAIR)||(lstate==MLM_LINE_LOCKED))&&
     (strlen(caller_comment[line])>0)&&(strlen(REMORA_ONAIR_FLAG)>0)) {
    sprintf(output,"%s %s %s",REMORA_ONAIR_FLAG,caller_comment[line],
	    REMORA_ONAIR_FLAG);
  }
  else {
    strcpy(output,caller_comment[line]);
  }
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_first_fader_device+line,0,1,0,0,output);
  strncpy(caller_textline[line][1],output,2048);
}


void ClearLineEntry(int line)
{
  int j;

  caller_name[line][0]=0;
  caller_city[line][0]=0;
  caller_state[line][0]=0;
  caller_station[line][0]=0;
  caller_comment[line][0]=0;
  for(j=0;j<2;j++) {
    caller_textline[line][j][0]=0;
  }
}


void MLMStartup(const char *args)
{
  int i;
  int j;

  /*
   * Initialize Globals
   */
  delay_offset=0;
  predelay_clock_string[0]=0;
  postdelay_clock_string[0]=0;
  delay_warning_on=0;
  for(i=0;i<12;i++) {
    caller_name[i][0]=0;
    caller_city[i][0]=0;
    caller_state[i][0]=0;
    caller_station[i][0]=0;
    caller_comment[i][0]=0;
    caller_lstate[i]=MLM_LINE_INACTIVE;
    line_bank[i]=-1;
    for(j=0;j<2;j++) {
      caller_textline[i][j][0]=0;
    }
  }

  /*
   * Load Configuration Values
   */
  strncpy(vguest_hostname,MLMGetStringValue(args,"vGuest","Hostname",""),1024);
  vguest_tcp_port=MLMGetIntegerValue(args,"vGuest","TCPPort",10212);
  strncpy(vguest_profile_name,
	  MLMGetStringValue(args,"vGuest","ProfileName",""),256);
  strncpy(vguest_password,
	  MLMGetStringValue(args,"vGuest","ProfilePassword",""),256);
  vguest_engine=MLMGetIntegerValue(args,"vGuest","AudioEngine",0);
  vguest_first_fader_device=MLMGetHexValue(args,"vGuest","FirstFaderDevice",0);
  vguest_hybrid_device[0]=MLMGetHexValue(args,"vGuest","Hybrid1Device",0);
  vguest_hybrid_device[1]=MLMGetHexValue(args,"vGuest","Hybrid2Device",0);
  vguest_hybrid_device[2]=MLMGetHexValue(args,"vGuest","Hybrid3Device",0);
  vguest_hybrid_device[3]=MLMGetHexValue(args,"vGuest","Hybrid4Device",0);
  vguest_clock_device=MLMGetHexValue(args,"vGuest","ClockDevice",0);
  vguest_clock_line=MLMGetIntegerValue(args,"vGuest","ClockLine",48);
  vguest_clock_column=MLMGetIntegerValue(args,"vGuest","ClockColumn",0);
  delay_enable_clock=MLMGetBooleanValue(args,"Delay","EnableClock",0);
  delay_low_limit=MLMGetIntegerValue(args,"Delay","LowLimit",3000);

  /*
   * Initialize vGuest Connection
   */
  vguest_handle=MLMVGuestOpen(vguest_hostname,vguest_tcp_port,
			      vguest_profile_name,vguest_password);

  /*
   * Write Clock Headers
   */
  if(delay_enable_clock==1) {
    MLMStartTimer(0,100,MLM_TIMER_REPEATING);         /* Clock Poll Timer */
    MLMStartTimer(1,1000,MLM_TIMER_REPEATING);  /* Delay Warning Flash Timer */
    MLMVGuestWriteText(vguest_handle,vguest_engine,
		       vguest_clock_device,0,
		       vguest_clock_line,
		       vguest_clock_column,
		       MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
		       MLM_VGUEST_TEXT_VIDEO_NORMAL|
		       MLM_VGUEST_TEXT_COLOR_WHITE,
		       "---- CURRENT TIME ----");
    MLMVGuestWriteText(vguest_handle,vguest_engine,
		       vguest_clock_device,0,
		       vguest_clock_line+1,
		       vguest_clock_column,
		       MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
		       MLM_VGUEST_TEXT_VIDEO_NORMAL|
		       MLM_VGUEST_TEXT_COLOR_WHITE,
		       "Pre-Delay   Post-Delay");
  }

  /*
   * Start Onair Flash Timers
   */
  if(ONAIR_FLASH_ON_TIME>0) {
    MLMStartTimer(2,ONAIR_FLASH_ON_TIME,MLM_TIMER_ONESHOT);
  }
}


void MLMShutdown()
{
  int i;

  /*
   * Clear Line Info Area
   */
  for(i=0;i<4;i++) {
    MLMVGuestWriteText(vguest_handle,vguest_engine,
		       vguest_first_fader_device,0,i+48,0,
		       MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
		       MLM_VGUEST_TEXT_VIDEO_NORMAL|
		       MLM_VGUEST_TEXT_COLOR_WHITE," ");
    MLMVGuestWriteText(vguest_handle,vguest_engine,
		       vguest_first_fader_device+7,0,i+48,0,
		       MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
		       MLM_VGUEST_TEXT_VIDEO_NORMAL|
		       MLM_VGUEST_TEXT_COLOR_WHITE," ");
  }

  /*
   * Clear Fader Labels
   */
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_hybrid_device[0],0,2,0,0," ");
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_hybrid_device[1],0,2,0,0," ");
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_hybrid_device[2],0,2,0,0," ");
  MLMVGuestWriteText(vguest_handle,vguest_engine,
		     vguest_hybrid_device[3],0,2,0,0," ");

  /*
   * Clear Clocks
   */
  if(delay_enable_clock==1) {
    for(i=0;i<4;i++) {
      MLMVGuestWriteText(vguest_handle,vguest_engine,
			 vguest_clock_device,0,
			 vguest_clock_line+i,
			 vguest_clock_column,
			 MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
			 MLM_VGUEST_TEXT_VIDEO_NORMAL|
			 MLM_VGUEST_TEXT_COLOR_WHITE," ");
    }
  }
}


void MLMLineStateChanged(int lstate,int line,int bank,int console)
{
  char label[8];
  int i;
  int active=0;

  sprintf(label,"Line %2d",line+1);
  switch(lstate) {
      case MLM_LINE_ONAIR:
      case MLM_LINE_LOCKED:
	/*
	 * Set Fader Label
	 */
	MLMVGuestWriteText(vguest_handle,vguest_engine,
			   vguest_hybrid_device[bank],0,2,0,0,label);
	line_bank[line]=bank;
	break;

      default:
	/*
	 * Clear Fader Label
	 */
	for(i=0;i<12;i++) {
	  if(((MLMLineState(i)==MLM_LINE_ONAIR)||
	      (MLMLineState(i)==MLM_LINE_LOCKED))&&
	     (MLMLineBank(line)==MLMLineBank(i))) {
	    active=1;
	  }
	}
	if(active==0) {
	  MLMVGuestWriteText(vguest_handle,vguest_engine,
		       vguest_hybrid_device[line_bank[line]],0,2,0,0," ");
	}
	line_bank[line]=-1;
	break;
  }

  switch(lstate) {
      case MLM_LINE_INACTIVE:
      case MLM_LINE_IDLE:
	/*
	 * Clear Line Label
	 */
	ClearLineEntry(line);
	MLMVGuestWriteText(vguest_handle,vguest_engine,
			   vguest_first_fader_device+line,0,0,0,0," ");
	MLMVGuestWriteText(vguest_handle,vguest_engine,
			   vguest_first_fader_device+line,0,1,0,0," ");
	break;

      default:
	WriteLineEntry(line,lstate);
	break;
  }
  caller_lstate[line]=lstate;
}


void MLMDelayOffsetChanged(int offset)
{
  delay_offset=offset;
}


void MLMCallerElementReceived(int line,int elem,const char *str)
{
  switch(elem) {
      case MLM_ELEMENT_NAME:
	strncpy(caller_name[line],str,256);
	break;

      case MLM_ELEMENT_CITY:
	strncpy(caller_city[line],str,256);
	break;

      case MLM_ELEMENT_STATE:
	strncpy(caller_state[line],str,256);
	break;

      case MLM_ELEMENT_STATION:
	strncpy(caller_station[line],str,256);
	break;

      case MLM_ELEMENT_COMMENT:
	strncpy(caller_comment[line],str,1024);
	/*
	 * FIXME
	 * 
	 * This is brittle -- we should really have an 'update finished' 
	 * message to send this on.  (We can't write the entry on every 
	 * element update -- it crashes Supervisor!)
	 */
	WriteLineEntry(line,caller_lstate[line]);
	break;

      default:
	return;
  }
}


void MLMTimerExpired(int timernum)
{
  int i;
  int j;
  char buffer[256];

  switch(timernum) {
    case 0:  /* Poll Clock */
      if((strcmp(MLMDateTime(delay_offset,"hh:mm:ss"),
		 predelay_clock_string)!=0)||
	 (strcmp(MLMDateTime(0,"hh:mm:ss"),
		 postdelay_clock_string))!=0) {
	strcpy(predelay_clock_string,MLMDateTime(delay_offset,"hh:mm:ss"));
	strcpy(postdelay_clock_string,MLMDateTime(0,"hh:mm:ss"));
	sprintf(buffer,"%s     %s",predelay_clock_string,
		postdelay_clock_string);
	MLMVGuestWriteText(vguest_handle,vguest_engine,
			   vguest_clock_device,0,
			   vguest_clock_line+2,
			   vguest_clock_column,
			   MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
			   MLM_VGUEST_TEXT_VIDEO_NORMAL|
			   MLM_VGUEST_TEXT_COLOR_WHITE,
			   buffer);
      }
      break;

    case 1:  /* Delay Warning */
      switch(delay_warning_on) {
        case 0:
	  if(delay_offset<delay_low_limit) {
	    sprintf(buffer,"Delay below :%02d seconds!",
		    delay_low_limit/1000);
	    MLMVGuestWriteText(vguest_handle,vguest_engine,
			       vguest_clock_device,0,
			       vguest_clock_line+3,
			       vguest_clock_column,
			       MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
			       MLM_VGUEST_TEXT_VIDEO_NORMAL|
			       MLM_VGUEST_TEXT_COLOR_WHITE,
			       buffer);
	    delay_warning_on=1;
	  }
	  break;

        case 1:
	  MLMVGuestWriteText(vguest_handle,vguest_engine,
			     vguest_clock_device,0,
			     vguest_clock_line+3,
			     vguest_clock_column,
			     MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
			     MLM_VGUEST_TEXT_VIDEO_NORMAL|
			     MLM_VGUEST_TEXT_COLOR_WHITE,
			     " ");
	  delay_warning_on=0;
	  break;
      }
      break;

      case 2:  /* On Air Flash ON Timer */
	for(i=0;i<12;i++) {
	  if((caller_lstate[i]==MLM_LINE_ONAIR)||
	     (caller_lstate[i]==MLM_LINE_LOCKED)) {
	    MLMVGuestWriteText(vguest_handle,vguest_engine,
			       vguest_first_fader_device+i,0,0,0,0," ");
	    MLMVGuestWriteText(vguest_handle,vguest_engine,
			       vguest_first_fader_device+i,0,1,0,0," ");
	  }
	}
	MLMStartTimer(3,ONAIR_FLASH_OFF_TIME,MLM_TIMER_ONESHOT);
	break;

      case 3:  /* On Air Flash OFF Timer */
	for(i=0;i<12;i++) {
	  if((caller_lstate[i]==MLM_LINE_ONAIR)||
	     (caller_lstate[i]==MLM_LINE_LOCKED)) {
	    MLMVGuestWriteText(vguest_handle,vguest_engine,
			       vguest_first_fader_device+i,0,0,0,0,
			       caller_textline[i][0]);
	    MLMVGuestWriteText(vguest_handle,vguest_engine,
			       vguest_first_fader_device+i,0,1,0,0,
			       caller_textline[i][1]);
	  }
	}
	MLMStartTimer(2,ONAIR_FLASH_ON_TIME,MLM_TIMER_ONESHOT);
	break;
  }
}
