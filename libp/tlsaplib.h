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
//          File: tlsaplib.h
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
#ifdef __cplusplus
extern "C"
{
#endif

  /* ------------------------- * configuration * ------------------------- */
  /* define for thread safe send */
  /* #define TLSAP_CONF_THREAD */

  /*
  Thread safety:
  Different aspects can be pointed out in respect to thread safety:
  1) Working with different client objects. In this respect tlsap library is fully thread safe,
     you can use several threads to indiviudally control several client objects.
     No synchronization is required. For calls to tlsap library.
  2) Working with the same client object from several threads.  
     Tlsap library is by default not thread safe, in this respect; you should serialize all calls to
     tlsap library. There exists compile time option TLSAP_CONF_THREAD to enable thread safe sending.
     When enabled you are able to call from multiple threads all functions from 3a) section (see below),
     except TlsAP_Connect/TlsAP_Disonnect. All other functions are thread sensitive anyway.
  3) Calling functions during the call to TlsAP_WaitEvents:
     it is safe to call: all functions from 3a) section except TlsAP_Connect, during wait for events.
     Also it is safe to call TlsAP_Disonnect; in this case TlsAP_WaitEvents will return with a value of -1 (connection closed).


  Operation Sequence:

  1) Initialization:
  * Define callback functions in your code
  * Define TlsAP_Callbacks variablem
   TlsAP_Callbacks cb;
  * fill it with your callback function pointers
   memset(&cb, 0, sizeof(cb));
   cb.EnumerateShowsCallback=EnumerateShowsCallback;
   cb.EnumerateHostsCallback=EnumerateHostsCallback;
  <...>
  * define your own data for AP session (if required), it will be passed to callback functions
   YourDataStrcuture userData;
  * call TlsAP_Create to create AP client object
    TlsAP_Client hClient=TlsAP_Create(&userData, &cb);
  2) Connection:
    call:
    res=TlsAP_Connect(hClient, "host[:port]", "username", "pass");
    res: 0 on error
       : 1 on success
  3a) You can can call any of the following functions to send data to server:
    int TlsAP_Connect(TlsAP_Client client, const char *strHostPort, const char *strUser, const char *strPassword);
    int TlsAP_Disconnect(TlsAP_Client client);
    int TlsAP_EnumerateShows(TlsAP_Client client);
    int TlsAP_EnumerateHostsForShow(TlsAP_Client client, const char *strShowName);
    int TlsAP_ChangeShow(TlsAP_Client client, int nSession, const char *strShowName, const char *strShowPass);
    int TlsAP_ConnectToShow(TlsAP_Client client, int nSession, const char *strShowName, const char *strShowPassword, const char *strHostName);
    int TlsAP_EnumerateDirectors(TlsAP_Client client, int nSession);
    int TlsAP_AttachToDirector(TlsAP_Client client, int nSession, const char *strDirectorName);
    int TlsAP_AttachAsTalent(TlsAP_Client client, int nSession);
    int TlsAP_PostBusyAllButton(TlsAP_Client client, int nSession);
    int TlsAP_PostDelayDumpButton(TlsAP_Client client, int nSession, int bDown);
    int TlsAP_PostDialKey(TlsAP_Client client, int nSession, int chKey);
    int TlsAP_PostDropButton(TlsAP_Client client, int nSession, int nColumn);
    int TlsAP_PostHoldButton(TlsAP_Client client, int nSession, int nColumn);
    int TlsAP_PostLineButton(TlsAP_Client client, int nSession, int nLine, int nColumn);
    int TlsAP_PostMuteButton(TlsAP_Client client, int nSession);
    int TlsAP_PostNextButton(TlsAP_Client client, int nSession);
    int TlsAP_PostRecordButton(TlsAP_Client client, int nSession);
    int TlsAP_PostSpeakerButton(TlsAP_Client client, int nSession);
    int TlsAP_PostText(TlsAP_Client client, int nSession, int nTextType, int nLine, const char *strText);
    int TlsAP_PostTransferButton(TlsAP_Client client, int nSession);
    int TlsAP_PostUpdateAll(TlsAP_Client client, int nSession);
    int TlsAP_EnumerateShowsForHost(TlsAP_Client client, const char *strHostName);

    return value: 0 on error; you should reconnect to server to continue operation
                : 1 on success
  3b) You can disconnect from the server:
    TlsAP_Disconnect(hClient);

  3c) You can wait for events from the server:
    res=TlsAP_WaitEvents(hClient, timeout);
    timeout is time to wait in milliseconds; use -1 for infinite wait

    return value: <0 on error; you should reconnect to server to continue operation
                :  0 on timeout
                : >0 on success

    TlsAP_WaitEvents functions triggers your callback functions, supplied on call to TlsAP_Create.
    Here is a full list of callback functions:

    void EnumerateShowsCallback(void *userData, const char *strShowName, int bRequiresPass, int bIsActive, const char *strHostName, int bIsLast);
    void EnumerateHostsCallback(void *userData, const char *strHostName, int bIsLast);
    void EnumerateDirectorsCallback(void *userData, int nSession, const char *strDirectorName, int bIsTalent, int bIsFree, int bIsLast);
    void SendShowInfo(void *userData, int nSession, int nMaxLines, int nHybrids);
    void SendMode(void *userData, int nSession, int bTalent);
    void SendShowChange(void *userData, int nSession, const char *strNewShowName, const char *strOldShowName);
    void SendRecord(void *userData, int nSession, int bOn);
    void SendBusyAll(void *userData, int nSession, int bOn);
    void SendTransfer(void *userData, int nSession, int bOn);
    void SendMute(void *userData, int nSession, int bOn);
    void SendRingMute(void *userData, int nSession, int bOn);
    void SendDelayDump(void *userData, int nSession, int bOn);
    void SendHandset(void *userData, int nSession, int bOn);
    void SendSpeaker(void *userData, int nSession, int bOn);
    void SendLineState(void *userData, int nSession, int nLine, int nColumn, int nLineState, long lElapsedTimeSec);
    void SendSeizeLine(void *userData, int nSession, int nLine, int nColumn);
    void SendRingLine(void *userData, int nSession, const char *strCallerId, int nLine);
    void SendDropLine(void *userData, int nSession, const char *strCallerId, int nLine);
    void SendAnswerLine(void *userData, int nSession, const char *strCallerId, int nLine, int nColumn);
    void SendSelectLine(void *userData, int nSession, const char *strCallerId, int nLine, int nColumn);
    void SendDialKey(void *userData, int nSession, int chKey);
    void SendText(void *userData, int nSession, int nTextType, const char *strText, int nLine);
    void SendError(void *userData, int nSession, long nErrorNumber, const char *strErrorText);
    void EnumerateShowsForHostCallback(void *userData, const char *strShowName, int bIsLast);

  4) Destroy client object when it is no longer needed:
    TlsAP_Destroy(hClient);

  for more detailed operation descriptions see: http://www.telos-systems.com/support/TLSAPX2a-API.pdf
  */

  /* ------------------------- * types * ------------------------- */
  struct TlsAP_Client_s;
  typedef struct TlsAP_Client_s *TlsAP_Client;

  typedef struct TlsAP_Callbacks_s
  {
    void (*EnumerateShowsCallback)(void *,const char *,int,int,const char *,int);
    void (*EnumerateHostsCallback)(void *,const char *,int);
    void (*EnumerateDirectorsCallback)(void *,int,const char *,int,int,int);
    void (*SendShowInfo)(void *,int,int,int);
    void (*SendMode)(void *,int,int);
    void (*SendShowChange)(void *,int,const char *,const char *);
    void (*SendRecord)(void *,int,int);
    void (*SendBusyAll)(void *,int,int);
    void (*SendTransfer)(void *,int,int);
    void (*SendMute)(void *,int,int);
    void (*SendRingMute)(void *,int,int);
    void (*SendDelayDump)(void *,int,int);
    void (*SendHandset)(void *,int,int);
    void (*SendSpeaker)(void *,int,int);
    void (*SendLineState)(void *,int,int,int,int,long);
    void (*SendSeizeLine)(void *,int,int,int);
    void (*SendRingLine)(void *,int,const char *,int);
    void (*SendDropLine)(void *,int,const char *,int);
    void (*SendAnswerLine)(void *,int,const char *,int,int);
    void (*SendSelectLine)(void *,int,const char *,int,int);
    void (*SendDialKey)(void *,int,int);
    void (*SendText)(void *,int,int,const char *,int);
    void (*SendError)(void *,int,long,const char *);
    void (*EnumerateShowsForHostCallback)(void *,const char *,int);
  } TlsAP_Callbacks;

  /* ------------------------- * functions * ------------------------- */
  TlsAP_Client TlsAP_Create(void *userData,TlsAP_Callbacks *eventHandlers);
  int TlsAP_Destroy(TlsAP_Client client);
  int TlsAP_WaitEvents(TlsAP_Client client,unsigned long timeout);

  int TlsAP_Connect(TlsAP_Client client,const char *strHostPort,const char *strUser,const char *strPassword,unsigned short tcpPort,int *socket);
  int TlsAP_Disconnect(TlsAP_Client client);
  int TlsAP_EnumerateShows(TlsAP_Client client);
  int TlsAP_EnumerateHostsForShow(TlsAP_Client client,const char *strShowName);
  int TlsAP_ChangeShow(TlsAP_Client client,int nSession,const char *strShowName,const char *strShowPass);
  int TlsAP_ConnectToShow(TlsAP_Client client,int nSession,const char *strShowName,const char *strShowPassword,const char *strHostName);
  int TlsAP_EnumerateDirectors(TlsAP_Client client,int nSession);
  int TlsAP_AttachToDirector(TlsAP_Client client,int nSession,const char *strDirectorName);
  int TlsAP_AttachAsTalent(TlsAP_Client client,int nSession);
  int TlsAP_PostBusyAllButton(TlsAP_Client client,int nSession);
  int TlsAP_PostDelayDumpButton(TlsAP_Client client,int nSession,int bDown);
  int TlsAP_PostDialKey(TlsAP_Client client,int nSession,int chKey);
  int TlsAP_PostDropButton(TlsAP_Client client,int nSession,int nColumn);
  int TlsAP_PostHoldButton(TlsAP_Client client,int nSession,int nColumn);
  int TlsAP_PostLineButton(TlsAP_Client client,int nSession,int nLine,int nColumn);
  int TlsAP_PostMuteButton(TlsAP_Client client,int nSession);
  int TlsAP_PostNextButton(TlsAP_Client client,int nSession);
  int TlsAP_PostRecordButton(TlsAP_Client client,int nSession);
  int TlsAP_PostSpeakerButton(TlsAP_Client client,int nSession);
  int TlsAP_PostText(TlsAP_Client client,int nSession,int nTextType,int nLine,const char *strText);
  int TlsAP_PostTransferButton(TlsAP_Client client,int nSession);
  int TlsAP_PostUpdateAll(TlsAP_Client client,int nSession);
  int TlsAP_EnumerateShowsForHost(TlsAP_Client client,const char *strHostName);

  /* ------------------------- * line states * ------------------------- */
#define LINE_STATE_INVALID 0x0000
#define LINE_STATE_RESERVED 0x0100
#define LINE_STATE_IDLE 0x0200
#define LINE_STATE_RINGING 0x0301
#define LINE_STATE_HOLD 0x0402
#define LINE_STATE_READY_HOLD 0x0504
#define LINE_STATE_READY_NEXT 0x0604
#define LINE_STATE_TALENT_HOLD 0x0702
#define LINE_STATE_ON_AIR 0x0808
#define LINE_STATE_ON_AIR_LOCKED 0x0908
#define LINE_STATE_HANDSET 0x0A10
#define LINE_STATE_HANDSET_LOCKED 0x0B10
#define LINE_STATE_SPEAKER 0x0C10
#define LINE_STATE_SPEAKER_LOCKED 0x0D10
#define LINE_STATE_USED_ELSEWHERE 0x0E20
#define LINE_STATE_BUSY_ALL 0x0F20
#define LINE_STATE_SHARED_UNAVAILABLE 0x1020

  /* ------------------------ * default values * ------------------------ */
#define TELOS2101_DEFAULT_PORT 9998

#ifdef __cplusplus
}
#endif
