// pbx_callcommander.c
//
// Virtual broadcast telephone interface for Asterisk
//
//   (C) Copyright 2008 Fred Gleason <fredg@paravelsystems.com>
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

#define AST_MODULE "pbx"

#include <asterisk/module.h>
#include <asterisk/io.h>
#include <asterisk/sched.h>
#include <asterisk/linkedlists.h>
#include <asterisk/app.h>
#include <asterisk/pbx.h>
#include <asterisk/lock.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <signal.h>

#include <ml.h>

#define PBX_CC_MAX_ARGS 10
#define PBX_CC_MAX_LENGTH 256
#define PBX_CC_MAX_LINES 12
#define PBX_CC_MAX_BANKS 4
#define PBX_CC_MAX_CONSOLES 4

/*
 * CCVP Line States
 */
#define PBX_CC_LINE_INACTIVE 0
#define PBX_CC_LINE_IDLE 1
#define PBX_CC_LINE_ELSEWHERE 2
#define PBX_CC_LINE_HANDSET 3
#define PBX_CC_LINE_SPEAKER 4
#define PBX_CC_LINE_HOLD 5
#define PBX_CC_LINE_SCREENED 6
#define PBX_CC_LINE_ONAIR 7
#define PBX_CC_LINE_RINGING 8
#define PBX_CC_LINE_LOCKED 9
#define PBX_CC_LINE_BUSIED 10
#define PBX_CC_LINE_NEXT 11

/*
 * Application Data
 */
static char *cc_app_name="CallCommander";
static char *cc_app_synopsis="Connect CallCommander Ports";
static char *cc_app_descrip="  CallCommander(port-type,[port-num]):  Connect to a CallCommander port.\n"
"Valid port types are:\n"
"\n"
"     Trunk - Connections to the PSTN [1 - 12]\n"
"     Hybrid - Connections to audio hybrids [1 - 4]\n"
"     Screener - Connections to call screener stations [1 - 4]\n"
"\n"
"A port-num may optionally be specified as well.  If not supplied, the next\n"
"available port of the specified type will be used.";

/*
 * Mutexes
 */
ast_mutex_t cc_mutex;

/*
 * Module Variables
 */
static struct sched_context *cc_sched;
static struct io_context *cc_ioc;
static pthread_t cc_thread_id=AST_PTHREADT_NULL;
static int *cc_ioc_id;
static int cc_sock=-1;
static int cc_shutdown=0;
struct conn {
  AST_LIST_ENTRY(conn) list;
  int fd;
  int *ioc_id;
  int istate;
  char accum[PBX_CC_MAX_LENGTH];
  int ptr;
  int arg;
  uint16_t token;
  char args[PBX_CC_MAX_ARGS][PBX_CC_MAX_LENGTH];
};
AST_LIST_HEAD_NOLOCK(conns,conn) cc_connections;

static int cc_line_state[PBX_CC_MAX_LINES]={PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE,
					    PBX_CC_LINE_IDLE};
static int cc_line_bank[PBX_CC_MAX_LINES]=
{255,255,255,255,255,255,255,255,255,255,255,255};
static int cc_line_console[PBX_CC_MAX_LINES]=
{255,255,255,255,255,255,255,255,255,255,255,255};

static int string_to_int(const char *str,int *ok)
{
  int res;

  if(sscanf(str,"%d",&res)==1) {
    if(ok!=NULL) {
      *ok=1;
    }
  }
  else {
    if(ok!=NULL) {
      *ok=0;
    }
  }
  return res;
}


static uint16_t tokenize(const char *verb)
{
  return 256*(0xff&verb[0])+(0xff&verb[1]);
}


static void *mld_thread(void *ignore)
{
  int res;

  while(cc_shutdown!=1) {
    res=ast_sched_wait(cc_sched);
    if((res>1000)||(res<0)) {
      res = 1000;
    }
    ast_io_wait(cc_ioc,res);
  }

  return NULL;
}


void echo_command(struct conn *c,const char *str)
{
  // ast_log(LOG_NOTICE,"echo_command(%s)\n",str);
  write(c->fd,str,strlen(str));
}


void send_command(const char *str)
{
  struct conn *conn;

  AST_LIST_TRAVERSE_SAFE_BEGIN(&cc_connections,conn,list) {
    echo_command(conn,str);
  }
  AST_LIST_TRAVERSE_SAFE_END;
}


void dispatch_command(struct conn *c)
{
  int i;
  char str[1024];
  int ok;
  int line;
  int bank;
  int console;

/*
  ast_log(LOG_NOTICE,"Token: 0x%04X\n",(unsigned)c->token);
  for(i=0;i<c->arg;i++) {
    ast_log(LOG_NOTICE,"  Arg%d: %s\n",i,c->args[i]);
  }
*/

  switch(c->token) {
    case 0x434C:   // CL -- Request Console List
      for(i=0;i<PBX_CC_MAX_BANKS;i++) {
	sprintf(str,"CL %d 0 1 Screener %d!",i,i+1);
	echo_command(c,str);
      }
      echo_command(c,"CL 8 1 1 On Air Control!");
      echo_command(c,"CL!");
      break;

    case 0x4C43:   // LC -- Connect to Console
      if(c->arg<2) {
	return;
      }
      console=string_to_int(c->args[0],&ok);
      if((!ok)||(console<0)) {
	return;
      }
      if((console>PBX_CC_MAX_CONSOLES)&&(console!=8)) {
	sprintf(str,"LC %d 0 -!",console);
	echo_command(c,str);
	return;
      }
      if(console==8) {
	sprintf(str,"LC 1 %d  +!",console);
      }
      else {
	sprintf(str,"LC 0 %d  +!",console);
      }
      echo_command(c,str);
      break;

    case 0x5353:   // SS -- Request Line State 
      ast_mutex_lock(&cc_mutex);
      for(i=0;i<PBX_CC_MAX_LINES;i++) {
	sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],cc_line_bank[i]);
	echo_command(c,str);
      }
      ast_mutex_unlock(&cc_mutex);
      break;

    case 0x4C41:   // LA -- Select Line 
      if(c->arg!=3) {
	return;
      }
      line=string_to_int(c->args[0],&ok);
      if((!ok)||(line<0)||(line>PBX_CC_MAX_LINES)) {
	return;
      }
      bank=string_to_int(c->args[1],&ok);
      if((!ok)||(bank<0)||(bank>PBX_CC_MAX_BANKS)) {
	return;
      }
      console=string_to_int(c->args[2],&ok);
      if((!ok)||(console<0)||((console>PBX_CC_MAX_CONSOLES)&&(console!=8))) {
	return;
      }
      ast_mutex_lock(&cc_mutex);
      if(console==8) {   // Talent Mode
	switch(cc_line_state[line]) {
	  case PBX_CC_LINE_IDLE:
	  case PBX_CC_LINE_HOLD:
	  case PBX_CC_LINE_SCREENED:
	  case PBX_CC_LINE_ONAIR:
	  case PBX_CC_LINE_RINGING:
	    if(cc_line_bank[line]==bank) {
	      cc_line_state[line]=PBX_CC_LINE_LOCKED;
	    }
	    else {
	      cc_line_state[line]=PBX_CC_LINE_ONAIR;
	      cc_line_bank[line]=bank;
	      cc_line_console[line]=255;
	    }
	    sprintf(str,"SS %d %d %d %d!",line,cc_line_state[line],
		    cc_line_bank[line],console);
	    send_command(str);
	    break;
	    
	  case PBX_CC_LINE_LOCKED:
	    if(cc_line_state[line]==PBX_CC_LINE_LOCKED) {
	      cc_line_state[line]=PBX_CC_LINE_ONAIR;
	      sprintf(str,"SS %d %d %d %d!",line,cc_line_state[line],
		      cc_line_bank[line],console);
	      send_command(str);
	    }
	    ast_mutex_unlock(&cc_mutex);
	    return;
	}
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  if(i!=line) {
	    if((cc_line_state[i]==PBX_CC_LINE_ONAIR)&&\
	       (cc_line_bank[i]==bank)) {
	      cc_line_state[i]=PBX_CC_LINE_IDLE;
	      cc_line_bank[i]=255;
	      cc_line_console[i]=255;
	      sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
		      cc_line_bank[line]);
	      send_command(str);
	    }
	  }
	}
      }
      else {   // Producer Mode
	switch(cc_line_state[line]) {
	  case PBX_CC_LINE_IDLE:
	  case PBX_CC_LINE_HOLD:
	  case PBX_CC_LINE_SCREENED:
	  case PBX_CC_LINE_RINGING:
	    cc_line_state[line]=PBX_CC_LINE_HANDSET;
	    cc_line_bank[line]=255;
	    cc_line_console[line]=console;
	    sprintf(str,"SS %d %d %d %d!",line,cc_line_state[line],
		    cc_line_bank[line],console);
	    send_command(str);
	    break;
	}
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  if(i!=line) {
	    if((cc_line_state[i]==PBX_CC_LINE_HANDSET)||
	       (cc_line_state[i]==PBX_CC_LINE_SPEAKER)) {
	      cc_line_state[i]=PBX_CC_LINE_IDLE;
	      cc_line_bank[i]=255;
	      cc_line_console[i]=255;
	      sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
		      cc_line_bank[line]);
	      send_command(str);
	    }
	  }
	}
      }
      ast_mutex_unlock(&cc_mutex);
      break;

    case 0x4844:   // HD -- Hold 
      if(c->arg!=2) {
	return;
      }
      bank=string_to_int(c->args[0],&ok);
      if((!ok)||(bank<0)||(bank>PBX_CC_MAX_BANKS)) {
	return;
      }
      console=string_to_int(c->args[1],&ok);
      if((!ok)||(console<0)||((console>PBX_CC_MAX_CONSOLES)&&(console!=8))) {
	return;
      }
      ast_mutex_lock(&cc_mutex);
      if(console==8) { // Talent Mode
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  switch(cc_line_state[i]) {
	    case PBX_CC_LINE_ONAIR:
	      if(cc_line_bank[i]==bank) {
		cc_line_state[i]=PBX_CC_LINE_HOLD;
		cc_line_bank[i]=255;
		cc_line_console[i]=255;
		sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
			cc_line_bank[i]);
		send_command(str);
	      }
	      break;
	      
	  }
	}
      }
      else {  // Producer Mode
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  switch(cc_line_state[i]) {
	    case PBX_CC_LINE_HANDSET:
	    case PBX_CC_LINE_SPEAKER:
	      if(cc_line_console[i]==console) {
		if(bank==0) {
		  cc_line_state[i]=PBX_CC_LINE_HOLD;
		}
		else {
		  cc_line_state[i]=PBX_CC_LINE_SCREENED;
		}
		cc_line_bank[i]=255;
		cc_line_console[i]=255;
		sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
			cc_line_bank[i]);
		send_command(str);
	      }
	      break;
	  }
	}
      }
      ast_mutex_unlock(&cc_mutex);
      break;

    case 0x4450:   // DP -- Drop 
      if(c->arg!=2) {
	return;
      }
      bank=string_to_int(c->args[0],&ok);
      if((!ok)||(bank<0)||(bank>PBX_CC_MAX_BANKS)) {
	return;
      }
      console=string_to_int(c->args[1],&ok);
      if((!ok)||(console<0)||((console>PBX_CC_MAX_CONSOLES)&&(console!=8))) {
	return;
      }
      ast_mutex_lock(&cc_mutex);
      if(console==8) { // Talent Mode
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  switch(cc_line_state[i]) {
	    case PBX_CC_LINE_ONAIR:
	      if(cc_line_bank[i]==bank) {
		cc_line_state[i]=PBX_CC_LINE_IDLE;
		cc_line_bank[i]=255;
		cc_line_console[i]=255;
		sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
			cc_line_bank[i]);
		send_command(str);
	      }
	      break;
	  }
	}
      }
      else {   // Producer Mode
	for(i=0;i<PBX_CC_MAX_LINES;i++) {
	  switch(cc_line_state[i]) {
	    case PBX_CC_LINE_HANDSET:
	    case PBX_CC_LINE_SPEAKER:
	      if(cc_line_console[i]==console) {
		cc_line_state[i]=PBX_CC_LINE_IDLE;
		cc_line_bank[i]=255;
		cc_line_console[i]=255;
		sprintf(str,"SS %d %d %d 255!",i,cc_line_state[i],
			cc_line_bank[i]);
		send_command(str);
	      }
	      break;
	  }
	}
      }
      ast_mutex_unlock(&cc_mutex);
      break;

    case 0x4241:   // BA -- Busy All 
      break;

    case 0x4253:   // BS -- Request Busy All State 
      break;

    case 0x4444:   // DD -- Delay Dump 
      break;

    case 0x4453:   // DS -- Request Delay Dump State 
      break;

    case 0x5244:   // RD -- Toggle Record 
      break;

    case 0x5253:   // RS -- Request Record State 
      break;

    case 0x544F:   // TO -- Send DTMF Digit 
      break;
  }
}


static void process_connection(struct conn *c)
{
  char buf[1024];
  int n;
  int i;
  struct conn *conn;

  if((n=recvfrom(c->fd,buf,1024,MSG_DONTWAIT,NULL,NULL))<=0) {  // Closed
    ast_io_remove(cc_ioc,c->ioc_id);
    AST_LIST_TRAVERSE_SAFE_BEGIN(&cc_connections,conn,list) {
      if(conn->fd==c->fd) {
	AST_LIST_REMOVE_CURRENT(&cc_connections,list);
      }
    }
    AST_LIST_TRAVERSE_SAFE_END;
    close(c->fd);
    free(c);
    return;
  }
  for(i=0;i<n;i++) {
    switch(c->istate) {
      case 0:   // First byte of verb
	c->accum[0]=buf[i];
	c->istate=1;
	break;
	
      case 1:   // Second byte of verb
	c->accum[1]=buf[i];
	c->istate=2;
	break;
	
      case 2:  // Verb delimiter 
	if((buf[i]==' ')||(buf[i]=='!')) {
	  c->token=tokenize(c->accum);
	  if(buf[i]=='!') {
	    dispatch_command(c);
	    c->istate=0;
	  }
	  else {
	    c->istate=3;
	  }
	}
	else {
	  c->istate=0;
	}
	break;
	
      case 3:  // Argument
	if((buf[i]!=' ')&&(buf[i]!='!')) {
	  c->args[c->arg][c->ptr++]=buf[i];
	}
	else {
	  c->args[c->arg++][c->ptr]=0;
	  c->ptr=0;
	}
	if(buf[i]=='!') {
	  dispatch_command(c);
	  c->arg=0;
	  c->istate=0;
	}
	break;
    }
  }
}


static int read_socket(int *id, int fd, short events, void *cbdata)
{
  int new_fd;
  struct conn *conn;

  if(fd==cc_sock) {  // New connection
    if((new_fd=accept(cc_sock,NULL,NULL))<0) {
      return 1;
    }
    if((conn=ast_calloc(1,sizeof(*conn)))<0) {
      close(new_fd);
      return 1;
    }
    conn->fd=new_fd;
    if((conn->ioc_id=ast_io_add(cc_ioc,new_fd,read_socket,AST_IO_IN,NULL))==
       NULL) {
      close(new_fd);
      return 1;
    }
    AST_LIST_INSERT_HEAD(&cc_connections,conn,list);
    return 1;
  }

  //
  // Handle Existing Connections
  //
  AST_LIST_TRAVERSE_SAFE_BEGIN(&cc_connections,conn,list) {
    if(conn->fd==fd) {
      process_connection(conn);
    }
  }
  AST_LIST_TRAVERSE_SAFE_END;

  return 1;
}


static int exec_trunk(struct ast_channel *chan,int port_num)
{
  int i;
  char str[1024];
  struct ast_channel *dchan;
  int reason;
  struct ast_bridge_config bridge_config;
  struct ast_frame *bridge_frame;
  struct ast_channel *bridge_channel;

  if(port_num>=PBX_CC_MAX_LINES) {
    ast_log(LOG_WARNING,
	    "CallCommander Trunk port numbers must be between 1 and %d\n",
	    PBX_CC_MAX_LINES);
    return -1;
  }

  ast_mutex_lock(&cc_mutex);
  if(port_num<0) {  // Auto assign a trunk
    for(i=0;i<PBX_CC_MAX_LINES;i++) {
      if(cc_line_state[i]==PBX_CC_LINE_IDLE) {
	port_num=i;
	i=PBX_CC_MAX_LINES;
      }
    }
  }

  //
  // FIXME:  These should return busy!
  //
  if(port_num<0) {  
    ast_mutex_unlock(&cc_mutex);
    return -1;
  }
  if(cc_line_state[port_num]!=PBX_CC_LINE_IDLE) {
    ast_mutex_unlock(&cc_mutex);
    return -1;
  }

  //
  // Update CCVP State
  //
  cc_line_state[port_num]=PBX_CC_LINE_RINGING;
  sprintf(str,"SS %d %d 255 255!",port_num,cc_line_state[port_num]);
  send_command(str);
  ast_mutex_unlock(&cc_mutex);

  //
  // Wait for channel state to change
  //
  while(ast_safe_sleep(chan,100)==0) {
    ast_mutex_lock(&cc_mutex);
    switch(cc_line_state[port_num]) {
      case PBX_CC_LINE_HANDSET:
      case PBX_CC_LINE_SPEAKER:
	dchan=ast_request_and_dial("Zap",chan->nativeformats,"1",
				   10000,&reason,"","");
	if(dchan!=NULL) {
	  ast_answer(chan);
	  ast_channel_bridge(chan,dchan,&bridge_config,&bridge_frame,
			     &bridge_channel);
	  cc_line_state[port_num]=PBX_CC_LINE_HANDSET;
	  cc_line_bank[port_num]=255;
	  cc_line_console[port_num]=0;
	  sprintf(str,"SS %d %d %d %d!",port_num,cc_line_state[port_num],
		  cc_line_bank[port_num],cc_line_console[port_num]);
	  send_command(str);
	}
	break;

      case PBX_CC_LINE_ONAIR:
      case PBX_CC_LINE_LOCKED:
	break;

      case PBX_CC_LINE_HOLD:
      case PBX_CC_LINE_SCREENED:
	break;
    }
    ast_mutex_unlock(&cc_mutex);
  }
  ast_mutex_lock(&cc_mutex);
  cc_line_state[port_num]=PBX_CC_LINE_IDLE;
  sprintf(str,"SS %d %d 255 255!",port_num,cc_line_state[port_num]);
  send_command(str);
  ast_mutex_unlock(&cc_mutex);

  return -1;
}


static int exec_hybrid(struct ast_channel *chan,int port_num)
{
  if((port_num)<0||(port_num>=PBX_CC_MAX_BANKS)) {
    ast_log(LOG_WARNING,
	    "CallCommander Hybrid port numbers must be between 1 and %d\n",
	    PBX_CC_MAX_BANKS);
    return -1;
  }
  return -1;
}


static int exec_screener(struct ast_channel *chan,int port_num)
{
  if((port_num)<0||(port_num>=PBX_CC_MAX_CONSOLES)) {
    ast_log(LOG_WARNING,
	    "CallCommander screener port numbers must be between 1 and %d\n",
	    PBX_CC_MAX_CONSOLES);
    return -1;
  }
  return -1;
}


static int cc_app_exec(struct ast_channel *chan, void *data)
{
  int port_num=-1;
  int ok;
  char *tmp;

  AST_DECLARE_APP_ARGS(args,
		       AST_APP_ARG(port_type);
		       AST_APP_ARG(port_num);
    );
  if(ast_strlen_zero(data)) {
    ast_log(LOG_WARNING, "CallCommander requires an argument (port-type)\n");
    return -1;
  }
  tmp = ast_strdupa(data);
  AST_STANDARD_APP_ARGS(args,tmp);
  if(args.port_num) {
    port_num=string_to_int(args.port_num,&ok)-1;
    if(!ok) {
      ast_log(LOG_WARNING,"CallCommander port-num must be numeric.\n");
      return -1;
    }
  }
  if(!strcasecmp(args.port_type,"trunk")) {
    return exec_trunk(chan,port_num);
  }
  if(!strcasecmp(args.port_type,"hybrid")) {
    return exec_hybrid(chan,port_num);
  }
  if(!strcasecmp(args.port_type,"screener")) {
    return exec_screener(chan,port_num);
  }

  ast_log(LOG_WARNING,"CallCommander port-type must be \"Trunk\", \"Hybrid\" or \"Screener\".\n");  

  return -1;
}


static int load_module(void)
{
  struct sockaddr_un sa;
  mode_t mode;

  /*
   * Register Applications
   */
  if(ast_register_application(cc_app_name,cc_app_exec,cc_app_synopsis,
			      cc_app_descrip)<0) {
    return -1;
  }

  /*
   * Create Remote Control Socket Connection
   */
  cc_ioc=io_context_create();
  if(cc_ioc==NULL) {
    ast_log(LOG_ERROR,"out of memory");
    ast_unregister_application(cc_app_name);
    return -1;
  }
  cc_sched=sched_context_create();
  if(cc_sched==NULL) {
    io_context_destroy(cc_ioc);
    ast_log(LOG_ERROR,"out of memory");
    ast_unregister_application(cc_app_name);
    return -1;
  }
  unlink(ML_ASTERISK_SOCKET_NAME);
  if((cc_sock=socket(PF_UNIX,SOCK_SEQPACKET,0))<0) {
    printf("socket() error\n");
    ast_unregister_application(cc_app_name);
    return -1;
  }
  sa.sun_family=AF_UNIX;
  strcpy(sa.sun_path,ML_ASTERISK_SOCKET_NAME);
  mode=umask(~(S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH));
  if(bind(cc_sock,&sa,sizeof(struct sockaddr_un))<0) {
    umask(mode);
    ast_log(LOG_WARNING,"bind() error");
    close(cc_sock);
    cc_sock=-1;
    ast_unregister_application(cc_app_name);
    return -1;
  }
  umask(mode);
  if(listen(cc_sock,128)<0) {
    ast_log(LOG_WARNING,"listen() error");
    close(cc_sock);
    cc_sock=-1;
    ast_unregister_application(cc_app_name);
    return -1;
  }
  if((cc_ioc_id=ast_io_add(cc_ioc,cc_sock,read_socket,AST_IO_IN,NULL))==NULL) {
    ast_log(LOG_WARNING,"ast_io_add() error");
  }
  ast_pthread_create_background(&cc_thread_id,NULL,mld_thread,NULL);

  ast_mutex_init(&cc_mutex);

  return 0;
}


static int unload_module(void)
{
  /*
   * Close mld(8) socket
   */
  cc_shutdown=1;
  if(cc_thread_id!=AST_PTHREADT_NULL) {
    pthread_kill(cc_thread_id,SIGURG);
    pthread_join(cc_thread_id,NULL);
  }
  close(cc_sock);
  unlink(ML_ASTERISK_SOCKET_NAME);
  cc_sock=-1;
  io_context_destroy(cc_ioc);
  sched_context_destroy(cc_sched);

  /*
   * Unregister Applications
   */
  ast_unregister_application(cc_app_name);

  return 0;
}


static int reload_module(void)
{
  return 0;
}


AST_MODULE_INFO(ASTERISK_GPL_KEY, AST_MODFLAG_DEFAULT, 
		"CallCommander Broadcast Telephone Interface",
		.load = load_module,
		.unload = unload_module,
		.reload = reload_module,
  );
