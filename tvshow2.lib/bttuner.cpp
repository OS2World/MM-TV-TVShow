/*
** Module   :BTTUNER.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Sat  11/01/2003 Created
**
*/
#include "bti2c.h"
#include "bttuner.h"

APIRET BTSetFreq(PTVCARD pbttv,ULONG frequency,USHORT type)
{
   double tempdouble;
   ULONG tempdword;
   USHORT tempword;
   UCHAR cb1,cb2,pb1,pb2;
   int i;
   if (type==TVMode)
    for (i=0;i<3;i++)
    {
        if ((frequency>=(pbttv->pcard->ptuner->BandLim[i*2]))&&(frequency<=pbttv->pcard->ptuner->BandLim[i*2+1]))
        {
            cb1=pbttv->pcard->ptuner->TunerCB1[i];
            cb2=pbttv->pcard->ptuner->TunerCB2[i];
            tempdouble=(double)frequency;
            tempdouble=(tempdouble+38900000)/1000000;
            tempword=(USHORT)(tempdouble*16);
            tempword=tempword&0x7fff;
            pb2=(UCHAR)tempword;
            pb1=(UCHAR)(tempword>>8);
            pbttv->tuner.band=i;
            break;
        }
        else if (i==3) return 1;
    }
   if (type==FMMode)
        if ((frequency>=pbttv->pcard->ptuner->BandLim[6])&&(frequency<=pbttv->pcard->ptuner->BandLim[7]))
        {
            cb1=pbttv->pcard->ptuner->TunerCB1[3];
            cb2=pbttv->pcard->ptuner->TunerCB2[3];
            tempdouble=(double)frequency;
            tempdouble=(tempdouble+10700000)/1000000;
            tempword=(USHORT)(tempdouble*20);
            tempword=tempword&0x7fff;
            pb2=(UCHAR)tempword;
            pb1=(UCHAR)(tempword>>8);
            pbttv->tuner.band=3;
        }
        else return 1;

    pbttv->tuner.frequency=frequency;
    pbttv->tuner.mode=type;
    dprintf(LOG_DEBUG2,"i2c write tuner : addr: %x cb1: %x cb2: %x pb1: %x pb2: %x\n",pbttv->pcard->tunerAddr,cb1,cb2,pb1,pb2);
    BTI2CWriteFirst(pbttv,pbttv->pcard->tunerAddr,cb1);
    BTI2CWrite(pbttv,cb2);
    BTI2CWrite(pbttv,pb1);
    BTI2CWriteLast(pbttv,pb2);
    return 0;
}
void BTSelectTuner(PTVCARD pbttv,UINT tuner_type)
{
    if (tuner_type < TUNER_MAX)
        {
            pbttv->pcard->ptuner = &tuners[tuner_type];
        }
      else
        {
            pbttv->pcard->ptuner = NULL;
        }
}
void BTSelectCard(PTVCARD pbttv,UINT card_type)
{
    if (card_type < CARD_MAX)
        {
            pbttv->pcard = &cards[card_type];
        }
      else
        {
            pbttv->pcard = NULL;
        }
}
UCHAR BTGetTunerStatus(PTVCARD pbttv)
{
APIRET rc;
UCHAR tempbyte;

    tempbyte=BTI2CReadFirst(pbttv, pbttv->pcard->tunerAddr,rc);
    BTI2CReadLast(pbttv,rc);
    dprintf(LOG_DEBUG2,"i2c read tuner : addr: %x data: %x\n",pbttv->pcard->tunerAddr,tempbyte);
    return tempbyte;

}
