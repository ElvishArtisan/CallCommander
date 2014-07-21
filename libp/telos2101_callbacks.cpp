// telos2101_callbacks.cpp
//
// Callback Functions for the Telos 2101 Bus Driver
//
//   (C) Copyright 2002-2006 Fred Gleason <fredg@paravelsystems.com>
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

#include "telos2101_driver.h"

//#define TELOS2101_DRIVER_SHOW_CALLBACKS

static void Ph_EnumerateShowsCallback(void *userData,const char *show_name,
				      int passwd_reqd,int is_active,
				      const char *host_name,int last_name)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_EnumerateShowsCallback(%s,%d,%d,%s,%d)\n",
  	 show_name,passwd_reqd,is_active,host_name,last_name);
#endif
  ((Telos2101Driver *)userData)->showListData(show_name,host_name,is_active,
					      passwd_reqd,last_name);
}

static void Ph_EnumerateShowsForHostCallback(void *userData,
					     const char *showName,int isLast)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("EnumerateShowsForHostCallback(%s, %d)\n",showName,isLast);
#endif
}

static void Ph_EnumerateHostsCallback(void *userData,const char *hostName,
				      int isLast)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("EnumerateHostsCallback(%s, %d)\n",hostName,isLast);
#endif
}

static void Ph_EnumerateDirectorsCallback(void *userData,int session,
					  const char *director_name,
					  int talent_mode,int is_free,
					  int is_last)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_EnumerateDirectorsCallback(%d,%s,%d,%d,%d)\n",
    session,director_name,talent_mode,is_free,is_last);
#endif
/*
  if(QString(director_name).left(2)=="AP") {  // Ignore Asst Producer Sessions
    return;
  }
*/
  ((Telos2101Driver *)userData)->
    consoleListData(director_name,(ConsoleData::Mode)talent_mode,
		    is_free,is_last);
}

static void Ph_SendShowInfo(void *userData,int session,int max_lines,
			    int max_hybrids)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendShowInfo(%d, %d, %d)\n",session,max_lines,max_hybrids);
#endif
  ((Telos2101Driver *)userData)->showInfoData(max_lines,max_hybrids);
}

static void Ph_SendMode(void *userData,int session,int talent)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendMode(%d,%d)\n",session,talent);
#endif
  ((Telos2101Driver *)userData)->modeData(talent);
}

static void Ph_SendShowChange(void *userData,int session,
			      const char *newShowName,const char *oldShowName)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendShowChange(%d, %s, %s)\n",session,newShowName,oldShowName);
#endif
}

static void Ph_SendRecord(void *userData,int session,int on)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendRecord(%d,%d)\n",session,on);
#endif
  ((Telos2101Driver *)userData)->recordData(on);
}

static void Ph_SendBusyAll(void *userData,int session,int on)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendBusyAll(%d,%d)\n",session,on);
#endif
  ((Telos2101Driver *)userData)->busyAllData(on);
}

static void Ph_SendTransfer(void *userData,int session,int on)
{
  //  FIXME:  Implement this!
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendTransfer(%d,%d)\n",session,on);
#endif
}

static void Ph_SendMute(void *userData,int session,int on)
{
  //  FIXME:  Implement this!
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendMute(%d, %d)\n",session,on);
#endif
}

static void Ph_SendRingMute(void *userData,int session,int on)
{
  //  FIXME:  Implement this!
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendRingMute(%d, %d)\n",session,on);
#endif
}

static void Ph_SendDelayDump(void *userData,int session,int on)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendDelayDump(%d,%d)\n",session,on);
#endif
  ((Telos2101Driver *)userData)->delayDumpData(on);
}

static void Ph_SendHandset(void *userData,int session,int on)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendHandset(%d,%d)\n",session,on);
#endif
}

static void Ph_SendSpeaker(void *userData,int session,int on)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("SendSpeaker(%d, %d)\n",session,on);
#endif
}

static void Ph_SendLineState(void *userData,int session,int line,int bank,
			     int lstate,long elapsed_time)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendLineState(%d,%d,%d,0x%04X,%ld)\n",
         session,line,bank,lstate,elapsed_time);
#endif
  ((Telos2101Driver *)userData)->lineStateData(line,bank,lstate);
}

static void Ph_SendSeizeLine(void *userData,int session,int line,int column)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendSeizeLine(%d,%d,%d)\n",session,line,column);
#endif
}

static void Ph_SendRingLine(void *userData,int session,const char *pnum,
			    int line)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendRingLine(%d,%s,%d)\n",session,pnum,line);
#endif
  ((Telos2101Driver *)userData)->callerIdData(line,pnum);
}

static void Ph_SendDropLine(void *userData,int session,const char *callerId,
			    int line)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendDropLine(%d,%s,%d)\n",session,callerId,line);
#endif
}

static void Ph_SendAnswerLine(void *userData,int session,const char *callerId,
			      int line,int column)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendAnswerLine(%d,%s,%d,%d)\n",session,callerId,line,column);
#endif
}

static void Ph_SendSelectLine(void *userData,int session,const char *callerId,
			      int line,int column)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendSelectLine(%d,%s,%d,%d)\n",session,callerId,line,column);
#endif
}

static void Ph_SendDialKey(void *userData,int session,int key)
{
  //  FIXME:  Implement this!
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendDialKey(%d,%d)\n",session,key);
#endif
}

static void Ph_SendText(void *userData,int session,int text_type,
			const char *text,int line)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendText(%d,%d,%s,%d)\n",session,text_type,text,line);
#endif
  ((Telos2101Driver *)userData)->textData(text_type,line,text);
}

static void Ph_SendError(void *userData,int session,long err_num,
			 const char *err_text)
{
#ifdef TELOS2101_DRIVER_SHOW_CALLBACKS
  printf("Ph_SendError(%d,%ld,%s)\n",session,err_num,err_text);
#endif
  ((Telos2101Driver *)userData)->errorData(err_num,err_text);
}

TlsAP_Callbacks ph_callbacks=
{
	Ph_EnumerateShowsCallback,
	Ph_EnumerateHostsCallback,
	Ph_EnumerateDirectorsCallback,
	Ph_SendShowInfo,
	Ph_SendMode,
	Ph_SendShowChange,
	Ph_SendRecord,
	Ph_SendBusyAll,
	Ph_SendTransfer,
	Ph_SendMute,
	Ph_SendRingMute,
	Ph_SendDelayDump,
	Ph_SendHandset,
	Ph_SendSpeaker,
	Ph_SendLineState,
	Ph_SendSeizeLine,
	Ph_SendRingLine,
	Ph_SendDropLine,
	Ph_SendAnswerLine,
	Ph_SendSelectLine,
	Ph_SendDialKey,
	Ph_SendText,
	Ph_SendError,
	Ph_EnumerateShowsForHostCallback,
};
