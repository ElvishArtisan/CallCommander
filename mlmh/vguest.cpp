// vguest.cpp
//
//
// Communications Layer for the Logitek vGuest Protocol
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
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
#include <syslog.h>
#include <signal.h>

#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <Qt3Support/q3socketdevice.h>

#include <globals.h>
#include <vguest.h>

VGuest::VGuest(int id)
{
  //
  // Initialize Data Structures
  //
  vguest_id=id;
  vguest_connecting=false;
  vguest_connected=false;
  vguest_istate=0;
  vguest_cmd_length=0;
  vguest_cmd_ptr=0;
  

  //
  // Random Number Generator
  //
  srandom(QTime::currentTime().msec());

  //
  // Initialize the connection
  //
  vguest_socket=new Q3SocketDevice();
  vguest_socket->setBlocking(false);
}


int VGuest::id() const
{
  return vguest_id;
}


bool VGuest::connect(const QHostAddress &addr,Q_UINT16 port,
		       const QString &user,const QString &password)
{
  vguest_address=addr;
  vguest_port=port;
  vguest_profile=user;
  vguest_password=password;

  global_hard_shutdown=true;
  if(global_shutdown_flag) {
    SigHandler(SIGTERM);
  }

  while(!vguest_connected) {
    //
    // Open the TCP Connection
    //
    vguest_socket->setBlocking(true);
    if(vguest_socket->connect(addr,port)) {
      vguest_socket->setBlocking(false);
  
      //
      // Try to Authenticate with Supervisor
      //
      vguest_connecting=true;
      while(vguest_connecting) {
	readyRead();
	usleep(10000);
      }
    }
    else {
      usleep(1000*GetHoldoff());
    }
  }
  syslog(LOG_DAEMON|LOG_NOTICE,"connection to vGuest device at %s established",
	 (const char *)addr.toString());
  global_hard_shutdown=false;
  return vguest_connected;
}


void VGuest::close()
{
  vguest_socket->close();
}


void VGuest::sendPulse(int engine,int device,int surface,int relay,int length)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x07\x51%c%c%c%c%c%c",
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&relay,
	  0xFF&length);
  SendCommand(buffer,9);
}


void VGuest::sendTurnOn(int engine,int device,int surface,int bus)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x06\x52%c%c%c%c%c",
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&bus);
  SendCommand(buffer,8);
}


void VGuest::sendTurnOff(int engine,int device,int surface,int bus)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x06\x53%c%c%c%c%c",
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&bus);
  SendCommand(buffer,8);
}


void VGuest::sendSetInputAssign(int dest_engine,int dest_device,int surface,
				int src_engine,int src_device)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x08\x54%c%c%c%c%c%c%c",
	  0xFF&dest_engine,
	  0xFF&(dest_device>>8),
	  0xFF&dest_device,
	  0xFF&surface,
	  0xFF&src_engine,
	  0xFF&(src_device>>8),
	  0xFF&src_device);
  SendCommand(buffer,10);
}


void VGuest::sendSetInputMode(int engine,int device,int surface,int mode)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x06\x55%c%c%c%c%c",
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&mode);
  SendCommand(buffer,8);
}


void VGuest::sendSetFaderLevel(int engine,int device,int surface,int level)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  sprintf(buffer,"\x02\x06\x56%c%c%c%c%c",
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&level);
  SendCommand(buffer,8);
}


void VGuest::sendWriteText(int engine,int device,int surface,int dline,
			   int dcol,int attr,const QString &msg)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  QString str=msg.left(VGUEST_MAX_TEXT_LENGTH);
  int len=str.length();

  sprintf(buffer,"\x02%c\x5C%c%c%c%c%c%c%c%s",
	  8+len,
	  0xFF&engine,
	  0xFF&(device>>8),
	  0xFF&device,
	  0xFF&surface,
	  0xFF&dline,
	  0xFF&dcol,
	  0xFF&attr,
	  (const char *)str);
  SendCommand(buffer,10+len);
}


void VGuest::readyRead()
{
  char buffer[255];
  int n=0;

  while((n=vguest_socket->readBlock(buffer,255))>0) {
    for(int i=0;i<n;i++) {
      switch(vguest_istate) {
	  case 0:   // STX Command Start
	    if(buffer[i]==0x02) {
	      vguest_istate=1;
	    }
	    break;

	  case 1:   // Command Length
	    vguest_cmd_length=buffer[i];
	    vguest_cmd_buffer[0]=2;
	    vguest_cmd_buffer[1]=buffer[i];
	    vguest_cmd_ptr=2;
	    vguest_istate=2;
	    break;

	  case 2:   // Command Body
	    vguest_cmd_buffer[vguest_cmd_ptr++]=buffer[i];
	    if(vguest_cmd_ptr==(vguest_cmd_length+2)) {
	      DispatchCommand(vguest_cmd_buffer,vguest_cmd_length+2);
	      vguest_istate=0;
	    }
	    break;
      }
    }
  }
  if(n==0) {  // Connection closed, attempt to reconnect
    syslog(LOG_DAEMON|LOG_ERR,
	   "connection to vGuest device at %s closed, attempting reconnect",
	   (const char *)vguest_address.toString());
    vguest_connected=false;
    delete vguest_socket;
    vguest_socket=new Q3SocketDevice();
    vguest_socket->setBlocking(false);
    connect(vguest_address,vguest_port,vguest_profile,vguest_password);
  }
}


void VGuest::SendCommand(char *str,int len)
{
  // printf("SENT: %s",(const char *)RenderCommand(str,len));
  vguest_socket->writeBlock(str,len);
}


void VGuest::DispatchCommand(char *cmd,int len)
{
  char buffer[VGUEST_MAX_COMMAND_LENGTH];
  QString str;

  // printf("RCVD: %s\n",(const char *)RenderCommand(cmd,len));

  switch(0xFF&cmd[2]) {
      case 0xF9:   // Username/Password Query
	buffer[0]=0x02;
	buffer[1]=0x22;
	buffer[2]=0xF9;
	buffer[3]=VGUEST_ID_BYTE;
	sprintf(buffer+4,"%s%s",
		(const char *)PadString(vguest_profile,16),
		(const char *)PadString(vguest_password,16));
	SendCommand(buffer,36);
	break;

      case 0xF0:   // Connect Status
	switch(0xFF&cmd[3]) {
	    case 0x0A:  // Valid connection
	    case 0x14:
	      vguest_connecting=false;
	      vguest_connected=true;
	      break;

	    default:
	      vguest_connecting=false;
	      vguest_connected=false;
	      vguest_socket->close();
	      break;
	}
	break;

      case 0x51:   // Pulse
	if((mlm_vguest_pulse_received_sym!=NULL)&&global_init_complete) {
	  (*mlm_vguest_pulse_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]),
	     (int)(0xFF&cmd[8]));
	  fflush(stdout);
	}
	break;

      case 0x52:   // Turn On
	if((mlm_vguest_turn_on_received_sym!=NULL)&&global_init_complete) {
	  (*mlm_vguest_turn_on_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]));
	  fflush(stdout);
	}
	break;

      case 0x53:   // Turn Off
	if((mlm_vguest_turn_off_received_sym!=NULL)&&global_init_complete) {
	  (*mlm_vguest_turn_off_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]));
	  fflush(stdout);
	}
	break;


      case 0x54:   // Input Assign
	if((mlm_vguest_set_input_assign_received_sym!=NULL)&&
	   global_init_complete) {
	  (*mlm_vguest_set_input_assign_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]),
	     (int)(256*(0xFF&cmd[8])+(0xFF&cmd[9])));
	  fflush(stdout);
	}
	break;

      case 0x55:   // Input Mode
	if((mlm_vguest_set_input_mode_received_sym!=NULL)&&
	   global_init_complete) {
	  (*mlm_vguest_set_input_mode_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]));
	  fflush(stdout);
	}
	break;

      case 0x56:   // Fader Level
	if((mlm_vguest_set_fader_level_received_sym!=NULL)&&
	   global_init_complete) {
	  (*mlm_vguest_set_fader_level_received_sym)
	    ((int)vguest_id,
	     (int)(0xFF&cmd[3]),
	     (int)(256*(0xFF&cmd[4])+(0xFF&cmd[5])),
	     (int)(0xFF&cmd[6]),
	     (int)(0xFF&cmd[7]));
	  fflush(stdout);
	}
	break;
  }
}


QString VGuest::PadString(QString str,unsigned len)
{
  QString out;
  out=str.left(len);
  while(out.length()<(int)len) {
    out+=" ";
  }
  return out;
}


QString VGuest::RenderCommand(char *cmd,int len)
{
  QString str;

  for(int i=0;i<len;i++) {
    str+=QString().sprintf("%02X",0xFF&cmd[i]);
  }
  return str;
}


int VGuest::GetHoldoff()
{
#ifdef WIN32
  return VGUEST_RECONNECT_MIN_INTERVAL;
#else
  return (int)(VGUEST_RECONNECT_MIN_INTERVAL+
	       (VGUEST_RECONNECT_MAX_INTERVAL-VGUEST_RECONNECT_MIN_INTERVAL)*
	       (double)random()/(double)RAND_MAX);
#endif  // WIN32
}
