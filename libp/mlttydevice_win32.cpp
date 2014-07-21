//   mlttydevice_win.cpp
//
//   A Qt driver for tty ports for win32 OSes.
//
//   (C) Copyright 2009,2010 Fred Gleason <fredg@paravelsystems.com>
//       All Rights Reserved.
//
//    $Id:
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

#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QtGui/QMessageBox>

#include "mlttydevice.h"

MLTTYDevice::MLTTYDevice(QObject *parent)
  : QIODevice(parent)
{
  tty_speed=9600;
  tty_length=8;
  tty_parity=MLTTYDevice::None;
  tty_flow_control=MLTTYDevice::FlowNone;
  tty_open=false;
  tty_notifier=NULL;

  tty_write_timer=new QTimer(this);
  tty_write_timer->setSingleShot(false);
  connect(tty_write_timer,SIGNAL(timeout()),this,SLOT(writeTtyData()));
}

MLTTYDevice::~MLTTYDevice()
{
  close();
  delete tty_write_timer;
}


QString MLTTYDevice::name() const
{
  return tty_name;
}


void MLTTYDevice::setName(const QString &str)
{
  tty_name=str;
}


bool MLTTYDevice::open(QIODevice::OpenMode mode)
{
  DWORD flags=0;
  WCHAR name[255];
  QString param_str;
  WCHAR params[255];
  DCB dcb;
  COMMTIMEOUTS timeouts;

  tty_mode=mode;
  if((mode&QIODevice::ReadWrite)==QIODevice::ReadWrite) {
    flags|=GENERIC_WRITE|GENERIC_READ;
  }
  else {
    if(((mode&QIODevice::WriteOnly)!=0)) {
      flags|=GENERIC_WRITE;
    }
    if(((mode&QIODevice::ReadOnly)!=0)) {
      flags|=GENERIC_READ;
    }
  }
  tty_name.toWCharArray(name);
  name[tty_name.length()]=0;
  tty_fd=CreateFile(name,flags,0,NULL,OPEN_EXISTING,
		    FILE_ATTRIBUTE_NORMAL,NULL);
  if(tty_fd==INVALID_HANDLE_VALUE) {
    //tty_status=IO_OpenError;
    return false;
  }
  tty_open=true;
  //tty_status=IO_Ok;

  SetupComm(tty_fd,WIN32_BUFFER_SIZE,WIN32_BUFFER_SIZE);
  switch(tty_parity) {
  case MLTTYDevice::None:
    param_str=QString().sprintf("%d,N,%d,1",tty_speed,tty_length);
    break;

  case MLTTYDevice::Even:
    param_str=QString().sprintf("%d,E,%d,1",tty_speed,tty_length);
    break;

  case MLTTYDevice::Odd:
    param_str=QString().sprintf("%d,O,%d,1",tty_speed,tty_length);
    break;
  }
  param_str.toWCharArray(params);
  params[param_str.length()]=0;
  BuildCommDCB(params,&dcb);
  SetCommState(tty_fd,&dcb);
  timeouts.ReadIntervalTimeout=MAXDWORD;
  timeouts.ReadTotalTimeoutMultiplier=0;
  timeouts.ReadTotalTimeoutConstant=0;
  timeouts.WriteTotalTimeoutMultiplier=0;
  timeouts.WriteTotalTimeoutConstant=0;
  SetCommTimeouts(tty_fd,&timeouts);

  return true;
/*
  int flags=O_NONBLOCK|O_NOCTTY;
  struct termios term;

  tty_mode=mode;
  if((mode&QIODevice::ReadWrite)==QIODevice::ReadWrite) {
    flags|=O_RDWR;
  }
  else {
    if(((mode&QIODevice::WriteOnly)!=0)) {
      flags|=O_WRONLY;
    }
    if(((mode&QIODevice::ReadOnly)!=0)) {
      flags|=O_RDONLY;
    }
  }
  if((mode&QIODevice::Append)!=0) {
    flags|=O_APPEND;
  }
  if((mode&QIODevice::Truncate)!=0) {
    flags|=O_TRUNC;
  }

  if((tty_fd=::open(tty_name.toAscii(),flags))<0) {
    return false;
  }
  tty_open=true;

  tcgetattr(tty_fd,&term);

  //
  // Set Speed
  //
  cfsetispeed(&term,B0);
  cfsetospeed(&term,tty_speed);

  //
  // Set Mode
  //
  cfmakeraw(&term);
  term.c_iflag |= IGNBRK; 

  //
  // Set Parity
  //
  switch(tty_parity) {
  case MLTTYDevice::None:
    term.c_iflag |= IGNPAR;
    break;

  case MLTTYDevice::Even:
    term.c_cflag |= PARENB;
    break;

  case MLTTYDevice::Odd:
    term.c_cflag |= PARENB|PARODD;
    break;
  }

  //
  // Set Word Length
  //
  term.c_cflag &= ~CSIZE;
  term.c_cflag |= tty_length;

  //
  // Set Flow Control
  //
  switch(tty_flow_control) {
  case MLTTYDevice::FlowNone:
    term.c_cflag &= ~CRTSCTS;
    term.c_iflag &= ~IXON;
    term.c_iflag &= ~IXOFF;
    break;

  case MLTTYDevice::FlowRtsCts:
    term.c_cflag |= CRTSCTS;
    term.c_iflag &= ~IXON;
    term.c_iflag &= ~IXOFF;
    break;

  case MLTTYDevice::FlowXonXoff:
    term.c_cflag &= ~CRTSCTS;
    term.c_iflag |= IXON;
    term.c_iflag |= IXOFF;
    break;
  }

  tcsetattr(tty_fd,TCSADRAIN,&term);

  tty_notifier=new QSocketNotifier(tty_fd,QSocketNotifier::Read,this);
  connect(tty_notifier,SIGNAL(activated(int)),this,SLOT(readTtyData(int)));

  tty_write_timer->start(10);
*/
  return true;
}


void MLTTYDevice::close()
{
  if(tty_open) {
    emit aboutToClose();
    tty_write_timer->stop();
    delete tty_notifier;
    tty_notifier=NULL;
    CloseHandle(tty_fd);
    if((tty_mode&QIODevice::ReadOnly)!=0) {
      emit readChannelFinished();
    }
  }
  tty_open=false;
/*
  if(tty_open) {
    emit aboutToClose();
    tty_write_timer->stop();
    delete tty_notifier;
    tty_notifier=NULL;
    ::close(tty_fd);
    if((tty_mode&QIODevice::ReadOnly)!=0) {
      emit readChannelFinished();
    }
  }
  tty_open=false;
*/
}


qint64 MLTTYDevice::read(char *data,qint64 maxlen)
{
  return readData(data,maxlen);
}


QByteArray MLTTYDevice::read(qint64 maxlen)
{
  qint64 n=0;
  char *data=new char[maxlen];
  n=readData(data,maxlen);
  QByteArray ret(data,n);
  delete data;
  return ret;
}


QByteArray MLTTYDevice::readAll()
{
  return read(bytesAvailable());
}


qint64 MLTTYDevice::readBlock(char *data,qint64 maxlen)
{
  return readData(data,maxlen);
}


qint64 MLTTYDevice::write(const char *data,qint64 len)
{
  QMessageBox::information(NULL,"write","write");
  return writeData(data,len);
}


qint64 MLTTYDevice::write(const QByteArray &array)
{
  return write(array.constData(),array.size());
}


bool MLTTYDevice::getChar(char *ch)
{
  return readData(ch,1)==1;
}


bool MLTTYDevice::putChar(char ch)
{
  return writeData(&ch,1)==1;
}


qint64 MLTTYDevice::size() const
{
  return bytesAvailable();
}


qint64 MLTTYDevice::bytesAvailable() const
{
  int val=0;
/*
  if(tty_open) {
    ioctl(tty_fd,FIONREAD,&val);
  }
*/
  return val;
}


qint64 MLTTYDevice::bytesToWrite() const
{
  int val=0;
/*
  if(tty_open) {
    ioctl(tty_fd,TIOCOUTQ,&val);
  }
*/
  return val;
}


bool MLTTYDevice::isSequential() const
{
  return true;
}


bool MLTTYDevice::isReadable() const
{
  return((tty_mode&QIODevice::ReadOnly)!=0)||
    ((tty_mode&QIODevice::ReadWrite)!=0);
}


bool MLTTYDevice::isWritable() const
{
  return ((tty_mode&QIODevice::WriteOnly)!=0)||
    ((tty_mode&QIODevice::ReadWrite)!=0);
}


bool MLTTYDevice::isOpen() const
{
  return tty_open;
}


QString MLTTYDevice::deviceName() const
{
  return tty_name;
}


void MLTTYDevice::setDeviceName(QString name)
{
  tty_name=name;
}


int MLTTYDevice::speed() const
{
  return tty_speed;
/*
  switch(tty_speed) {
  case B0:
    return 0;
    break;

  case B50:
    return 50;
    break;

  case B75:
    return 75;
    break;

  case B110:
    return 110;
    break;

  case B134:
    return 134;
    break;

  case B150:
    return 150;
    break;

  case B200:
    return 200;
    break;

  case B300:
    return 300;
    break;

  case B600:
    return 600;
    break;

  case B1200:
    return 1200;
    break;

  case B1800:
    return 1800;
    break;

  case B2400:
    return 2400;
    break;

  case B4800:
    return 4800;
    break;

  case B9600:
    return 9600;
    break;

  case B19200:
    return 19200;
    break;

  case B38400:
    return 38400;
    break;

  case B57600:
    return 57600;
    break;

  case B115200:
    return 115200;
    break;

  case B230400:
    return 230400;
    break;
  }
*/
  return 0;
}


void MLTTYDevice::setSpeed(int speed)
{
  tty_speed=speed;
/*
  switch(speed) {
  case 0:
    tty_speed=B0;
    break;

  case 50:
    tty_speed=B50;
    break;

  case 75:
    tty_speed=B75;
    break;

  case 110:
    tty_speed=B110;
    break;

  case 134:
    tty_speed=B134;
    break;

  case 150:
    tty_speed=B150;
    break;

  case 200:
    tty_speed=B200;
    break;

  case 300:
    tty_speed=B300;
    break;

  case 600:
    tty_speed=B600;
    break;

  case 1200:
    tty_speed=B1200;
    break;

  case 1800:
    tty_speed=B1800;
    break;

  case 2400:
    tty_speed=B2400;
    break;

  case 4800:
    tty_speed=B4800;
    break;

  case 9600:
    tty_speed=B9600;
    break;

  case 19200:
    tty_speed=B19200;
    break;

  case 38400:
    tty_speed=B38400;
    break;

  case 57600:
    tty_speed=B57600;
    break;

  case 115200:
    tty_speed=B115200;
    break;

  case 230400:
    tty_speed=B230400;
    break;

  default:
    tty_speed=B9600;
    break;
  }
*/
}


int MLTTYDevice::wordLength() const
{
  return tty_length;
/*
  switch(tty_length) {
  case CS5:
    return 5;
    break;

  case CS6:
    return 6;
    break;

  case CS7:
    return 7;
    break;

  case CS8:
    return 8;
    break;
  }
*/
  return 0;
}


void MLTTYDevice::setWordLength(int length)
{
  tty_length=length;
/*
  switch(length) {
  case 5:
    tty_length=CS5;
    break;

  case 6:
    tty_length=CS6;
    break;

  case 7:
    tty_length=CS7;
    break;

  case 8:
    tty_length=CS8;
    break;

  default:
    tty_length=CS8;
    break;
  }
*/
}


MLTTYDevice::Parity MLTTYDevice::parity() const
{
  return tty_parity;
}


void MLTTYDevice::setParity(Parity parity)
{
  tty_parity=parity;
}


MLTTYDevice::FlowControl MLTTYDevice::flowControl() const
{
  return tty_flow_control;
}


void MLTTYDevice::setFlowControl(FlowControl ctrl)
{
  tty_flow_control=ctrl;
}


qint64 MLTTYDevice::readData(char *data,qint64 maxlen)
{
  DWORD n;

  if(!ReadFile(tty_fd,data,maxlen,&n,NULL)) {
    if(GetLastError()!=ERROR_TIMEOUT) {
      //tty_status=IO_ReadError;
      return -1;
    }
  }
  //tty_status=IO_Ok;
  return n;


/*
  qint64 n;

  if((n=::read(tty_fd,data,(size_t)maxlen))<0) {
    return 0;
  }

  return n;
*/
}


qint64 MLTTYDevice::writeData(const char *data,qint64 len)
{
  DWORD n;

  if(!WriteFile(tty_fd,data,len,&n,NULL)) {
    //tty_status=IO_WriteError;
    return n;
  }
  //tty_status=IO_Ok;
  return n;



/*
  for(qint64 i=0;i<len;i++) {
    tty_write_queue.push(data[i]);
  }
  emit bytesWritten(len);
  return len;
*/
}


void MLTTYDevice::readTtyData(int sock)
{
  emit readyRead();
}


void MLTTYDevice::writeTtyData()
{
/*
  char data[2048];
  int bytes=0;

  ioctl(tty_fd,TIOCOUTQ,&bytes);
  int n=2048-bytes;
  if(tty_write_queue.size()<n) {
    n=tty_write_queue.size();
  }


  for(ssize_t i=0;i<n;i++) {
    data[i]=tty_write_queue.front();
    tty_write_queue.pop();
  }
  ::write(tty_fd,data,n);
*/
}
