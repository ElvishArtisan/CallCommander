/* mlm_udpdelay.c
 *
 * A UDP Packet Delay Plugin form CallCommander
 *
 *   (C) Copyright 2009 Fred Gleason <fredg@paravelsystems.com>
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
 * This is a sample CallCommander Logic Module.  All it does is print
 * a message to stdout for each callback.  (To see the messages, be sure 
 * to specify a Log Path in MDbAdmin->ManageLogicModules).
 *
 * To compile this module, just do:
 * 
 *   gcc -shared -o mlm_test.mlm mlm_udpdelay.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>

#include <mlm/mlm.h>

#define UDPDELAY_MAX_PACKET_SIZE 1500
#define UDPDELAY_POLL_INTERVAL 100
struct pkt_data {
  char pkt_data[UDPDELAY_MAX_PACKET_SIZE];
  int pkt_len;
  double pkt_timestamp;
};

int *udp_handles=NULL;
int *dest_destinations=NULL;
double *fixed_delays=NULL;
char **dest_hostnames=NULL;
uint16_t **dest_ports=NULL;
struct pkt_data **pkt_data=NULL;
unsigned *pkt_count=NULL;
//double delay_secs=0.0;
double *delay_secs=NULL;
int delay_quantity=0;

int udpdelay_GetDelayNumber(int handle)
{
  int i;

  for(i=0;i<delay_quantity;i++) {
    if(udp_handles[i]==handle) {
      return i;
    }
  }
  return -1;
}

double udpdelay_Timestamp()
{
  struct timeval tv;

  memset(&tv,0,sizeof(tv));
  gettimeofday(&tv,NULL);
  return ((double)tv.tv_sec)+((double)tv.tv_usec/1000000.0);
}


void udpdelay_PushPacket(int delaynum,const char *data,int len)
{
  int i;

  char str[1024];
  strncpy(str,data,len);
  printf("IN: %s\n",str);


  pkt_data[delaynum]=
    realloc(pkt_data[delaynum],++(pkt_count[delaynum])*sizeof(struct pkt_data));
  for(i=0;i<len;i++) {
    pkt_data[delaynum][pkt_count[delaynum]-1].pkt_data[i]=data[i];
  }
  pkt_data[delaynum][pkt_count[delaynum]-1].pkt_len=len;
  pkt_data[delaynum][pkt_count[delaynum]-1].pkt_timestamp=udpdelay_Timestamp();
}


void udpdelay_PopPacket(int delaynum)
{
  int i;

  pkt_count[delaynum]--;
  for(i=0;i<pkt_count[delaynum];i++) {
    pkt_data[delaynum][i]=pkt_data[delaynum][i+1];
  }
  pkt_data[delaynum]=
    realloc(pkt_data[delaynum],sizeof(struct pkt_data)*pkt_count[delaynum]);
}


void udpdelay_SendPacket(int delaynum,const char *data,int len)
{
  int i;

  for(i=0;i<dest_destinations[delaynum];i++) {
    MLMSendUdp(dest_hostnames[delaynum]+i*PATH_MAX,
	       dest_ports[delaynum][i],data,len);
  }
}


void MLMStartup(const char *args)
{
  char hostname[PATH_MAX];
  char section[255];
  char label[255];

  sprintf(section,"Delay%d",delay_quantity+1);
  udp_handles=realloc(udp_handles,(delay_quantity+1)*sizeof(int));
  while((udp_handles[delay_quantity]=
	 MLMOpenUdp(MLMGetIntegerValue(args,section,"InputPort",0)))>0) {
    dest_destinations=realloc(dest_destinations,(delay_quantity+1)*sizeof(int));
    dest_destinations[delay_quantity]=0;
    fixed_delays=realloc(fixed_delays,(delay_quantity+1)*sizeof(double));
    fixed_delays[delay_quantity]=
      (double)MLMGetIntegerValue(args,section,"FixedDelay",0)/1000.0;
    delay_secs=realloc(delay_secs,(delay_quantity+1)*sizeof(double));
    delay_secs[delay_quantity]=fixed_delays[delay_quantity];
    pkt_data=realloc(pkt_data,(delay_quantity+1)*sizeof(pkt_data));
    pkt_data[delay_quantity]=NULL;
    pkt_count=realloc(pkt_count,(delay_quantity+1)*sizeof(unsigned));
    pkt_count[delay_quantity]=0;
    sprintf(label,"DestinationHostname%d",dest_destinations[delay_quantity]+1);
    dest_hostnames=
      realloc(dest_hostnames,(delay_quantity+1)*sizeof(dest_hostnames));
    dest_hostnames[delay_quantity]=NULL;
    dest_ports=realloc(dest_ports,(delay_quantity+1)*sizeof(dest_ports));
    dest_ports[delay_quantity]=NULL;
    strncpy(hostname,MLMGetStringValue(args,section,label,""),PATH_MAX);
    if(strlen(hostname)==0) {
      syslog(LOG_WARNING,"mlm_udpdelay: no destination hostnames specified");
      return;
    }
    while(strlen(hostname)>0) {
      dest_hostnames[delay_quantity]=
	realloc(dest_hostnames[delay_quantity],
		(dest_destinations[delay_quantity]+1)*PATH_MAX);
      strcpy(dest_hostnames[delay_quantity]+
	     PATH_MAX*dest_destinations[delay_quantity],hostname);
      dest_ports[delay_quantity]=
	realloc(dest_ports[delay_quantity],
		(dest_destinations[delay_quantity]+1)*sizeof(uint16_t));
      sprintf(label,"DestinationPort%d",dest_destinations[delay_quantity]+1);
      dest_ports[delay_quantity][dest_destinations[delay_quantity]]=
	MLMGetIntegerValue(args,section,label,0);
      
      dest_destinations[delay_quantity]++;
      sprintf(label,"DestinationHostname%d",
	      dest_destinations[delay_quantity]+1);
      strncpy(hostname,MLMGetStringValue(args,section,label,""),PATH_MAX);
    }
    MLMStartTimer(delay_quantity,UDPDELAY_POLL_INTERVAL,MLM_TIMER_REPEATING);

    delay_quantity++;
    sprintf(section,"Delay%d",delay_quantity+1);
    udp_handles=realloc(udp_handles,(delay_quantity+1)*sizeof(int));
  }
}


void MLMShutdown()
{
  int i;
  for(i=0;i<delay_quantity;i++) {
    MLMStopTimer(i);
    MLMCloseUdp(udp_handles[i]);
    free(pkt_data[i]);
    free(dest_hostnames[i]);
    free(dest_ports[i]);
  }
  free(delay_secs);
  free(pkt_data);
  free(pkt_count);
  free(dest_hostnames);
  free(fixed_delays);
  free(dest_ports);
}


void MLMDelayOffsetChanged(int msecs)
{
  unsigned i;
  double delay=(double)msecs/1000.0;

  for(i=0;i<delay_quantity;i++) {
    delay_secs[i]=delay+fixed_delays[i];
  }
}


void MLMUdpReceived(int handle,const char *data,int len)
{
  int i;
  int delaynum;
  if((delaynum=udpdelay_GetDelayNumber(handle))<0) {
    syslog(LOG_ERR,"mlm_udpdelay: invalid internal delay number");
    return;
  }

  for(i=0;i<delay_quantity;i++) {
    if(udp_handles[i]==handle) {
      if(delay_secs[delaynum]<=0.0) {
	udpdelay_SendPacket(i,data,len);
      }
      else {
	udpdelay_PushPacket(i,data,len);
      }
    }
  }
}


void MLMTimerExpired(int delaynum)
{
  double current_time;

  current_time=udpdelay_Timestamp();
  while(pkt_count[delaynum]>0) {
    if((pkt_data[delaynum][0].pkt_timestamp+delay_secs[delaynum])>
       current_time) {
      return;
    }
    udpdelay_SendPacket(delaynum,pkt_data[delaynum][0].pkt_data,
			pkt_data[delaynum][0].pkt_len);
    udpdelay_PopPacket(delaynum);
  }
}
