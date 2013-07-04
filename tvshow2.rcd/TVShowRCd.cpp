/*
** Module   :TVSHOWRCD.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Sat  04/01/2003 Created
**
*/

#include "base.h"
#include "card.h"
#include "btfuncs.h"
#include "btregs.h"
#include "cfgfile.h"

TVCARD bttv;
ULONG rcmask;

extern "C" void _Optlink RCThread(void * arg)
{
    CHAR     message[256]  = "";                     /* Input/Output buffer */
    CHAR     PipeName[256]  = "\\pipe\\tvshowremote";
    HPIPE    PipeHandle    = NULLHANDLE;             /* Pipe handle */
    ULONG    ulBytes;
    CHAR     messformat[256] = "%s %s %80c";
    CHAR     numbformat[256] = "%d";
    int      tempnum;
    CHAR     token1[256];
    CHAR     token2[256];
    CHAR     token3[256];
    CHAR     reply[256];
    APIRET rc;
    ULONG LastRCCommand, NewRCCommand;
    CHAR rcbuffer[64];
    int flag=TRUE;

    LastRCCommand=BTReadRegister(&bttv,GPIO_DATA,4,rc)&rcmask;
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
        dprintf(LOG_ERROR,"DosCreateNPipe error: return code = %u\n",rc);
        return;
    }
    while (flag)
    {
        rc = DosConnectNPipe(PipeHandle);
        if (rc != NO_ERROR) {dprintf(LOG_ERROR,"DosConnectNPipe error: return code = %u\n",rc);}
        while (flag)
        {
            NewRCCommand=BTReadRegister(&bttv,GPIO_DATA,4,rc)&rcmask;
            if (LastRCCommand!=NewRCCommand)
            {
               LastRCCommand=NewRCCommand;
               dprintf(LOG_INFO,"Command from RC: %X\n", NewRCCommand);
               sprintf(rcbuffer,"RCCODE: %X\n\0x00",NewRCCommand);
               rc = DosWrite(PipeHandle,rcbuffer, strlen(rcbuffer), &ulBytes);
               if (rc != NO_ERROR) { dprintf(LOG_ERROR,"DosWrite error: return code = %u\n",rc);}
               if (rc==ERROR_BROKEN_PIPE) {break;}
            }
        DosSleep(100);
        }
        rc = DosDisConnectNPipe(PipeHandle);
        if (rc != NO_ERROR) {dprintf(LOG_ERROR,"DosDisconnectNPipe error: return code = %u\n",rc);}
    }

}

int main(int argc, char **argv)
{
    Variable Vars[]=
    {
        {"remote.remotemask" , sizeof(rcmask)      , &rcmask        , Pdword },
        {0}
    };
    APIRET rc;
    LoadConfig("video.cfg", Vars);
    dprintf(LOG_MESSAGE,"TVShow Remote Control daemon. \n");
    LogLevel=9;
    strcpy(LogFile,"tvshowrcd.log");
    rc=BTFindCard(&bttv);
    if (rc>0) return 4;

//    rc = DosExitList(EXLST_ADD             /* Add to exit-list sequence     */
//                     | 0x00002A00,         /* Invocation order is 42 (0x2A) */
//                     (PFNEXITLIST) ExitRtn1); /* Specify added exit routine */

//     if (rc != NO_ERROR) {
//       printf("DosExitList error: return code = %u\n", rc);
//       return 1;
//     }

	int rctid;

    rctid=_beginthread(&RCThread,NULL,32768,NULL);

    DosWaitThread((PTID)&rctid, DCWW_WAIT);
    BTFreeCard(&bttv);
}
