/*
** Module   :MSPFUNC.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Wed  23/07/2003 Created
**
*/
#include "mspfunc.h"
#include "btfuncs.h"
#include "bti2cs.h"

//#include "bti2c.h"
#include "btregs.h"
/* Read the MSP version string */
void MSPReadId(PTVCARD pbttv,UCHAR addr)
{
    int rev1=0, rev2=0;
    rev1 = MSPRead(pbttv, addr, 0x12, 0x001e);
    rev2 = MSPRead(pbttv, addr, 0x12, 0x001f);
    dprintf(LOG_DEBUG,"MSP version 34%02d%c-%c%d\n",(rev2>>8)&0xff, (rev1&0xff)+'@', ((rev1>>8)&0xff)+'@', rev2&0x1f);
//    sprintf(bktr->msp_version_string, "34%02d%c-%c%d",
//      (rev2>>8)&0xff, (rev1&0xff)+'@', ((rev1>>8)&0xff)+'@', rev2&0x1f);

}
void MSPInit(PTVCARD pbttv,UCHAR pin)
{
    APIRET rc;
    ULONG mask = 1 << pin;
    ULONG oldgpiomask=0;
    oldgpiomask=BTReadRegister(pbttv,GPIO_OUT_EN,4,rc);
    BTWriteRegister(pbttv,GPIO_OUT_EN,(oldgpiomask|mask),4);
    BTWriteRegister(pbttv,GPIO_DATA,mask,4);
    DosSleep(5);
    BTWriteRegister(pbttv,GPIO_DATA,0,4);
    DosSleep(5);
    BTWriteRegister(pbttv,GPIO_DATA,mask,4);
    DosSleep(5);
}
UINT MSPRead(PTVCARD pbttv, UCHAR i2caddr, UCHAR dev, UINT addr)
{
    UINT data;
    UCHAR addrl, addrh, data1, data2, devr ;
    APIRET rc;
    addrh = (addr >>8) & 0xff;
    addrl = addr & 0xff;
    devr = dev|1;

    BTI2CBusStart(pbttv);
    BTI2CBusSendByte(pbttv,i2caddr, MSPWAITACK);
    BTI2CBusSendByte(pbttv,devr, MSPWAITACK);
    BTI2CBusSendByte(pbttv,addrh, MSPWAITACK);
    BTI2CBusSendByte(pbttv,addrl, MSPWAITACK);
    BTI2CBusStart(pbttv);
    BTI2CBusSendByte(pbttv,i2caddr|1,MSPWAITACK);
    data1=BTI2CBusReadByte(pbttv, 0);
    data2=BTI2CBusReadByte(pbttv, 1);
    BTI2CBusStop(pbttv);
    data = (data1<<8) | data2;
    dprintf(LOG_DEBUG2,"i2cs read MSP34xx : i2c addr: %x dev: %x addr: %x - data: %x\n",i2caddr,dev,addr,data);
    return data;
}
void MSPWrite(PTVCARD pbttv, UCHAR i2caddr, UCHAR dev, UINT addr, UINT data)
{
    UCHAR addrl, addrh, datal, datah;
    APIRET rc;
    addrh = (addr >>8) & 0xff;
    addrl = addr & 0xff;
    datah = (data >>8) & 0xff;
    datal = data & 0xff;
    dprintf(LOG_DEBUG2,"i2cs write MSP34xx : i2c addr: %x dev: %x addr: %x data: %x\n",i2caddr,dev,addr,data);

    BTI2CBusStart(pbttv);
    BTI2CBusSendByte(pbttv,i2caddr, MSPWAITACK);
    BTI2CBusSendByte(pbttv,dev, MSPWAITACK);
    BTI2CBusSendByte(pbttv,addrh, MSPWAITACK);
    BTI2CBusSendByte(pbttv,addrl, MSPWAITACK);
    BTI2CBusSendByte(pbttv,datah, MSPWAITACK);
    BTI2CBusSendByte(pbttv,datal, MSPWAITACK);
    BTI2CBusStop(pbttv);
}

void MSPReset(PTVCARD pbttv, UCHAR i2caddr)
{
    /* put into reset mode */
    BTI2CBusStart(pbttv);
    BTI2CBusSendByte(pbttv,i2caddr, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x00, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x80, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x00, MSPWAITACK);
    BTI2CBusStop(pbttv);
    DosSleep(300);
    /* put back to operational mode */
    BTI2CBusStart(pbttv);
    BTI2CBusSendByte(pbttv,i2caddr, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x00, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x00, MSPWAITACK);
    BTI2CBusSendByte(pbttv,0x00, MSPWAITACK);
    BTI2CBusStop(pbttv);
    return;
}
APIRET MSPSetLoudspeakerVolume(PTVCARD pbttv, UINT volume, UCHAR clip)
{
    UINT data;
    data=(volume<<4)|(clip&0x0f);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0000,data);
    return NO_ERROR;
}
APIRET MSPSetLoudspeakerBalance(PTVCARD pbttv, UINT balance, UCHAR mode)
{
    UINT data;
    data=(balance<<8)|(mode&0x0f);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0001,data);
    return NO_ERROR;
}
APIRET MSPSetLoudspeakerBass(PTVCARD pbttv, UINT bass)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0002,bass<<8);
    return NO_ERROR;
}
APIRET MSPSetLoudspeakerTreble(PTVCARD pbttv, UINT treble)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0003,treble<<8);
    return NO_ERROR;
}

APIRET MSPSetLoudspeakerLoudness(PTVCARD pbttv, UINT loudness, UCHAR mode)
{
    UINT data;
    data=(loudness<<8)|(mode&0x0f);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0004,data);
    return NO_ERROR;
}
APIRET MSPSetLoudspeakerSpatial(PTVCARD pbttv, UINT strength, UCHAR mode, UCHAR coeff)
{
    UINT data;
    data=(strength<<8)|((mode&0x0f)<<4)|(coeff&0x0f);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0005,data);
    return NO_ERROR;
}
APIRET MSPSetSCART1Volume(PTVCARD pbttv, UINT volume, UCHAR mode)
{
    UINT data;
    if (mode==LOGARITHMIC) data=(volume<<4)|(LOGARITHMIC);
        else data=(volume<<8)|(LINEAR);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0007,data);
    return NO_ERROR;
}
APIRET MSPSetChannel(PTVCARD pbttv, UCHAR output, UCHAR source, UCHAR matrix)
{
    UINT data, reg;
    switch (output)
    {
        case OUT_LOUDSPEAKER:
            reg=0x0008;
            break;
        case OUT_SCART1:
            reg=0x000a;
            break;
        case OUT_QPDETECTOR:
            reg=0x000c;
            break;
    }
    data=(source<<8)|(matrix);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, reg,data);
    return NO_ERROR;
}
APIRET MSPSetSCARTPrescale(PTVCARD pbttv, UINT prescale)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x000d,prescale<<8);
    return NO_ERROR;
}
APIRET MSPSetFMPrescale(PTVCARD pbttv, UCHAR prescale, UCHAR matrix)
{
    UINT data;
    data=(prescale<<8)|(matrix);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x000e,data);
    return NO_ERROR;
}
APIRET MSPSetFMDeempasis(PTVCARD pbttv, UCHAR fixed, UCHAR adaptive)
{
    UINT data;
    data=(fixed<<8)|(adaptive);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x000f,data);
    return NO_ERROR;
}
APIRET MSPSetNICAMPrescale(PTVCARD pbttv, UINT prescale)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0010,prescale<<8);
    return NO_ERROR;
}
APIRET MSPSetACB(PTVCARD pbttv, UCHAR in, UCHAR out, BOOL dctl0, BOOL dctl1)
{
    // not implemented yet
    return NO_ERROR;
}
APIRET MSPSetBeeper(PTVCARD pbttv, UCHAR volume, UCHAR freq)
{
    UINT data;
    data=(volume<<8)|(freq);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0014,data);
    return NO_ERROR;
}
APIRET MSPSetIdent(PTVCARD pbttv, UCHAR ident)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0015,ident);
    return NO_ERROR;
}
APIRET MSPSetFMDCNotch(PTVCARD pbttv, UCHAR notch)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0017,notch);
    return NO_ERROR;
}
APIRET MSPSetAVC(PTVCARD pbttv, UCHAR mode, UCHAR decay)
{
    UINT data;
    data=(mode<<12)|(decay<<8);
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x12, 0x0029,data);
    return NO_ERROR;
}
#define FM_STEREO_MONO 0
#define FM_STEREO_STEREO 1
#define FM_STEREO_BILINGUAL 2
UCHAR MSPReadStereoDetection(PTVCARD pbttv)
{
    char data;
    data=(MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x0018)>>8);
    dprintf(LOG_DEBUG,"stereodetector %x \n",data);
    if (data>32) return FM_STEREO_STEREO;
    if (data<-32) return FM_STEREO_BILINGUAL;
    return FM_STEREO_MONO;
}
UINT MSPReadQPDetector(PTVCARD pbttv, UCHAR channel)
{
    UINT data;
    if (channel==QPD_LEFT)
    {
        data=MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x0019);
        return data;
    }
    if (channel==QPD_RIGHT)
    {
        data=MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x001a);
        return data;
    }
    return 0;
}
#define DCLEVEL_FM1 0
#define DCLEVEL_FM2 1
UINT MSPReadDCLevel(PTVCARD pbttv, UCHAR channel)
{
    UINT data;
    if (channel==DCLEVEL_FM1)
    {
        data=MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x001b);
        return data;
    }
    if (channel==DCLEVEL_FM2)
    {
        data=MSPRead(pbttv,pbttv->pcard->mspAddr,0x12,0x001c);
        return data;
    }
    return 0;
}
APIRET MSPSetAudioMode(PTVCARD pbttv, ULONG mode)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x10, 0x0020,mode);
    return NO_ERROR;
}
APIRET MSPSetNICAMFMMode(PTVCARD pbttv, ULONG mode)
{
    MSPWrite(pbttv,pbttv->pcard->mspAddr,0x10, 0x0021,mode);
    return NO_ERROR;
}
UINT MSPReadAudioStatus(PTVCARD pbttv)
{
    return MSPRead(pbttv,pbttv->pcard->mspAddr,0x10,0x007e);
}
UINT MSPReadCADBits(PTVCARD pbttv)
{
    return MSPRead(pbttv,pbttv->pcard->mspAddr,0x10,0x0023);
}
UINT MSPReadADDBits(PTVCARD pbttv)
{
    return MSPRead(pbttv,pbttv->pcard->mspAddr,0x10,0x0038);
}
UINT MSPReadCIBBits(PTVCARD pbttv)
{
    return MSPRead(pbttv,pbttv->pcard->mspAddr,0x10,0x003e);
}
UINT MSPReadErrorRate(PTVCARD pbttv)
{
    return MSPRead(pbttv,pbttv->pcard->mspAddr,0x10,0x0057);
}

