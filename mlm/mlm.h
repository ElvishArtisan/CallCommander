/* mlm.h
 *
 * The CallCommander Logic Module Interface
 *
 *   (C) Copyright 2007 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef MLM_H
#define MLM_H

#include <stdint.h>
#include <syslog.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * MLM API Version Number
 */
#define MLM_VERSION 0

/*
 * Line States (from BusDriver::LineState)
 * (used in the MLMLineStateChanged() callback)
 */
#define MLM_LINE_INACTIVE 0
#define MLM_LINE_IDLE 1
#define MLM_LINE_ELSEWHERE 2
#define MLM_LINE_HANDSET 3
#define MLM_LINE_SPEAKER 4
#define MLM_LINE_HOLD 5
#define MLM_LINE_SCREENED_HOLD 6
#define MLM_LINE_ONAIR 7
#define MLM_LINE_RINGING 8
#define MLM_LINE_LOCKED 9
#define MLM_LINE_BUSIED 10
#define MLM_LINE_NEXT 11

/*
 * Console Modes (from ConsoleData::Mode)
 * (used in the MLMConsoleChanged() callback)
 */
#define MLM_CONSOLE_PRODUCER 1
#define MLM_CONSOLE_TALENT 1

/*
 * CallCommander Internal Caller Element Values (from BusDriver::Element)
 * (used in the MLMCallerElementReceived() callback)
 */
#define MLM_ELEMENT_NAME 256
#define MLM_ELEMENT_ADDR1 257
#define MLM_ELEMENT_ADDR2 258
#define MLM_ELEMENT_CITY 259
#define MLM_ELEMENT_STATE 260
#define MLM_ELEMENT_ZIP 261
#define MLM_ELEMENT_PHONE 262
#define MLM_ELEMENT_EMAIL 263
#define MLM_ELEMENT_GENDER 264
#define MLM_ELEMENT_STATION 265
#define MLM_ELEMENT_COMMENT 266
#define MLM_ELEMENT_CELLPHONE 267
#define MLM_ELEMENT_QUALITY 268
#define MLM_ELEMENT_COUNTER 269
#define MLM_ELEMENT_AGE 270
#define MLM_ELEMENT_RING_TIMER 271
#define MLM_ELEMENT_AIR_TIMER 272
#define MLM_ELEMENT_HOLD_TIMER 273

/*
 * String Element Boundary
 *
 * This element value and higher are legal to use in MLMSendString()
 * (lower elements are reserved for CallCommander internal use)
 */
#define MLM_FIRST_ELEMENT 1001

/*
 * Available Timers
 */
#define MLM_MAX_TIMERS 32

/*
 * Timer Modes
 * (for use in the MLMStartTimer() function.
 */
#define MLM_TIMER_REPEATING 0
#define MLM_TIMER_ONESHOT 1

/*
 * Logitek vGuest Default TCP Port
 */
#define MLM_VGUEST_DEFAULT_TCP_PORT 10212

/*
 * Logitek vGuest Input Modes
 * (used in the MLMVGuestSetInputMode() command)
 */
#define MLM_VGUEST_MODE_STEREO 0
#define MLM_VGUEST_MODE_MONO 1
#define MLM_VGUEST_MODE_REVERSE 2
#define MLM_VGUEST_MODE_LEFT_BOTH 3
#define MLM_VGUEST_MODE_RIGHT_BOTH 4
#define MLM_VGUEST_MODE_LEFT_ONLY 5
#define MLM_VGUEST_MODE_RIGHT_ONLY 6

/*
 * Logitek vGuest Text Attributes
 * (used in the MLMVGuestWriteText() command)
 *
 * A text attribute is constructed by bitwise OR'ing together a DISPLAY,
 * VIDEO and COLOR attribute.  For example, an attribute that would
 * overwrite a display with flashing, green text would be expressed as
 *   attr = MLM_VGUEST_TEXT_DISPLAY_OVERWRITE |
 *          MLM_VGUEST_TEXT_VIDEO_FLASH |
 *          MLM_VGUEST_TEXT_COLOR_GREEN;
 */
#define MLM_VGUEST_TEXT_DISPLAY_OVERWRITE 0x00
#define MLM_VGUEST_TEXT_DISPLAY_INSERT 0x80

#define MLM_VGUEST_TEXT_VIDEO_NORMAL 0x00
#define MLM_VGUEST_TEXT_VIDEO_FLASH 0x10
#define MLM_VGUEST_TEXT_VIDEO_REVERSE 0x20

#define MLM_VGUEST_TEXT_COLOR_WHITE 0x00
#define MLM_VGUEST_TEXT_COLOR_RED 0x01
#define MLM_VGUEST_TEXT_COLOR_YELLOW 0x02
#define MLM_VGUEST_TEXT_COLOR_GREEN 0x03
#define MLM_VGUEST_TEXT_COLOR_CYAN 0x04
#define MLM_VGUEST_TEXT_COLOR_MAGENTA 0x05
  
/*
 * Callbacks
 */
void MLMStartup(const char *args);
void MLMShutdown();
void MLMMetadataUpdateRequested();
void MLMWatchdogStateChanged(int state);
void MLMShowChanged(const char *showname,int state);
void MLMConsoleChanged(int console,int mode);
void MLMLineStateChanged(int lstate,int line,int bank,int console);
void MLMDumpStateChanged(int state);
void MLMRecordStateChanged(int state);
void MLMBusyAllStateChanged(int state);
void MLMScreenStateChanged(int state);
void MLMMuteStateChanged(int state);
void MLMConferenceStateChanged(int state);
void MLMVipStateChanged(int state);
void MLMDelayOffsetChanged(int msecs);
void MLMReceivedString(int elem,int line,const char *str);
void MLMCallerIdReceived(int line,const char *number);
void MLMChatMessageReceived(const char *name,const char *msg);
void MLMCallerElementReceived(int line,int elem,const char *str);
void MLMUdpRecieved(int handle,const char *data,int len);
void MLMSerialReceived(int handle,const char *data,int len);
void MLMTimerExpired(int timernum);
void MLMVGuestPulseReceived(int handle,int engine,int device,int surface,
			    int relay,int length);
void MLMVGuestTurnOnReceived(int handle,int engine,int device,int surface,
			     int bus);
void MLMVGuestTurnOffReceived(int handle,int engine,int device,int surface,
			      int bus);
void MLMVGuestSetInputAssignReceived(int handle,int dest_engine,
				     int dest_device,int surface,
				     int src_engine,int src_device);
void MLMVGuestSetInputModeReceived(int handle,int engine,int device,
				   int surface,int mode);
void MLMVGuestSetFaderLevelReceived(int handle,int engine,int device,
				    int surface,int level);

/*
 * System State Functions
 */
int MLMWatchdogState();
int MLMShow(const char *showname,int len);
int MLMConsole();
int MLMConsoleMode();
int MLMLineState(int line);
int MLMLineBank(int line);
int MLMLineConsole(int line);
int MLMDumpState();
int MLMRecordState();
int MLMBusyAllState();
int MLMScreenState();
int MLMMuteState();
int MLMConferenceState();
int MLMVipState();
int MLMDelayOffset();

/*
 * Command Functions
 */
void MLMSelectLine(int line,int console,int bank);
void MLMHold(int bank);
void MLMDrop(int bank);
void MLMDump();
void MLMToggleBusyAll();
void MLMToggleRecord();
void MLMTakeNext(int bank);
void MLMSendDtmf(char key);
void MLMFlash();
void MLMScreen(int state);
void MLMMute(int state);
void MLMConference(int state);
void MLMVip(int state);
void MLMSendDelayOffset(int msecs);
void MLMSendString(int elem,int line,const char *str);
void MLMSendChatMessage(const char *name,const char *msg);
void MLMSendLineWarning(int line,const char *msg);

/*
 * Communications Functions
 */
void MLMSendUdp(const char *hostname,uint16_t port,const char *data,int len);
int MLMOpenUdp(uint16_t port);
void MLMCloseUdp(int handle);
int MLMOpenSerial(const char *port,int speed,int parity,int word_length);
void MLMSendSerial(int handle,const char *data,int len);
void MLMCloseSerial(int handle);

/*
 * Convienence Functions
 */
const char *MLMDateTime(int offset_msecs,const char *format);
void MLMStartTimer(int timernum,int msecs,int mode);
void MLMStopTimer(int timernum);
int MLMGetIntegerValue(const char *filename,const char *section,
		       const char *label,int default_value);
int MLMGetHexValue(const char *filename,const char *section,
		   const char *label,int default_value);
int MLMGetBooleanValue(const char *filename,const char *section,
		       const char *label,int default_value);
const char *MLMGetStringValue(const char *filename,const char *section,
			      const char *label,const char *default_value);

/*
 * Logitek vGuest Functions
 */
int MLMVGuestOpen(const char *hostname,uint16_t port,
		  const char *user,const char *password);
void MLMVGuestClose(int handle);
void MLMVGuestPulse(int handle,int engine,int device,int surface,int relay,
		    int length);
void MLMVGuestTurnOn(int handle,int engine,int device,int surface,int bus);
void MLMVGuestTurnOff(int handle,int engine,int device,int surface,int bus);
void MLMVGuestSetInputAssign(int handle,int dest_engine,int dest_device,
			     int surface,int src_engine,int src_device);
void MLMVGuestSetInputMode(int handle,int engine,int device,int surface,
			   int mode);
void MLMVGuestSetFaderLevel(int handle,int engine,int device,int surface,
			    int level);
void MLMVGuestWriteText(int handle,int engine,int device,int surface,
			int dline,int dcol,int attr,const char *msg);

#ifdef __cplusplus
}
#endif

#endif  /* MLM_H */
