#include "base.h"
#include "btfuncs.h"
#include "bttuner.h"
#include "card.h"
#include "logger.h"
#include <string.h>
#include "btregs.h"
#include "videocfg.h"
#include "fmfuncs.h"
#include "tvaudio.h"
#include "bti2c.h"
TVCARD bttv;
PTVCARD pbttv;
CFGDATA cfg;

main()
{
    APIRET rc;
    int frequency=101000000;

    LogLevel=9;
    strcpy(LogFile,"fmshow.log");

    pbttv=&bttv;
    TVParseConfig(&cfg);
    rc=BTFindCard(pbttv);
    if (rc>0) return 4;
    BTSelectCard(pbttv,cfg.cardtype);
    if (cfg.tunertype>0) BTSelectTuner(pbttv,cfg.tunertype);
    if (cfg.cardtype==0) BTSelectTuner(pbttv,cfg.tunertype);
    BTReset(pbttv);
    BTDisableInt(pbttv,0xffffff);
    BTWriteRegister(pbttv,GPIO_OUT_EN,pbttv->pcard->gpiomask,4);
    TVMuxControl(pbttv,AMUX_INT);
    for (;;)
    {
    printf("frequency?: \n");
    scanf("%i",&frequency);
    if (frequency==0) break;
    if (frequency==1)
    {
        UCHAR tempbyte;
        for (;;)
        {
            tempbyte=BTGetTunerStatus(pbttv);
//        tempbyte=BTI2CReadFirst(pbttv,0xC2,rc);
//        printf("first tuner stat %x POR: %d FL: %d I210 %d A %d\n",tempbyte,(tempbyte&0x80)>>7, (tempbyte&0x40)>>6,(tempbyte&0x38)>>3,(tempbyte&7));
//                    DosSleep(1000);
//        tempbyte=BTI2CRead(pbttv,rc);
//        printf("read tuner stat %x POR: %d FL: %d I210 %d A %d\n",tempbyte,(tempbyte&0x80)>>7, (tempbyte&0x40)>>6,(tempbyte&0x38)>>3,(tempbyte&7));
//                    DosSleep(1000);
//        BTI2CWrite(pbttv,0xBB);
 //       tempbyte=BTI2CReadLast(pbttv,rc);
        printf("last tuner stat %x POR: %d FL: %d I210 %d A %d\n",tempbyte,(tempbyte&0x80)>>7, (tempbyte&0x40)>>6,(tempbyte&0x38)>>3,(tempbyte&7));
//                BTI2CWriteLast(pbttv,0);

//            DosSleep(1000);
        }
        continue;
    }
    if (frequency==2)
    {
        FMReadChannelList(pbttv, "station.dat");
        TVMuxControl(pbttv,AMUX_MUTE);
        FMSelectChannel(pbttv,1);
        TVMuxControl(pbttv,AMUX_INT);
        continue;
    }
    if (frequency==3)
    {
        TVMuxControl(pbttv,AMUX_MUTE);
        FMSelectPrevChannel(pbttv);
        TVMuxControl(pbttv,AMUX_INT);
        continue;
    }
    if (frequency==4)
    {
        TVMuxControl(pbttv,AMUX_MUTE);
        FMSelectNextChannel(pbttv);
        TVMuxControl(pbttv,AMUX_INT);
        continue;
    }
    if (frequency==5)
    {
//        TVMuxControl(pbttv,AMUX_MUTE);
        FMScanChannels(pbttv);
//        TVMuxControl(pbttv,AMUX_INT);
        continue;
    }
        if (frequency==7)
    {
        dprintf(LOG_DEBUG,"First byte from eeprom %x\n",BTI2CReadFirst(pbttv,0xa0,rc));
        continue;
    }
        if (frequency==8)
    {
        dprintf(LOG_DEBUG,"byte from eeprom %x\n",BTI2CRead(pbttv,rc));
        continue;
    }
        if (frequency==9)
    {
        dprintf(LOG_DEBUG,"last byte from eeprom %x\n",BTI2CReadLast(pbttv,rc));
        continue;
    }


    BTSetFreq(pbttv,frequency, FMMode);
}
    TVMuxControl(pbttv,AMUX_MUTE);
    BTFreeCard(pbttv);
}
