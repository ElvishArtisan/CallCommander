/* mlm_airtools.c
 *
 * CallCommander Integration for an AirTools 6100 Broadcast Audio Delay
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
 *   gcc -shared -o mlm_airtools.mlm mlm_airtools.c
 */

#define FLASH_TIME_ON 600
#define FLASH_TIME_OFF 400

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mlm/mlm.h>

/*
 * Globals
 */
int delay_offset;
int delay_handle;
int delay_msg_istate;

char delay_tty_device[1024];
int delay_tty_speed;
int delay_unit_address;


void MLMStartup(const char *args)
{
  /*
   * Load the configuration
   */
  delay_offset=0;
  delay_msg_istate=0;
  strncpy(delay_tty_device,MLMGetStringValue(args,"Serial","Device",""),1024);
  delay_tty_speed=MLMGetIntegerValue(args,"Serial","Speed",38400);
  delay_unit_address=MLMGetIntegerValue(args,"Serial","UnitAddress",1);

  /*
   * Open the serial connection
   */
  if((delay_handle=MLMOpenSerial(delay_tty_device,delay_tty_speed,0,8))>=0) {
    MLMStartTimer(0,1000,MLM_TIMER_REPEATING);
  }
  else {
    syslog(LOG_ERR,"mlm_airtools: unable to open device \"%s\", exiting",
	   delay_tty_device);
    exit(256);
  }
}


void MLMDumpStateChanged(int state)
{
  char buffer[10];

  if(state) {
    sprintf(buffer,"%c%c%c%c%c%c%c",0xFB,0xFF&delay_unit_address,
	    0x00,0x03,0x90,0x08,0x65);
    MLMSendSerial(delay_handle,buffer,7);
  }
  else {
    sprintf(buffer,"%c%c%c%c%c%c%c",0xFB,0xFF&delay_unit_address,
	    0x00,0x03,0x90,0x00,0x6D);
    MLMSendSerial(delay_handle,buffer,7);
  }
}


void MLMMetadataUpdateRequested()
{
  MLMSendDelayOffset(100*delay_offset);
}


void MLMTimerExpired(int timernum)
{
  char buffer[10];

  /*
   * Query Delay Length
   */
  sprintf(buffer,"%c%c%c%c%c%c",0xFB,0xFF&delay_unit_address,
	  0x00,0x02,0x11,0xEB);
  MLMSendSerial(delay_handle,buffer,6);
}


void MLMSerialReceived(int handle,const char *data,int len)
{
  int i;

  for(i=0;i<len;i++) {
    switch(delay_msg_istate) {
	case 0:  /* Unit Address */
	  if(data[i]==delay_unit_address) {
	    delay_msg_istate=1;
	  }
	  break;

	case 1:  /* Device Type */
	  delay_msg_istate=2;
	  break;

	case 2:  /* Mfgr's Code */
	  delay_msg_istate=3;
	  break;

	case 3:  /* Message Length MSB */
	  if(data[i]==0x00) {
	    delay_msg_istate=4;
	  }
	  else {
	    delay_msg_istate=0;
	  }
	  break;

	case 4:  /* Message Length LSB */
	  if(data[i]==0x0C) {
	    delay_msg_istate=5;
	  }
	  else {
	    delay_msg_istate=0;
	  }
	  break;

	case 5:  /* Make up percentage (0.5% steps, 0 - 200) */
	  delay_msg_istate=6;
	  break;

	case 6:  /* Make up time (0.1 sec steps, 0 - 200) */
	  if((0xFF&data[i])!=delay_offset) {
	    delay_offset=0xFF&data[i];
	    MLMSendDelayOffset(100*delay_offset);
	  }
	  delay_msg_istate=0;
	  break;
    }
  }
}
