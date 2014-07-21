/* mlm_cunctator.c
 *
 * CallCommander Integration for the Cunctator Delay Control System
 *
 *   (C) Copyright 2011 Fred Gleason <fredg@paravelsystems.com>
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
 *   gcc -shared -o mlm_cunctator.mlm mlm_cunctator.c
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
char delay_ip_address[20];
int delay_remote_port;
int delay_local_port;
int delay_number;

int delay_offset;
int delay_udp_handle;

void MLMStartup(const char *args)
{
  char buffer[128];

  /*
   * Load the configuration
   */
  delay_offset=0;
  strncpy(delay_ip_address,MLMGetStringValue(args,"Delay","IpAddress",""),19);
  delay_remote_port=MLMGetIntegerValue(args,"Delay","RemoteUdpPort",3749);
  delay_local_port=MLMGetIntegerValue(args,"Delay","LocalUdpPort",3750);
  delay_number=MLMGetIntegerValue(args,"Delay","DelayNumber",0);

  /*
   * Open the UDP port
   */
  if((delay_udp_handle=MLMOpenUdp(delay_local_port))<0) {
    syslog(LOG_ERR,"mlm_cunctator: unable to open UDP port %d, exiting",
	   delay_local_port);
    exit(256);
  }
  snprintf(buffer,128,"GO %d O 4 1 1!",delay_number);
  MLMSendUdp(delay_ip_address,delay_remote_port,buffer,strlen(buffer));
}


void MLMDumpStateChanged(int state)
{
  char buffer[128];

  if(state) {
    snprintf(buffer,128,"GO %d O 3 1 1!",delay_number);
    MLMSendUdp(delay_ip_address,delay_remote_port,buffer,strlen(buffer));
  }
}


void MLMMetadataUpdateRequested()
{
  MLMSendDelayOffset(delay_offset);
}


void MLMUdpReceived(int handle,const char *data,int len)
{
  float delay;
  char str[1024];
  int i;

  for(i=0;i<len;i++) {
    str[i]=data[i];
  }
  if(sscanf(str,"%f",&delay)==1) {
    if((int)(delay*1000.0)!=delay_offset) {
      delay_offset=(int)(delay*1000.0);
      MLMSendDelayOffset(delay_offset);
    }
  }
}
