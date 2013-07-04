#define INCL_BASE
#define INCL_DOSNMPIPES

#include <os2.h>

#include "base.h"

#include "tvshow2.h"
#include "tvshow2.hpp"
#include "logger.h"
#include "tvfuncs.h"
#include "bttuner.h"
#include "btfuncs.h"
#include "pipehndl.hpp"
#include <istring.hpp>

extern TVShowWindow* pmainWindow;

extern "C" void _Optlink PipeThread(void * arg)
{

  CHAR     message[256]  = "";                     /* Input/Output buffer */
  CHAR     PipeName[256]  = "\\pipe\\tvshow";
  HPIPE   PipeHandle;
  ULONG    ulBytes;
  CHAR     messformat[256] = "%s %s %80c";
  CHAR     numbformat[256] = "%d";
  CHAR     statusreplyform[256] = "STATUS: %d,%d,%d,%d";
  CHAR     statusreply[256] = "";
  UCHAR      tempnum;
  CHAR     token1[256];
  CHAR     token2[256];
  CHAR     token3[256];
  IString  reply;
  APIRET rc;

    rc = DosCreateNPipe(PipeName,                /* Name of pipe to create */
                        &PipeHandle,             /* Handle returned for pipe */
                        NP_ACCESS_DUPLEX,        /* Duplex pipe */
                        NP_WAIT |
                        NP_TYPE_BYTE |
                        NP_READMODE_BYTE |
                        0x01,                    /* Unique instance of pipe */
                        sizeof(message),         /* Output buffer size */
                        sizeof(message),         /* Input buffer size */
                        0L);                     /* Use default time-out */
    if (rc != NO_ERROR) {
        printf("DosCreateNPipe error: return code = %u\n",rc);
        return;
    }

    int flag=TRUE;
    int readflag=TRUE;
    printf("pipe handler thread started\n");
    rc = DosConnectNPipe(PipeHandle);
    while(flag)
    {
        int i;
        reply="ERROR\0x00";
        do
        {
                rc = DosRead(PipeHandle,message, sizeof(message), &ulBytes);
                readflag=FALSE;
                if (rc != NO_ERROR) { printf("DosRead error: return code = %u\n",rc);   }
                if (ulBytes==0)
                {
                        rc = DosDisConnectNPipe(PipeHandle);
                        if (rc != NO_ERROR) {printf("DosDisconnectNPipe error: return code = %u\n",rc);}
                        rc = DosConnectNPipe(PipeHandle);
                        if (rc != NO_ERROR) {printf("DosConnectNPipe error: return code = %u\n",rc);}
                        readflag=TRUE;
                }
        }
        while (readflag);
        memset(token1,0,255);
        memset(token2,0,255);
        memset(token3,0,255);
        sscanf(message,messformat,token1,token2,token3);
        dprintf(LOG_DEBUG,"token1:'%s', token2:'%s', token3:'%s' \n",token1,token2,token3);
        if (!strncmp(token1,"QUIT",4))
         {
            flag=FALSE;
            reply="OK";
         }
        if (!strncmp(token1,"CHANNEL",7))
         {
            sscanf(token2,numbformat,&tempnum);
            if (pmainWindow->setChannel(tempnum)==NO_ERROR)
                 {reply="OK";}
            else {reply="ERROR";}
         }
        if (!strncmp(token1,"NEXTCHANNEL",11))
         {
            pmainWindow->nextChannel();
            reply="OK";
         }
        if (!strncmp(token1,"PREVCHANNEL",11))
         {
            pmainWindow->prevChannel();
            reply="OK";
         }
        if (!strncmp(token1,"INPUT",5))
         {
            sscanf(token2,numbformat,&tempnum);
            if ((tempnum>=0)||(tempnum<=3))
            {
              BTSelectInput(pbttv,tempnum);
              reply="OK";
            }
         }
        if (!strncmp(token1,"SYSTEM",6))
         {
            reply="OK";
         }
      if (!strncmp(token1,"BRIGHTNESS",10))
         {
            sscanf(token2,numbformat,&tempnum);
            if (pmainWindow->setBrightness(tempnum)==NO_ERROR)
               reply="OK";
         }
      if (!strncmp(token1,"CONTRAST",8))
         {
            sscanf(token2,numbformat,&tempnum);
            if (pmainWindow->setContrast(tempnum)==NO_ERROR)
               reply="OK";
         }
      if (!strncmp(token1,"SATURATION",10))
         {
            sscanf(token2,numbformat,&tempnum);
            if (pmainWindow->setSaturation(tempnum)==NO_ERROR)
               reply="OK";
         }
      if (!strncmp(token1,"MUTE",4))
         {
//         if (Muted) {BTMuxControl(AMUX_TUNER);Muted=FALSE;}
//            else {BTMuxControl(AMUX_MUTE);Muted=TRUE;}
            reply="OK";
         }
      if (!strncmp(token1,"SAVE",4))
         {
//           SaveFrameAsJPEG(token2, token3, cfg.videowidth, cfg.videoheight, 75, cfg.iscolor, buf);
            reply="OK";
         }
       reply=reply+IString("\r\n");
       rc = DosWrite(PipeHandle,(char *)reply, reply.length(), &ulBytes);

       if (rc != NO_ERROR) { printf("DosWrite error: return code = %u\n",rc);   }
//       rc = DosDisConnectNPipe(PipeHandle);

    }
}
