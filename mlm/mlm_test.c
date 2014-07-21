/* mlm_test.c
 *
 * A sample CallCommander Logic Module
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
 * This is a sample CallCommander Logic Module.  All it does is print
 * a message to stdout for each callback.  (To see the messages, be sure 
 * to specify a Log Path in MDbAdmin->ManageLogicModules).
 *
 * To compile this module, just do:
 * 
 *   gcc -shared -o mlm_test.mlm mlm_test.c
 */

#include <stdio.h>
#include <mlm/mlm.h>

int vguest_handle;

void MLMStartup(const char *args)
{
  printf("MLMStartup(\"%s\")\n",args);
}


void MLMShutdown()
{
  printf("MLMShutdown()\n");
}


void MLMMetadataUpdateRequested()
{
  printf("MLMMetadataUpdateRequested()\n");
}


void MLMWatchdogStateChanged(int state)
{
  printf("MLMWatchdogStateChanged(%d)\n",state);
}


void MLMShowChanged(const char *showname,int state)
{
  printf("MLMShowChanged(\"%s\",%d)\n",showname,state);
}


void MLMConsoleChanged(int console,int mode)
{
  printf("MLMConsoleChanged(%d,%d)\n",console,mode);
}


void MLMLineStateChanged(int lstate,int line,int bank,int console)
{
  printf("MLMLineStateChanged(%d,%d,%d,%d)\n",lstate,line,bank,console);
}


void MLMDumpStateChanged(int state)
{
  char buffer[1024];
  int i,j;

  printf("MLMDumpStateChanged(%d)\n",state);

  for(i=0;i<12;i++ ) {
    for(j=0;j<2;j++) {
      sprintf(buffer,"Fader %d - Line %d",i+1,j);
      MLMVGuestWriteText(vguest_handle,3,0x000b+i,0,j,0,
			 MLM_VGUEST_TEXT_DISPLAY_OVERWRITE|
			 MLM_VGUEST_TEXT_VIDEO_FLASH|
			 MLM_VGUEST_TEXT_COLOR_WHITE,buffer);
    }
  }
}


void MLMRecordStateChanged(int state)
{
  printf("MLMRecordStateChanged(%d)\n",state);
}


void MLMBusyAllStateChanged(int state)
{
  printf("MLMBusyAllStateChanged(%d)\n",state);
}


void MLMScreenStateChanged(int state)
{
  printf("MLMScreenStateChanged(%d)\n",state);
}


void MLMMuteStateChanged(int state)
{
  printf("MLMMuteStateChanged(%d)\n",state);
}


void MLMConferenceStateChanged(int state)
{
  printf("MLMConferenceStateChanged(%d)\n",state);
}


void MLMVipStateChanged(int state)
{
  printf("MLMVipStateChanged(%d)\n",state);
}


void MLMDelayOffsetChanged(int msecs)
{
  printf("MLMDelayOffsetChanged(%d)\n",msecs);
}


void MLMReceivedString(int elem,int line,const char *str)
{
  printf("MLMReceivedString(%d,%d,\"%s\")\n",elem,line,str);
}


void MLMCallerIdReceived(int line,const char *number)
{
  printf("MLMCallerIdReceived(%d,\"%s\")\n",line,number);
}


void MLMChatMessageReceived(const char *name,const char *msg)
{
  printf("MLMChatMessageRecieved(\"%s\",\"%s\")\n",name,msg);
}


void MLMCallerElementReceived(int line,int elem,const char *str)
{
  printf("MLMCallerElementReceived(%d,%d,\"%s\")\n",line,elem,str);
}


void MLMUdpReceived(int handle,const char *data,int len)
{
  char buffer[1024];
  int i;
  for(i=0;i<len;i++) {
    buffer[i]=data[i];
  }
  buffer[len]=0;
  printf("MLMUdpReceived(%d,\"%s\",%d)\n",handle,buffer,len);
}


void MLMSerialReceived(int handle,const char *data,int len)
{
  char buffer[1024];
  int i;
  for(i=0;i<len;i++) {
    buffer[i]=data[i];
  }
  buffer[len]=0;
  printf("MLMSerialReceived(%d,\"%s\",%d)\n",handle,buffer,len);
}


void MLMTimerExpired(int timernum)
{
  printf("MLMTimerExpired(%d)\n",timernum);
}


void MLMVGuestPulseReceived(int handle,int engine,int device,int surface,
			    int relay,int length)
{
  printf("MLMVGuestPulseReceived(%d,%d,0x%04X,%d,%d,%d)\n",
	 handle,engine,device,surface,relay,length);
}


void MLMVGuestTurnOnReceived(int handle,int engine,int device,int surface,
			     int bus)
{
  printf("MLMVGuestTurnOnReceived(%d,%d,0x%04X,%d,%d)\n",
	 handle,engine,device,surface,bus);
}


void MLMVGuestTurnOffReceived(int handle,int engine,int device,int surface,
			      int bus)
{
  printf("MLMVGuestTurnOffReceived(%d,%d,0x%04X,%d,%d)\n",
	 handle,engine,device,surface,bus);
}


void MLMVGuestSetInputAssignReceived(int handle,int dest_engine,
				     int dest_device,int surface,
				     int src_engine,int src_device)
{
  printf("MLMVGuestSetInputAssignReceived(%d,%d,0x%04X,%d,%d,0x%04X)\n",
	 handle,dest_engine,dest_device,surface,src_engine,src_device);
}


void MLMVGuestSetInputModeReceived(int handle,int engine,int device,
				   int surface,int mode)
{
  printf("MLMVGuestSetInputModeReceived(%d,%d,0x%04X,%d,%d)\n",
	 handle,engine,device,surface,mode);
}


void MLMVGuestSetFaderLevelReceived(int handle,int engine,int device,
				    int surface,int level)
{
  printf("MLMVGuestSetFaderLevelReceived(%d,%d,0x%04X,%d,%d)\n",
	 handle,engine,device,surface,level);
}
