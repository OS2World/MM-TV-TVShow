
#include "base.h"

//#include "tvshow2.h"
#include "logger.h"
#include "tvfuncs.h"
#include "btregs.h"
#include "bttuner.h"
#include "btfuncs.h"
#include "pipehndl.hpp"
#include "savejpeg.hpp"

extern "C" void _Optlink PipeThread(void * arg)
{

  CHAR     message[256]  = "";                     /* Input/Output buffer */
  CHAR     PipeName[256]  = "\\pipe\\tvshow";
  HPIPE   PipeHandle;
  ULONG    ulBytes;
  CHAR     messformat[256] = "%s %s %80c";
  CHAR     numbformat[256] = "%d";
  CHAR     statusreplyform[256] = "STATUS: %d,%d,%d,%d,%d,%d\n";
//  CHAR     statusreply[256] = "";
  UCHAR      tempnum;
  CHAR     token1[256];
  CHAR     token2[256];
  CHAR     token3[256];
  CHAR     reply[256];
  PVOID    vidbuf;
  APIRET rc;
  int err;
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
//        int i;
        memcpy(reply,"ERROR\n\0x00",6);
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
            memcpy(reply,"OK\n\0x00",3);
         }
      if (!strncmp(token1,"CHANNEL",7))
         {
            sscanf(token2,numbformat,&tempnum);
//            BTMuxControl(AMUX_MUTE);
//            BTSelectChannel(tempnum);
//            if (!Muted) BTMuxControl(AMUX_TUNER);
            memcpy(reply,"OK\n\0x00",3);
         }
      if (!strncmp(token1,"NEXTCHANNEL",11))
         {
//            BTMuxControl(AMUX_MUTE);
//            BTSelectNextChannel();
//            if (!Muted) BTMuxControl(AMUX_TUNER);
            memcpy(reply,"OK\n\0x00",3);
         }
      if (!strncmp(token1,"PREVCHANNEL",11))
         {
//            BTMuxControl(AMUX_MUTE);
//            BTSelectPrevChannel();
//            if (!Muted) BTMuxControl(AMUX_TUNER);
            memcpy(reply,"OK\n\0x00",3);
         }
      if (!strncmp(token1,"INPUT",5))
         {
            sscanf(token2,numbformat,&tempnum);
            if ((tempnum>=0)&&(tempnum<=3))
            {
              TVSelectInput(pbttv,tempnum);
              memcpy(reply,"OK\n\0x00",3);
            }
         }
      if (!strncmp(token1,"SYSTEM",6))
         {
            sscanf(token2,numbformat,&tempnum);
//            BTMuxControl(AMUX_MUTE);
            if ((tempnum>=0)&&(tempnum<=7))
            {
                BTSelectColorSystem(pbttv,tempnum);
//            if (!Muted) BTMuxControl(AMUX_TUNER);
                memcpy(reply,"OK\n\0x00",3);
            }
         }
      if (!strncmp(token1,"BRIGHTNESS",10))
         {
            sscanf(token2,numbformat,&tempnum);
            if ((tempnum>=0)&&(tempnum<=100))
            {
                TVSetBrightness(pbttv,tempnum);
                memcpy(reply,"OK\n\0x00",3);
            }
         }
      if (!strncmp(token1,"CONTRAST",8))
         {
            sscanf(token2,numbformat,&tempnum);
            if ((tempnum>=0)&&(tempnum<=100))
            {
                TVSetContrast(pbttv,tempnum);
                memcpy(reply,"OK\n\0x00",3);
            }
         }
      if (!strncmp(token1,"SATURATION",10))
         {
            sscanf(token2,numbformat,&tempnum);
            if ((tempnum>=0)&&(tempnum<=100))
            {
                TVSetSaturation(pbttv,tempnum);
                memcpy(reply,"OK\n\0x00",3);
            }
         }
      if (!strncmp(token1,"MUTE",4))
         {
//         if (Muted) {BTMuxControl(AMUX_TUNER);Muted=FALSE;}
//            else {BTMuxControl(AMUX_MUTE);Muted=TRUE;}
            memcpy(reply,"OK\n\0x00",3);
         }
      if (!strncmp(token1,"SAVE",4))
         {
            APIRET rcjpeg;
            err=0;
            UCHAR dstat;
            dstat=BTReadRegister(pbttv,DSTATUS, 1, rc);

            dprintf(LOG_INFO,"Save command, INT_STAT %x, DSTATUS %x \n",BTReadRegister(pbttv,INT_STAT,4,rc),dstat);
            vidbuf=malloc(cfg.videowidth*cfg.videoheight*EvenFrame.bpp);
            BTCopyDualFrame(pbttv,vidbuf,&EvenFrame,&OddFrame);
            rcjpeg=SaveFrameAsJPEG(token2, token3, cfg.videowidth, cfg.videoheight, 75, cfg.iscolor,(PCHAR)vidbuf);
            free(vidbuf);
            memcpy(reply,"OK\n\0x00",5);
//            printf("dstat %X\n",dstat);
            if ((dstat&BT848_DSTATUS_PRES)==0)
               {  dprintf(LOG_ERROR,"Video not present\n");
                  err|=1;
                  }
            if ((dstat&BT848_DSTATUS_HLOC)==0)
               {  dprintf(LOG_ERROR,"Sync not present\n");
                  err|=2;
                  }
            if (rcjpeg==1)
               {  dprintf(LOG_ERROR,"File I/O Error\n");
                  err|=4;
                  }
            if (rcjpeg==9)
               {  dprintf(LOG_ERROR,"Warning: possible black frame\n");
                  err|=8;
                  }
            if (err!=0)
               {  sprintf(reply,"ERROR %d \n",err);
                  }

         }
    if (!strncmp(token1,"STATUS",6))
    {
       UCHAR dstat;
       char vidpres=0, hlock=0;
       dstat=BTReadRegister(pbttv,DSTATUS, 1, rc);
       if ((dstat&BT848_DSTATUS_PRES)!=0) vidpres=1;
       if ((dstat&BT848_DSTATUS_HLOC)!=0) hlock=1;
       sprintf(reply,statusreplyform,pbttv->brightness,pbttv->contrast,pbttv->saturation, pbttv->videoinput,vidpres,hlock);
    }
    rc = DosWrite(PipeHandle,reply, strlen(reply), &ulBytes);
    if (rc != NO_ERROR) { printf("DosWrite error: return code = %u\n",rc);   }
//       rc = DosDisConnectNPipe(PipeHandle);

    }
}
