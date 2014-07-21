/* ********************************************************************
// Copyright (C) 2005 Telos Systems, TLS Corp.
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
//     * Redistributions of source code must retain the above
//       copyright notice, this list of conditions and the following
//       disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials
//       provided with the distribution.
//     * Neither the name of the TLS Corporation nor the names of its
//       contributors may be used to endorse or promote products
//       derived from this software without specific prior written
//       permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
// --------------------------------------------------------------------
//          File: tlsaplib.cpp
//   Description: 2101 SDK
//        Author: Ioan L. Rus
// Linux version: Oleg Krylov
//          Date: Apr 25, 2005
//     Revisions: 
// --------------------------------------------------------------------
// Date         Description
// ------------ -------------------------------------------------------
// 
// ********************************************************************
*/
#include "tlsaplib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ------------------------- * sockets * ------------------------- */
#ifdef WIN32 /* Assume MSVC @ Win32 compiler */
#include <Winsock.h>
#pragma comment(lib, "Wsock32")

#define socket_t        SOCKET
#define socketerrno     WSAGetLastError()
typedef int socklen_t;

#define EWOULDBLOCK     (WSAEWOULDBLOCK)

static __inline int sysInitSocks(void)
{
  WSADATA wsaData;
  int err;

  /* initilaize windows sockets */
  err=WSAStartup(MAKEWORD(1,1),&wsaData);
  return err==0?1:0;
}

static __inline int sysShutSocks(void)
{
  int err;

  /* cleannup windows sockets */
  err=WSACleanup();
  return err==0?1:0;
}

#ifdef TLSAP_CONF_THREAD
typedef Qt::HANDLE sysMutex_t;

static __inline int sysInitMutex(sysMutex_t *m)
{
  if(!m)
  {
    return 0;
  }
  *m=CreateMutex(NULL,FALSE,NULL);
  return *m?1:0;
}

static __inline void sysShutMutex(sysMutex_t *m)
{
  if(m&&*m)
  {
    CloseHandle(*m);
  }
}

static __inline void sysLockMutex(sysMutex_t *m)
{
  if(m&&*m)
  {
    WaitForSingleObject(*m,INFINITE);
  }
}

static __inline void sysUnlockMutex(sysMutex_t *m)
{
  if(m&&*m)
  {
    ReleaseMutex(*m);
  }
}
#endif
#else /* Assume GCC @ posix compiler */
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#define closesocket(s)              close(s)
#define ioctlsocket(s, cmd, argp)   ioctl(s, cmd, argp)
#define socketerrno                 (errno)

#define socket_t int

#ifdef TLSAP_CONF_THREAD
#include <pthread.h>

typedef pthread_mutex_t sysMutex_t;

static __inline int sysInitMutex(sysMutex_t *m)
{
  if(!m)
  {
    return 0;
  }
  return pthread_mutex_init(m,NULL)==0?1:0;
}

static __inline void sysShutMutex(sysMutex_t *m)
{
  if(m)
  {
    pthread_mutex_destroy(m);
  }
}

static __inline void sysLockMutex(sysMutex_t *m)
{
  if(m)
  {
    pthread_mutex_lock(m);
  }
}

static __inline void sysUnlockMutex(sysMutex_t *m)
{
  if(m)
  {
    pthread_mutex_unlock(m);
  }
}

#endif

static __inline int sysInitSocks(void)
{
  return 1;
}
static __inline int sysShutSocks(void)
{
  return 1;
}
#endif

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif
#ifndef MSG_WAITALL
#define MSG_WAITALL 0
#endif

/* ------------------------- * types * ------------------------- */
#ifdef WIN32
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
#else
typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
#endif  // WIN32

typedef struct TlsAP_Client_s
{
  /* user defined data */
  void *userData;
  TlsAP_Callbacks eventHandlers;

  /* private data */
  socket_t socket;

#ifdef TLSAP_CONF_THREAD
  sysMutex_t send_mutex;
#endif
} TlsAP_Client_t;

/* ------------------------- * aux * ------------------------- */
typedef struct netIPaddr_s
{
  byte addr[4];
  word port;
} netIPaddr_t;

static netIPaddr_t *Net_ParseIPaddr(netIPaddr_t *addr,const char *host,word port);
static socket_t Net_TCPconnect(netIPaddr_t *addr,int *socket);
static socket_t Net_TCPconnectEx(const char *host,word port,int *socket);
static int Net_TCPwrite(socket_t sock,const void *data,int len);
static int Net_TCPread(socket_t sock,void *data,int len);

/* ------------------------- * functions * ------------------------- */

/*
** TlsAP_Create
**
** creates TlsAP client
*/
TlsAP_Client_t * TlsAP_Create(void *userData,TlsAP_Callbacks *eventHandlers)
{
  TlsAP_Client_t *p;

  if(!sysInitSocks())
  {
    return NULL;
  }
  p=(TlsAP_Client_t*)malloc(sizeof(TlsAP_Client_t));
  if(!p)
  {
    sysShutSocks();
    return NULL;
  }

  p->userData=userData;
  p->eventHandlers= *eventHandlers;
  p->socket= -1;

#ifdef TLSAP_CONF_THREAD
  if(!sysInitMutex(&p->send_mutex))
  {
    free(p);
    sysShutSocks();
    return NULL;
  }
#endif

  return p;
}

/*
** TlsAP_Destroy
**
** destroys TlsAP client; if connection is in progress it is dropped automatically
*/
int TlsAP_Destroy(TlsAP_Client_t *client)
{
  if(client)
  {
    TlsAP_Disconnect(client);
#ifdef TLSAP_CONF_THREAD
    sysShutMutex(&client->send_mutex);
#endif
    free(client);
    sysShutSocks();
  }
  return 1;
}

/* ------------------------- * private Cmsg sending stuff * ------------------------- */
#define nID(a,b,c,d)    ((d)|((c)<<8)|((b)<<16)|((a)<<24))
#define hID(a,b,c,d)    ((a)|((b)<<8)|((c)<<16)|((d)<<24))
#define nsID(s)         (nID((s)[0],(s)[1],(s)[2],(s)[3]))
#define hsID(s)         (hID((s)[0],(s)[1],(s)[2],(s)[3]))

#define CMSG_DWORD              1
#define CMSG_STRING             2
#define CMSG_BYTEARRAY          3
#define CMSG_WORDARRAY          4
#define CMSG_DWORDARRAY         5
#define CMSG_NESTMESSAGE        6
#define CMSG_BYTE               7
#define CMSG_WORD               8
#define CMSG_QWORD              9
#define CMSG_QWORDARRAY         10

#define CMSG_MAXSIZE            (1024*8) /* 8Kb message limit */

typedef struct cmsgVal_s
{
  dword id;
  byte type;

  union
  {
    byte vByte;
    word vWord;
    dword vDword;
    struct
    {
      word size;
      const void *p;
    } vArray;
  } v;
} cmsgVal_t;

#define SET_BYTE_VAL(dst, sid, val)   do{(dst).id=nsID(sid); (dst).type=CMSG_BYTE; (dst).v.vByte=(byte)(val);}while(0)
#define SET_WORD_VAL(dst, sid, val)   do{(dst).id=nsID(sid); (dst).type=CMSG_WORD; (dst).v.vWord=(word)(val);}while(0)
#define SET_DWORD_VAL(dst, sid, val)  do{(dst).id=nsID(sid); (dst).type=CMSG_DWORD; (dst).v.vDword=(dword)(val);}while(0)
#define SET_STRING_VAL(dst, sid, val) do{(dst).id=nsID(sid); (dst).type=CMSG_STRING; (dst).v.vArray.size=(word)(strlen(val)+1); (dst).v.vArray.p=(val);}while(0)

static __inline void setWord(byte *dst,word val)
{
  dst[0]=(byte)((val>>8)&0xFF);
  dst[1]=(byte)((val)&0xFF);
}

static __inline void setDword(byte *dst,dword val)
{
  dst[0]=(byte)((val>>24)&0xFF);
  dst[1]=(byte)((val>>16)&0xFF);
  dst[2]=(byte)((val>>8)&0xFF);
  dst[3]=(byte)((val)&0xFF);
}

static __inline word getWord(const byte *src)
{
  return (src[0]<<8)|src[1];
}

static __inline dword getDword(const byte *src)
{
  return (src[0]<<24)|(src[1]<<16)|(src[2]<<8)|src[3];
}

enum {
  APCLITOSRV_CONNECT,
  APCLITOSRV_ENUMERATESHOWS,
  APCLITOSRV_ENUMERATEHOSTSFORSHOW,
  APCLITOSRV_CHANGESHOW,
  APCLITOSRV_CONNECTTOSHOW,
  APCLITOSRV_ENUMERATEDIRECTORS,
  APCLITOSRV_ATTACHTODIRECTOR,
  APCLITOSRV_ATTACHASTALENT,
  APCLITOSRV_POSTBUSYALLBUTTON,
  APCLITOSRV_POSTDELAYDUMPBUTTON,
  APCLITOSRV_POSTDIALKEY,
  APCLITOSRV_POSTDROPBUTTON,
  APCLITOSRV_POSTHOLDBUTTON,
  APCLITOSRV_POSTLINEBUTTON,
  APCLITOSRV_POSTMUTEBUTTON,
  APCLITOSRV_POSTNEXTBUTTON,
  APCLITOSRV_POSTRECORDBUTTON,
  APCLITOSRV_POSTSPEAKERBUTTON,
  APCLITOSRV_POSTTEXT,
  APCLITOSRV_POSTTRANSFERBUTTON,
  APCLITOSRV_POSTUPDATEALL,
  APCLITOSRV_ENUMERATESHOWSFORHOST,
  APCLITOSRV_MAX_INDEX
};

enum {
  APSRVTOCLI_ENUMERATESHOWSCALLBACK,
  APSRVTOCLI_ENUMERATEHOSTSCALLBACK,
  APSRVTOCLI_ENUMERATEDIRECTORSCALLBACK,
  APSRVTOCLI_SENDSHOWINFO,
  APSRVTOCLI_SENDMODE,
  APSRVTOCLI_SENDSHOWCHANGE,
  APSRVTOCLI_SENDRECORD,
  APSRVTOCLI_SENDBUSYALL,
  APSRVTOCLI_SENDTRANSFER,
  APSRVTOCLI_SENDMUTE,
  APSRVTOCLI_SENDRINGMUTE,
  APSRVTOCLI_SENDDELAYDUMP,
  APSRVTOCLI_SENDHANDSET,
  APSRVTOCLI_SENDSPEAKER,
  APSRVTOCLI_SENDLINESTATE,
  APSRVTOCLI_SENDSEIZELINE,
  APSRVTOCLI_SENDRINGLINE,
  APSRVTOCLI_SENDDROPLINE,
  APSRVTOCLI_SENDANSWERLINE,
  APSRVTOCLI_SENDSELECTLINE,
  APSRVTOCLI_SENDDIALKEY,
  APSRVTOCLI_SENDTEXT,
  APSRVTOCLI_SENDERROR,
  APSRVTOCLI_ENUMERATESHOWSFORHOSTCALLBACK,
  APSRVTOCLI_MAX_INDEX
};

typedef struct ph_evParams_s
{
  const char *showName;
  const char *hostName;
  const char *directorName;
  const char *newShowName;
  const char *oldShowName;
  const char *callerId;
  const char *text;
  const char *errorText;
  dword ret;
  dword rqPass;
  dword isActive;
  dword isLast;
  dword session;
  dword isTalent;
  dword isFree;
  dword maxLines;
  dword maxHybrids;
  dword on;
  dword line;
  dword column;
  dword key;
  dword lineState;
  dword elpassedTimeSec;
  dword textType;
  dword errorNumber;
} tlsParams_t;

/*
** TlsAP_ParseParams
*/
static int TlsAP_ParseParams(tlsParams_t *params,dword *pid,const byte *p,dword len)
{
  dword elems, size;
  dword id, msgid;
  int type;

  /* check header len */
  if(!params||!p||len<6)
  {
    return 0;
  }

  msgid=getDword(p); p+=4; len-=4;
  elems=getWord(p); p+=2, len-=2;

  memset(params,0,sizeof(tlsParams_t));

  while(elems--)
  {
    /* element id & type */
    if(len<5)
    {
      return 0;
    }
    id=getDword(p); p+=4; len-=4;
    type= *p; p++; len--;

    switch(type)
    {
      /* scalar elements */
    case CMSG_BYTE:
      if(len<1)
      {
        return 0;
      }
      p++; len--;
      break;
    case CMSG_WORD:
      if(len<2)
      {
        return 0;
      }
      p+=2; len-=2;
      break;
    case CMSG_DWORD:
      if(len<4)
      {
        return 0;
      }
      switch(id)
      {
      case nID('P','A','S','S'):
        params->rqPass=getDword(p);
        break;
      case nID('A','C','T','V'):
        params->isActive=getDword(p);
        break;
      case nID('L','A','S','T'):
        params->isLast=getDword(p);
        break;
      case nID('S','S','N','_'):
        params->session=getDword(p);
        break;
      case nID('T','L','N','T'):
        params->isTalent=getDword(p);
        break;
      case nID('F','R','E','E'):
        params->isFree=getDword(p);
        break;
      case nID('N','L','I','N'):
        params->maxLines=getDword(p);
        break;
      case nID('N','H','Y','B'):
        params->maxHybrids=getDword(p);
        break;
      case nID('O','N','_','_'):
        params->on=getDword(p);
        break;
      case nID('L','I','N','E'):
        params->line=getDword(p);
        break;
      case nID('C','O','L','_'):
        params->column=getDword(p);
        break;
      case nID('S','T','A','T'):
        params->lineState=getDword(p);
        break;
      case nID('T','I','M','E'):
        params->elpassedTimeSec=getDword(p);
        break;
      case nID('T','T','Y','P'):
        params->textType=getDword(p);
        break;
      case nID('E','N','U','M'):
        params->errorNumber=getDword(p);
        break;
      case nID('R','E','T','_'):
        params->ret=getDword(p);
        break;
      case nID('K','E','Y','_'):
        params->key=getDword(p);
        break;
      }
      p+=4; len-=4;
      break;
    case CMSG_QWORD:
      if(len<8)
      {
        return 0;
      }
      p+=8; len-=8;
      break;

      /* array elements */
    case CMSG_STRING:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;
      if(len<size||(size>0&&p[size-1]!=0))
      {
        return 0;
      }
      switch(id)
      {
      case nID('S','H','O','W'):
        params->showName=(size>0?(const char*)p:"");
        break;
      case nID('H','O','S','T'):
        params->hostName=(size>0?(const char*)p:"");
        break;
      case nID('D','N','A','M'):
        params->directorName=(size>0?(const char*)p:"");
        break;
      case nID('N','E','W','S'):
        params->newShowName=(size>0?(const char*)p:"");
        break;
      case nID('O','L','D','S'):
        params->oldShowName=(size>0?(const char*)p:"");
        break;
      case nID('C','L','I','D'):
        params->callerId=(size>0?(const char*)p:"");
        break;
      case nID('T','E','X','T'):
        params->text=(size>0?(const char*)p:"");
        break;
      case nID('E','T','X','T'):
        params->errorText=(size>0?(const char*)p:"");
        break;
      }
      p+=size; len-=size;
      break;
    case CMSG_BYTEARRAY:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;
      if(len<size)
      {
        return 0;
      }
      p+=size; len-=size;
      break;
    case CMSG_WORDARRAY:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;
      if(len<size*2)
      {
        return 0;
      }
      p+=2*size; len-=2*size;
      break;
    case CMSG_DWORDARRAY:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;
      if(len<size*4)
      {
        return 0;
      }
      p+=4*size; len-=4*size;
      break;
    case CMSG_QWORDARRAY:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;
      if(len<size*8)
      {
        return 0;
      }
      p+=size*8; len-=size*8;
      break;
      /* nested message */
    case CMSG_NESTMESSAGE:
      if(len<2)
      {
        return 0;
      }
      size=getWord(p); p+=2; len-=2;

      if(len<size)
      {
        return 0;
      }
      /* just skip child message */
      p+=size; len-=size;
      break;
    default:
      return 0;
    }
  }

  if(len)
  {
    return 0;
  } /* data left in buffer, something was wrong! */
  if(pid)
  {
    *pid=msgid;
  }
  return 1;
}

typedef struct tlsMsg_s
{
  dword size;
  byte *data;
} tlsMsg_t;

/*
** TlsAP_Send
*/
static int TlsAP_Send(TlsAP_Client_t *client,tlsMsg_t *msg)
{
  byte sz_data[8];
  int res;

  if(!client||client->socket==-1||!msg||!msg->size||!msg->data)
  {
    return 0;
  }

  /* send message */
  setDword(sz_data+0,msg->size);
  setDword(sz_data+4,msg->size^0xA5A55A5A);

#ifdef TLSAP_CONF_THREAD
  /* lock socket mutex */
  sysLockMutex(&client->send_mutex);
#endif
  if(!Net_TCPwrite(client->socket,sz_data,8)||!Net_TCPwrite(client->socket,msg->data,msg->size))
  {
    res=0;
  }
  else
  {
    res=1;
  }
#ifdef TLSAP_CONF_THREAD
  /* unlock socket mutex */
  sysUnlockMutex(&client->send_mutex);
#endif
  return res;
}

/*
** TlsAP_Recv
*/
static int TlsAP_Recv(TlsAP_Client_t *client,tlsMsg_t *msg,unsigned long timeout)
{
  byte sz_data[8];
  fd_set fds;
  dword sz;
  int res;

  if(!client||client->socket==-1||!msg)
  {
    return -1;
  }

  /* wait for connection, using select */
  FD_ZERO(&fds);
  FD_SET(client->socket,&fds);

  if(timeout==(unsigned long)-1)
  {
    res=select((int)client->socket+1,&fds,0,0,NULL);
  }
  else
  {
    struct timeval tv;

    tv.tv_sec=(long)(timeout/1000);
    tv.tv_usec=(long)((timeout%1000)*1000);

    res=select((int)client->socket+1,&fds,0,0,&tv);
  }
  if(client->socket==-1)
  {
    return -1;
  }
  if(res<=0)
  {
    return 0;
  }

  /* read message */
  res=0;

  if(Net_TCPread(client->socket,sz_data,8))
  {
    sz=getDword(sz_data+0);
    if(sz>msg->size||sz!=(getDword(sz_data+4)^0xA5A55A5A))
    {
    printf("HERE 11\n");
      res= -1;
    }
    else if(!Net_TCPread(client->socket,msg->data,sz))
    {
      res= -1;
    }
    else
    {
      res=1;
      msg->size=sz;
    }
  }

  return res;
}

/*
** TlsAP_SendMsg
*/
static int TlsAP_SendMsg(TlsAP_Client_t *client,dword id,cmsgVal_t *values,word num)
{
  byte buffer[CMSG_MAXSIZE], *p;
  tlsMsg_t tmsg;
  dword sz;
  word n, len, m;

  /* calculate size */
  sz=6+5*num;
  for(n=0; n<num; n++)
  {
    switch(values[n].type)
    {
    case CMSG_DWORD:
      sz+=4;
      break;
    case CMSG_STRING:
      sz+=2+values[n].v.vArray.size;
      break;
    case CMSG_BYTEARRAY:
      sz+=2+values[n].v.vArray.size;
      break;
    case CMSG_WORDARRAY:
      sz+=2+values[n].v.vArray.size*2;
      break;
    case CMSG_DWORDARRAY:
      sz+=2+values[n].v.vArray.size*4;
      break;
    case CMSG_NESTMESSAGE:
      sz+=2+values[n].v.vArray.size;
      break;
    case CMSG_BYTE:
      sz+=1;
      break;
    case CMSG_WORD:
      sz+=2;
      break;
    default:
      return 0;
    }
  }

  /* allocate */
  if(sz>sizeof(buffer))
  {
    return 0;
  } /* message too big */
  p=buffer;
  tmsg.data=buffer;
  tmsg.size=sz;

  /* fill */
  setDword(p,id); p+=4;
  setWord(p,num); p+=2;

  for(n=0; n<num; n++)
  {
    setDword(p,values[n].id); p+=4;
    *p++=values[n].type;

    switch(values[n].type)
    {
    case CMSG_DWORD:
      setDword(p,values[n].v.vDword); p+=4;
      break;
    case CMSG_STRING:
      setWord(p,len=values[n].v.vArray.size); p+=2;
      memcpy(p,values[n].v.vArray.p,len); p+=len;
      break;
    case CMSG_BYTEARRAY:
      setWord(p,len=values[n].v.vArray.size); p+=2;
      memcpy(p,values[n].v.vArray.p,len); p+=len;
      break;
    case CMSG_WORDARRAY:
      {
        word *src;

        setWord(p,len=values[n].v.vArray.size); p+=2;
        for(m=0, src=(word*)values[n].v.vArray.p; m<len; m++, src++)
        {
          setWord(p,*src); p+=2;
        }
        break;
      }
    case CMSG_DWORDARRAY:
      {
        dword *src;

        setWord(p,len=values[n].v.vArray.size); p+=2;
        for(m=0, src=(dword*)values[n].v.vArray.p; m<len; m++, src++)
        {
          setDword(p,*src); p+=4;
        }
        break;
      }
    case CMSG_NESTMESSAGE:
      setWord(p,len=values[n].v.vArray.size); p+=2;
      memcpy(p,values[n].v.vArray.p,len); p+=len;
      break;
    case CMSG_BYTE:
      *p++=values[n].v.vByte;
      break;
    case CMSG_WORD:
      setWord(p,values[n].v.vWord); p+=2;
      break;
    default:
      return 0;
    }
  }

  return TlsAP_Send(client,&tmsg);
}

/* ------------------------- * methods * ------------------------- */

/*
** TlsAP_WaitEvents
**
** wait for events
** returns: -1 on connection error; 0 on timeout; >0 on success
*/
int TlsAP_WaitEvents(TlsAP_Client_t *client,unsigned long timeout)
{
  byte buffer[CMSG_MAXSIZE];
  tlsParams_t params;
  tlsMsg_t tmsg;
  dword id;
  int ret;

  /* wait for message */
  tmsg.size=sizeof(buffer);
  tmsg.data=buffer;
  ret=TlsAP_Recv(client,&tmsg,timeout);
  if(ret<=0)
  {
    return ret;
  }
  ret=1;
  if(!TlsAP_ParseParams(&params,&id,tmsg.data,tmsg.size))
  {
    ret= -2;
  }
  else
  {
    switch(id)
    {
    case APSRVTOCLI_ENUMERATESHOWSCALLBACK:
      if(client->eventHandlers.EnumerateShowsCallback)
      {
        client->eventHandlers.EnumerateShowsCallback(client->userData,params.showName,params.rqPass,params.isActive,params.hostName,params.isLast);
      }
      break;
    case APSRVTOCLI_ENUMERATESHOWSFORHOSTCALLBACK:
      if(client->eventHandlers.EnumerateShowsForHostCallback)
      {
        client->eventHandlers.EnumerateShowsForHostCallback(client->userData,params.showName,params.isLast);
      }
      break;
    case APSRVTOCLI_ENUMERATEHOSTSCALLBACK:
      if(client->eventHandlers.EnumerateHostsCallback)
      {
        client->eventHandlers.EnumerateHostsCallback(client->userData,params.hostName,params.isLast);
      }
      break;
    case APSRVTOCLI_ENUMERATEDIRECTORSCALLBACK:
      if(client->eventHandlers.EnumerateDirectorsCallback)
      {
        client->eventHandlers.EnumerateDirectorsCallback(client->userData,params.session,params.directorName,params.isTalent,params.isFree,params.isLast);
      }
      break;
    case APSRVTOCLI_SENDSHOWINFO:
      if(client->eventHandlers.SendShowInfo)
      {
        client->eventHandlers.SendShowInfo(client->userData,params.session,params.maxLines,params.maxHybrids);
      }
      break;
    case APSRVTOCLI_SENDMODE:
      if(client->eventHandlers.SendMode)
      {
        client->eventHandlers.SendMode(client->userData,params.session,params.isTalent);
      }
      break;
    case APSRVTOCLI_SENDSHOWCHANGE:
      if(client->eventHandlers.SendShowChange)
      {
        client->eventHandlers.SendShowChange(client->userData,params.session,params.newShowName,params.oldShowName);
      }
      break;
    case APSRVTOCLI_SENDRECORD:
      if(client->eventHandlers.SendRecord)
      {
        client->eventHandlers.SendRecord(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDBUSYALL:
      if(client->eventHandlers.SendBusyAll)
      {
        client->eventHandlers.SendBusyAll(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDTRANSFER:
      if(client->eventHandlers.SendTransfer)
      {
        client->eventHandlers.SendTransfer(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDMUTE:
      if(client->eventHandlers.SendMute)
      {
        client->eventHandlers.SendMute(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDRINGMUTE:
      if(client->eventHandlers.SendRingMute)
      {
        client->eventHandlers.SendRingMute(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDDELAYDUMP:
      if(client->eventHandlers.SendDelayDump)
      {
        client->eventHandlers.SendDelayDump(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDHANDSET:
      if(client->eventHandlers.SendHandset)
      {
        client->eventHandlers.SendHandset(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDSPEAKER:
      if(client->eventHandlers.SendSpeaker)
      {
        client->eventHandlers.SendSpeaker(client->userData,params.session,params.on);
      }
      break;
    case APSRVTOCLI_SENDLINESTATE:
      if(client->eventHandlers.SendLineState)
      {
        client->eventHandlers.SendLineState(client->userData,params.session,params.line,params.column,params.lineState,params.elpassedTimeSec);
      }
      break;
    case APSRVTOCLI_SENDSEIZELINE:
      if(client->eventHandlers.SendSeizeLine)
      {
        client->eventHandlers.SendSeizeLine(client->userData,params.session,params.line,params.column);
      }
      break;
    case APSRVTOCLI_SENDRINGLINE:
      if(client->eventHandlers.SendRingLine)
      {
        client->eventHandlers.SendRingLine(client->userData,params.session,params.callerId,params.line);
      }
      break;
    case APSRVTOCLI_SENDDROPLINE:
      if(client->eventHandlers.SendDropLine)
      {
        client->eventHandlers.SendDropLine(client->userData,params.session,params.callerId,params.line);
      }
      break;
    case APSRVTOCLI_SENDANSWERLINE:
      if(client->eventHandlers.SendAnswerLine)
      {
        client->eventHandlers.SendAnswerLine(client->userData,params.session,params.callerId,params.line,params.column);
      }
      break;
    case APSRVTOCLI_SENDSELECTLINE:
      if(client->eventHandlers.SendSelectLine)
      {
        client->eventHandlers.SendSelectLine(client->userData,params.session,params.callerId,params.line,params.column);
      }
      break;
    case APSRVTOCLI_SENDDIALKEY:
      if(client->eventHandlers.SendDialKey)
      {
        client->eventHandlers.SendDialKey(client->userData,params.session,params.key);
      }
      break;
    case APSRVTOCLI_SENDTEXT:
      if(client->eventHandlers.SendText)
      {
        client->eventHandlers.SendText(client->userData,params.session,params.textType,params.text,params.line);
      }
      break;
    case APSRVTOCLI_SENDERROR:
      if(client->eventHandlers.SendError)
      {
        client->eventHandlers.SendError(client->userData,params.session,params.errorNumber,params.errorText);
      }
      break;
    default:
      ret= -3;
      break;
    }
  }

  return ret;
}

/*
** TlsAP_Connect
*/
int TlsAP_Connect(struct TlsAP_Client_s *client,const char *strHostPort,const char *strUser,const char *strPassword,unsigned short tcpPort,int *socket)
{
  byte buffer[CMSG_MAXSIZE];
  tlsParams_t params;
  tlsMsg_t tmsg;
  cmsgVal_t val[2];
  int ret;

  if(!client||!strHostPort||!strUser||!strPassword)
  {
    return 0;
  }

  TlsAP_Disconnect(client);
  client->socket=Net_TCPconnectEx(strHostPort,tcpPort,socket);
  if(client->socket==-1)
  {
    return 0;
  }

  /* send login request */
  SET_STRING_VAL(val[0],"USER",strUser);
  SET_STRING_VAL(val[1],"PASS",strPassword);
  if(!TlsAP_SendMsg(client,APCLITOSRV_CONNECT,val,2))
  {
    TlsAP_Disconnect(client);
    return 0;
  }

  /* wait for an answer */
  tmsg.size=sizeof(buffer);
  tmsg.data=buffer;
  if(TlsAP_Recv(client,&tmsg,(unsigned long)-1)<=0)
  {
    TlsAP_Disconnect(client);
    return 0;
  }
  if(!TlsAP_ParseParams(&params,NULL,tmsg.data,tmsg.size)||!params.ret)
  {
    TlsAP_Disconnect(client);
    ret=0;
  }
  else
  {
    ret=1;
  }

  return ret;
}

/*
** TlsAP_Disconnect
*/
int TlsAP_Disconnect(struct TlsAP_Client_s *client)
{
  if(!client)
  {
    return 0;
  }
  if(client->socket!=-1)
  {
    shutdown(client->socket,2);
    closesocket(client->socket);
    client->socket= -1;
  }
  return 1;
}

int TlsAP_EnumerateShows(struct TlsAP_Client_s *client)
{
  return TlsAP_SendMsg(client,APCLITOSRV_ENUMERATESHOWS,NULL,0);
}

int TlsAP_EnumerateHostsForShow(struct TlsAP_Client_s *client,const char *strShowName)
{
  cmsgVal_t val[1];

  SET_STRING_VAL(val[0],"SHOW",strShowName);
  return TlsAP_SendMsg(client,APCLITOSRV_ENUMERATEHOSTSFORSHOW,val,1);
}

int TlsAP_ChangeShow(struct TlsAP_Client_s *client,int nSession,const char *strShowName,const char *strShowPass)
{
  cmsgVal_t val[3];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_STRING_VAL(val[1],"SHOW",strShowName);
  SET_STRING_VAL(val[2],"PASS",strShowPass);
  return TlsAP_SendMsg(client,APCLITOSRV_CHANGESHOW,val,3);
}

int TlsAP_ConnectToShow(struct TlsAP_Client_s *client,int nSession,const char *strShowName,const char *strShowPassword,const char *strHostName)
{
  cmsgVal_t val[4];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_STRING_VAL(val[1],"SHOW",strShowName);
  SET_STRING_VAL(val[2],"PASS",strShowPassword);
  SET_STRING_VAL(val[3],"HOST",strHostName);
  return TlsAP_SendMsg(client,APCLITOSRV_CONNECTTOSHOW,val,4);
}

int TlsAP_EnumerateDirectors(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_ENUMERATEDIRECTORS,val,1);
}

int TlsAP_AttachToDirector(struct TlsAP_Client_s *client,int nSession,const char *strDirectorName)
{
  cmsgVal_t val[2];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_STRING_VAL(val[1],"DNAM",strDirectorName);
  return TlsAP_SendMsg(client,APCLITOSRV_ATTACHTODIRECTOR,val,2);
}

int TlsAP_AttachAsTalent(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_ATTACHASTALENT,val,1);
}

int TlsAP_PostBusyAllButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTBUSYALLBUTTON,val,1);
}

int TlsAP_PostDelayDumpButton(struct TlsAP_Client_s *client,int nSession,int bDown)
{
  cmsgVal_t val[2];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"DOWN",bDown);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTDELAYDUMPBUTTON,val,2);
}

int TlsAP_PostDialKey(struct TlsAP_Client_s *client,int nSession,int chKey)
{
  cmsgVal_t val[2];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"KEY_",chKey);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTDIALKEY,val,2);
}

int TlsAP_PostDropButton(struct TlsAP_Client_s *client,int nSession,int nColumn)
{
  cmsgVal_t val[2];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"COL_",nColumn);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTDROPBUTTON,val,2);
}

int TlsAP_PostHoldButton(struct TlsAP_Client_s *client,int nSession,int nColumn)
{
  cmsgVal_t val[2];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"COL_",nColumn);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTHOLDBUTTON,val,2);
}

int TlsAP_PostLineButton(struct TlsAP_Client_s *client,int nSession,int nLine,int nColumn)
{
  cmsgVal_t val[3];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"LINE",nLine);
  SET_DWORD_VAL(val[2],"COL_",nColumn);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTLINEBUTTON,val,3);
}

int TlsAP_PostMuteButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTMUTEBUTTON,val,1);
}

int TlsAP_PostNextButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTNEXTBUTTON,val,1);
}

int TlsAP_PostRecordButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTRECORDBUTTON,val,1);
}

int TlsAP_PostSpeakerButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTSPEAKERBUTTON,val,1);
}

int TlsAP_PostText(struct TlsAP_Client_s *client,int nSession,int nTextType,int nLine,const char *strText)
{
  cmsgVal_t val[4];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  SET_DWORD_VAL(val[1],"TTYP",nTextType);
  SET_DWORD_VAL(val[2],"LINE",nLine);
  SET_STRING_VAL(val[3],"TEXT",strText);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTTEXT,val,4);
}

int TlsAP_PostTransferButton(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTTRANSFERBUTTON,val,1);
}

int TlsAP_PostUpdateAll(struct TlsAP_Client_s *client,int nSession)
{
  cmsgVal_t val[1];

  SET_DWORD_VAL(val[0],"SSN_",nSession);
  return TlsAP_SendMsg(client,APCLITOSRV_POSTUPDATEALL,val,1);
}

int TlsAP_EnumerateShowsForHost(struct TlsAP_Client_s *client,const char *strHostName)
{
  cmsgVal_t val[2];

  SET_STRING_VAL(val[0],"HOST",strHostName);
  return TlsAP_SendMsg(client,APCLITOSRV_ENUMERATESHOWSFORHOST,val,1);
}

/* ------------------------- * network functions * ------------------------- */

/*
** Net_ParseIPaddr
**
** prepare network ip address
** host is in the form "<host>[:port]"
** <host> can be either ip, or hostname
** return value is addr on success, or NULL on fail
*/
static netIPaddr_t * Net_ParseIPaddr(netIPaddr_t *addr,const char *host,word port)
{
  const char *p;
  char buff[256];
  dword ip;
  int len;

  if(!addr)
  {
    return NULL;
  }
  memset(addr,0,sizeof(netIPaddr_t));

  /* check for [:<port>] and get host length */
  p=strchr(host,':');
  if(p)
  {
    len=(int)(p-host);
    port=(word)atoi(p+1);
  }
  else
  {
    len=(int)strlen(host);
  }

  /* check host name length */
  if(len<1||len>=(int)sizeof(buff))
  {
    return NULL;
  }

  /* copy host name */
  memcpy(buff,host,len);
  buff[len]=0;

  ip=inet_addr(buff);
  if(ip==INADDR_NONE)
  {
    /* inet_addr failed; try to resolve host name */
    const struct hostent *he;
    const struct in_addr *ia;

    he=gethostbyname(buff);
    if(!he||he->h_addrtype!=AF_INET)
    {
      return NULL;
    } /* unknown host name */
    ia=(const struct in_addr*)he->h_addr;
    ip=ia->s_addr;
  }

  memcpy(addr->addr,&ip,4);
  addr->port=port;
  return addr;
}

/*
** Net_TCPconnect
**
** creates tcp connection to remote host
** returns valid network socket on success
** -1 on error
*/
static socket_t Net_TCPconnect(netIPaddr_t *addr,int *soc)
{
  struct sockaddr_in addr_src;
  struct sockaddr_in addr_dst;
  socket_t s;

  if(!addr)
  {
    return -1;
  }

  /* local address */
  memset(&addr_src,0,sizeof(addr_src));
  addr_src.sin_family=AF_INET;
  addr_src.sin_addr.s_addr=htonl(INADDR_ANY);
  addr_src.sin_port=htons(0);

  /* remote address */
  memset(&addr_dst,0,sizeof(addr_dst));
  addr_dst.sin_family=AF_INET;
  memcpy(&addr_dst.sin_addr,addr->addr,4);
  addr_dst.sin_port=htons(addr->port);

  /* try to connect */
  if((s=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    return -1;
  }
  if(bind(s,(struct sockaddr*)&addr_src,sizeof(addr_src))==-1)
  {
    closesocket(s);
    return -1;
  }
  if(connect(s,(struct sockaddr*)&addr_dst,sizeof(addr_dst))==-1)
  {
    closesocket(s);
    return -1;
  }
  *soc=s;

  return s;
}

/*
** Net_TCPconnectEx
**
** creates tcp connection to remote host
** returns valid network socket on success
** -1 on error
*/
static socket_t Net_TCPconnectEx(const char *host,word port,int *socket)
{
  netIPaddr_t addr;
  return Net_TCPconnect(Net_ParseIPaddr(&addr,host,port),socket);
}

/*
** Net_TCPwrite
**
** writes len bytes to tcp socket
** returns 1 if ok
*/
static int Net_TCPwrite(socket_t sock,const void *data,int len)
{
  int ret;

  if(!data)
  {
    return 0;
  }
  while(len>0)
  {
    ret=send(sock,(const char *)data,len,MSG_NOSIGNAL);
    if(ret<0)
    {
      return 0;
    }

    data=(byte*)data+ret;
    len-=ret;
  }

  return 1;
}

/*
** Net_TCPread
**
** reads len bytes from tcp socket
** returns 1 if ok
*/
static int Net_TCPread(socket_t sock,void *data,int len)
{
  int ret;

  if(!data)
  {
    return 0;
  }
  while(len>0)
  {
    ret=recv(sock,(char *)data,len,MSG_NOSIGNAL|MSG_WAITALL);
    if(ret<=0)
    {
      return 0;
    }
    data=(byte*)data+ret;
    len-=ret;
  }

  return 1;
}
