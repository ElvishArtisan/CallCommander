// mlami.cpp
//
//   A connector for the Asterisk Manager Interface
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
//
//    $Id: mlami.cpp,v 1.5 2011/07/20 17:12:52 cvs Exp $
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU Library General Public License 
//   version 2 as published by the Free Software Foundation.
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

#include <sys/types.h>
#include <openssl/md5.h>

#include "mlami.h"

MLAmi::MLAmi(unsigned id,QObject *parent,const char *name)
  : QObject(parent,name)
{
  ami_id=id;
  ami_istate=0;
  ami_authenticated=false;
  ami_processing_event=false;
  ami_next_action_id=0;

  //
  // Initialize the Socket
  //
  ami_socket=new Q3Socket(this,"ami_socket");
  connect(ami_socket,SIGNAL(connected()),this,SLOT(connectedData()));
  connect(ami_socket,SIGNAL(readyRead()),this,SLOT(readyReadData()));
  connect(ami_socket,SIGNAL(error(int)),this,SLOT(errorData(int)));

  //
  // Initialize the Action ID Counter
  //
  ami_action_id=1;
}


void MLAmi::connectToHost(const QString &hostname,const QString &username,
			  const QString &passwd,Q_UINT16 port)
{
  ami_username=username;
  ami_password=passwd;
  ami_port=port;
  ami_socket->connectToHost(hostname,port);
}


unsigned MLAmi::sendAction(const QString &action,std::vector<QString> *labels,
			   std::vector<QString> *values)
{
  SendString(QString().sprintf("Action: %s",(const char *)action));
  for(unsigned i=0;i<labels->size();i++) {
    SendString(QString().sprintf("%s: %s",(const char *)labels->at(i),
				 (const char *)values->at(i)));
  }
  SendString(QString().sprintf("ActionID: %u",ami_next_action_id));	     
  SendString("");
  return ami_next_action_id++;
}


void MLAmi::connectedData()
{
  QString str;

  SendString("Action: Challenge");
  SendString("AuthType: MD5");
  SendString("");
}


void MLAmi::readyReadData()
{
  int n;
  char buf[1024];

  while((n=ami_socket->readBlock(buf,1024))>0) {
    buf[n]=0;
    // printf("RECV: %s\n",buf);
    for(int i=0;i<n;i++) {
      switch(ami_istate) {
	case 0:  // Label section
	  if(buf[i]==0x0D) {
	    DispatchLine(ami_label,"");
	    ami_label="";
	    ami_value="";
	    ami_istate=0;
	  }
	  else {
	    if(buf[i]==':') {
	      ami_istate=1;
	    }
	    else {
	      if(buf[i]!=0x0A) {
		ami_label+=buf[i];
	      }
	    }
	  }
	  break;

	case 1:  // Looking for start of value
	  if(buf[i]!=' ') {
	    ami_value=buf[i];
	    ami_istate=2;
	  }
	  break;

	case 2:  // Value section
	  if(buf[i]==0x0D) {
	    DispatchLine(ami_label,ami_value);
	    ami_label="";
	    ami_value="";
	    ami_istate=0;
	  }
	  else {
	    ami_value+=buf[i];
	  }
	  break;
      }
    }
  }
}


void MLAmi::errorData(int err)
{
}


void MLAmi::SendString(QString str)
{
  // printf("SendString(%s)\n",(const char *)str);
  str+="\r\n";
  ami_socket->writeBlock(str,str.length());
}


void MLAmi::DispatchLine(const QString &label,const QString &value)
{
  // printf("DispatchLine(%s,%s)\n",(const char *)label,
  //	 (const char *)value);
  if(!ami_authenticated) {
    Authenticate(label,value);
    return;
  }
  if(label.length()==0) {
    if(ami_labels.size()>0) {
      if(ami_processing_event) {
	emit eventReceived(ami_id,&ami_labels,&ami_values);
      }
      else {
	emit actionReceived(ami_id,ami_action_id,&ami_labels,&ami_values);
      }
    }
    ami_processing_event=false;
    ami_action_id=0;
    ami_labels.clear();
    ami_values.clear();
  }
  else {
    if(label.lower()=="event") {
      ami_processing_event=true;
      ami_labels.push_back(label);
      ami_values.push_back(value);
    }
    else {
      if(label.lower()=="actionid") {
	ami_action_id=value.toInt();
      }
      else {
	ami_labels.push_back(label);
	ami_values.push_back(value);
      }
    }
  }
}


void MLAmi::Authenticate(const QString &label,const QString &value)
{
//  unsigned char plaintext[value.length()+ami_password.length()+1];
  unsigned char plaintext[2048];
  unsigned char hashdata[MD5_DIGEST_LENGTH];
  QString response;
  QString str;

    if(label.lower()=="challenge") {
      strcpy((char *)plaintext,value);
      strcat((char *)plaintext,ami_password);
      MD5(plaintext,strlen((char *)plaintext),hashdata);
      for(int i=0;i<MD5_DIGEST_LENGTH;i++) {
	response+=QString().sprintf("%02x",hashdata[i]&0xff);
      }
      SendString("Action: Login");
      SendString("AuthType: MD5");
      SendString(QString().sprintf("Username: %s",(const char *)ami_username));
      SendString(QString().sprintf("Key: %s",(const char *)response));
      SendString("");
    }
    if((label.lower()=="message")&&
       (value.lower()=="authentication accepted")) {
      ami_authenticated=true;
      emit connected(ami_id,true);
    }
    if((label.lower()=="message")&&
       (value.lower()=="authentication failed")) {
      ami_authenticated=false;
      emit connected(ami_id,false);
    }
    return;
}
