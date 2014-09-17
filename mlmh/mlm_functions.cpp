/* mlm_functions.cpp
 *
 * API Functions for the  CallCommander Logic Module Host
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
 */

#include <string.h>
#include <syslog.h>

#include <QtCore/QDateTime>

#include <mlprofile.h>

#include <ml_reload_packet.h>

#include <globals.h>
#include <mlmh.h>
#include <mlm.h>

/*
 * State Functions
 */
int MLMWatchdogState()
{
  return global_watchdog_state;
}


int MLMShow(char *showname,int len)
{
  strncpy(showname,global_show,len);
  return global_show_state;
}


int MLMConsole()
{
  return global_console;
}


int MLMConsoleMode()
{
  return global_console_mode;
}


int MLMLineState(int line)
{
  if(line<MAX_LINES) {
    return global_lstate[line];
  }
  return MLM_LINE_INACTIVE;
}


int MLMLineBank(int line)
{
  if(line<MAX_LINES) {
    return global_bank[line];
  }
  return -1;
}


int MLMLineConsole(int line)
{
  if(line<MAX_LINES) {
    return global_line_console[line];
  }
  return -1;
}


int MLMDumpState()
{
  return global_dump_state;
}


int MLMRecordState()
{
  return global_record_state;
}


int MLMBusyAllState()
{
  return global_busy_all_state;
}


int MLMScreenState()
{
  return global_screen_state;
}


int MLMMuteState()
{
  return global_mute_state;
}


int MLMConferenceState()
{
  return global_conference_state;
}


int MLMVipState()
{
  return global_vip_state;
}


int MLMDelayOffset()
{
  return global_delay_offset;
}


/*
 * Command Functions
 */
void MLMSelectLine(int line,int console,int bank)
{
  global_bus_driver->selectLine(line,console,bank);
}


void MLMHold(int bank)
{
  global_bus_driver->hold(bank);
}


void MLMDrop(int bank)
{
  global_bus_driver->drop(bank);
}


void MLMDump()
{
  global_bus_driver->dumpDelay(true);
}


void MLMToggleBusyAll()
{
  global_bus_driver->busyAll();
}


void MLMToggleRecord()
{
  global_bus_driver->toggleRecord();
}


void MLMTakeNext(int bank)
{
  global_bus_driver->takeNext(bank);
}


void MLMSendDtmf(char key)
{
  global_bus_driver->sendDtmf(key);
}


void MLMFlash()
{
  global_bus_driver->flash();
}


void MLMScreen(int state)
{
  global_bus_driver->screen(state);
}


void MLMMute(int state)
{
  global_bus_driver->mute(state);
}


void MLMConference(int state)
{
  global_bus_driver->conference(state);
}


void MLMVip(int state)
{
  global_bus_driver->vip(state);
}


void MLMSendDelayOffset(int msecs)
{
  global_bus_driver->sendString(BusDriver::DelayOffsetGlobalElement,0,
				QString().sprintf("%d",msecs));
}


void MLMSendString(int elem,int line,const char *str)
{
  if(elem<MLM_FIRST_ELEMENT) {
    return;
  }
  global_bus_driver->sendString((BusDriver::Element)elem,line,str);
}


void MLMSendChatMessage(const char *name,const char *msg)
{
  global_bus_driver->sendString(BusDriver::MessageGlobalElement,0,
				QString().sprintf("%s|%s",name,msg));
}


void MLMSendLineWarning(int line,const char *msg)
{
  global_bus_driver->sendString(BusDriver::LineWarningGlobalElement,line,msg);
}


void MLMSendUdp(const char *hostname,uint16_t port,const char *data,int len)
{
  QHostAddress addr=MlResolveIpAddress(hostname);
  if(!addr.isNull()) {
    global_send_socket->writeBlock(data,len,addr,port);
  }
}


int MLMOpenUdp(uint16_t port)
{
  global_receive_sockets.push_back(new Q3SocketDevice(Q3SocketDevice::Datagram));
  global_receive_sockets.back()->bind(QHostAddress(),port);
  global_receive_sockets.back()->setBlocking(false);
  if(!global_receive_timer->isActive()) {
    global_receive_timer->start(MLMH_RECEIVE_POLL_INTERVAL);
  }
  return global_receive_sockets.back()->socket();
}


void MLMCloseUdp(int handle)
{
  for(unsigned i=0;i<global_receive_sockets.size();i++) {
    if(handle==global_receive_sockets[i]->socket()) {
      global_receive_sockets[i]->close();
      delete global_receive_sockets[i];
      vector<Q3SocketDevice *>::iterator it=global_receive_sockets.begin()+i;
      global_receive_sockets.erase(it,it+1);
      return;
    }
  }
}


int MLMOpenSerial(const char *port,int speed,int parity,int word_length)
{
  global_tty_devices.push_back(new MLTTYDevice(global_host));
  global_tty_devices.back()->setName(port);
  global_tty_devices.back()->setSpeed(speed);
  global_tty_devices.back()->setParity((MLTTYDevice::Parity)parity);
  global_tty_devices.back()->setWordLength(word_length);
  if(global_tty_devices.back()->open(QIODevice::ReadWrite)) {
    if(!global_receive_timer->isActive()) {
      global_receive_timer->start(MLMH_RECEIVE_POLL_INTERVAL);
    }
    return (int)global_tty_devices.size()-1;
  }
  return -1;
}


void MLMSendSerial(int handle,const char *data,int len)
{
  if((handle<0)||(handle>=(int)global_tty_devices.size())) {
    return;
  }
  global_tty_devices[handle]->writeBlock(data,len);
}


void MLMCloseSerial(int handle)
{
  //
  // FIXME: We really ought to take out the trash here!
  //
  global_tty_devices[handle]->close();
  delete global_tty_devices[handle];
  global_tty_devices[handle]=NULL;
}


const char *MLMDateTime(int offset_msecs,const char *format)
{
  static char str[1024];

  QDateTime datetime=QDateTime(QDate::currentDate(),QTime::currentTime().
			       addMSecs(offset_msecs));
  strncpy(str,datetime.toString(format),1024);
  return str;
}


int MLMVGuestOpen(const char *hostname,uint16_t port,
		  const char *user,const char *password)
{
  //
  // Resolve the Hostname
  //
  QHostAddress addr=MlResolveIpAddress(hostname);
  if(addr.isNull()) {
    return -1;
  }

  //
  // Generate a Unique Handle
  //
  int id=0;
  for(unsigned i=0;i<global_vguests.size();i++) {
    if(id<=global_vguests[i]->id()) {
      id=global_vguests[i]->id()+1;
    }
  }

  //
  // Attempt to Open the Connection
  //
  VGuest *vguest=new VGuest(id);
  if(!vguest->connect(addr,port,user,password)) {
    delete vguest;
    return -1;
  }
  global_vguests.push_back(vguest);
  if(!global_receive_timer->isActive()) {
    global_receive_timer->start(MLMH_RECEIVE_POLL_INTERVAL);
  }

  return vguest->id();
}


void MLMVGuestClose(int handle)
{
  for(unsigned i=0;i<global_vguests.size();i++) {
    if(handle==global_vguests[i]->id()) {
      global_vguests[i]->close();
      delete global_vguests[i];
      vector<VGuest *>::iterator it=global_vguests.begin()+i;
      global_vguests.erase(it,it+1);
    }
  }
}


void MLMVGuestPulse(int handle,int engine,int device,int surface,int relay,
		    int length)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendPulse(engine,device,surface,relay,length);  
}


void MLMVGuestTurnOn(int handle,int engine,int device,int surface,int bus)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendTurnOn(engine,device,surface,bus);
}


void MLMVGuestTurnOff(int handle,int engine,int device,int surface,int bus)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendTurnOff(engine,device,surface,bus);
}


void MLMVGuestSetInputAssign(int handle,int dest_engine,int dest_device,
			     int surface,int src_engine,int src_device)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendSetInputAssign(dest_engine,dest_device,surface,
					     src_engine,src_device);
}


void MLMVGuestSetInputMode(int handle,int engine,int device,int surface,
			   int mode)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendSetInputMode(engine,device,surface,mode);
}


void MLMVGuestSetFaderLevel(int handle,int engine,int device,int surface,
			    int level)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->sendSetFaderLevel(engine,device,surface,level);
}


void MLMVGuestWriteText(int handle,int engine,int device,int surface,int dline,
			int dcol,int attr,const char *msg)
{
  int offset=GetVGuestIndex(handle);
  if(offset<0) {
    return;
  }
  global_vguests[offset]->
    sendWriteText(engine,device,surface,dline,dcol,attr,msg);
}


int MLMGetIntegerValue(const char *filename,const char *section,
		       const char *label,int default_value)
{
  MLProfile *p=new MLProfile();
  p->setSource(filename);
  int r=p->intValue(section,label,default_value);
  delete p;
  return r;
}


int MLMGetHexValue(const char *filename,const char *section,
		   const char *label,int default_value)
{
  MLProfile *p=new MLProfile();
  p->setSource(filename);
  int r=p->hexValue(section,label,default_value);
  delete p;
  return r;
}


int MLMGetBooleanValue(const char *filename,const char *section,
		       const char *label,int default_value)
{
  MLProfile *p=new MLProfile();
  p->setSource(filename);
  bool r=p->boolValue(section,label,default_value);
  delete p;
  return (int)r;
}


const char *MLMGetStringValue(const char *filename,const char *section,
			      const char *label,const char *default_value)
{
  static char r[1024];

  MLProfile *p=new MLProfile();
  p->setSource(filename);
  strncpy(r,p->stringValue(section,label,default_value),1024);
  delete p;
  return r;
}


void MLMStartTimer(int timernum,int msecs,int mode)
{
  if((timernum<0)||(timernum>=MLM_MAX_TIMERS)) {
    return;
  }
  if(global_callback_timers[timernum]->isActive()) {
    global_callback_timers[timernum]->stop();
  }
  global_callback_timers[timernum]->start(msecs,mode);
}


void MLMStopTimer(int timernum)
{
  if((timernum<0)||(timernum>=MLM_MAX_TIMERS)) {
    return;
  }
  if(global_callback_timers[timernum]->isActive()) {
    global_callback_timers[timernum]->stop();
  }
}
