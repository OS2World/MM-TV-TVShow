#include <string.h>
//#include <time.h>

#include "base.h"
#include "btfuncs.h"
#include "bttuner.h"
#include "tvfuncs.h"
#include "logger.h"
#include "btregs.h"
#include "tvaudio.h"
#include "pipehndl.hpp"

//#include "fmfuncs.h"
//#include "ioctl.h"
//#include "maphymem.h"
#include "globals.h"
#include "driver.h"
#include "memory.h"
#define SAFETY_SPACE 32768
ULONG videolen,physaddrframe,physaddrrisc;

#include "btrisc.h"
int main()
{
    APIRET rc;
    int pipetid;
    int bpp;
    ULONG vlinelen;
    const progperline = 8;
	int progneeded;


    LogLevel=9;
    strcpy(LogFile,"grabber.log");

    pbttv=&bttv;
    TVParseConfig(&cfg);
    rc=BTFindCard(pbttv);
    if (rc>0) return 4;
    if (cfg.iscolor) bpp=2; else bpp=1;

    vlinelen = cfg.videowidth*bpp;
    videolen = vlinelen * cfg.videoheight;
    progneeded = progperline*cfg.videoheight+4*8+8+SAFETY_SPACE;

    dprintf(LOG_DEBUG,"Allocate frame buffer \n");
// allocate frames
    rc=MemCreateFrame(pbttv,&EvenFrame,cfg.videowidth,cfg.videoheight/2,bpp);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"CreateFrame even failed, return code %u \n", rc); return 6;};
    rc=MemCreateFrame(pbttv,&OddFrame,cfg.videowidth,cfg.videoheight/2,bpp);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"CreateFrame odd failed, return code %u \n", rc); return 6;};

    dprintf(LOG_DEBUG,"AllocMap risc buffer \n");
    rc=BTAllocMapContMem(pbttv->hDriver,progneeded,&physaddrrisc,(PPVOID)&riscbuf);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"BTAllocMapMem failed, return code %u \n", rc); return 6;};
    memset(riscbuf,0,progneeded);
    BTSelectCard(pbttv,cfg.cardtype);
//    if (cfg.tunertype>0) BTSelectTuner(pbttv,cfg.tunertype);
//    if (cfg.cardtype==0) BTSelectTuner(pbttv,cfg.tunertype);
    BTReset(pbttv);
    BTDisableInt(pbttv,0xffffff);
    TVSelectInput(pbttv,cfg.connector);

    TVSetSaturation(pbttv,cfg.saturation);
    TVSetContrast(pbttv,cfg.contrast);
    TVSetBrightness(pbttv,cfg.brightness);

//    BTWriteRegister(pbttv,GPIO_OUT_EN,pbttv->pcard->gpiomask,4);
//    TVMuxControl(pbttv,AMUX_TUNER);
    rc=BTSelectColorSystem(pbttv,cfg.colorsystem);
    if (!cfg.iscolor)
       {
          BTSetColorFormat(pbttv,BT_COLOR_GS,0);
          }
        else {
                if ((cfg.blittermode==1)||(cfg.blittermode==2))
                   {
                      BTSetColorFormat(pbttv,BT_COLOR_YUV2422,0);
                      }
                   else {
                      BTSetColorFormat(pbttv,BT_COLOR_RGB16,0);
                      }
//              BTWriteRegister(COLOR_CTL,0x43,1);
              }

//    BTColorBar(pbttv,TRUE);
    BTMakeProgramFrame(pbttv,&EvenFrame,&OddFrame,physaddrrisc,riscbuf);
    dprintf(LOG_DEBUG,"program start phys %08xh\n",physaddrrisc);
    rc=BTStartCapture(pbttv,physaddrrisc, FALSE, FALSE, 0);
//    BTRiscDump(physaddrrisc,riscbuf);
    pipetid=_beginthread(&PipeThread,NULL,32768,NULL);

//main loop
    rc=DosWaitThread((PTID)&pipetid,DCWW_WAIT);
    dprintf(LOG_DEBUG,"waitthread rc %d \n",rc);

//    TVMuxControl(pbttv,AMUX_MUTE);
    BTStopCapture(pbttv,FALSE, 0);
    BTDisableInt(pbttv,0xffffff);

    dprintf(LOG_DEBUG,"Free frame buffer\n");

    rc=MemDeleteFrame(pbttv,&OddFrame);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"DeleteFrame odd failed, return code %u \n", rc); return 6;};
    rc=MemDeleteFrame(pbttv,&EvenFrame);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"DeleteFrame even failed, return code %u \n", rc); return 6;};
    dprintf(LOG_DEBUG,"Free RISC buffer\n");
    BTFreeContMem(pbttv->hDriver,physaddrrisc,riscbuf);
    BTFreeCard(pbttv);
    return 0;
} /* end main */


