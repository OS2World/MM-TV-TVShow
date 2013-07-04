/*
** Module   :BTTUNER.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Fri  10/05/2002 Created
**
*/

#ifndef __BTTUNER_H
#define __BTTUNER_H

#if __cplusplus
extern "C" {
#endif


#include "card.h"

/*#define TunerLoBandL   4575
#define TunerLoBandH   16825
#define TunerMidBandL  17525
#define TunerMidBandH  44725
#define TunerHiBandL   45525
#define TunerHiBandH   85525
#define TunerFMBandL   8750
#define TunerFMBandH   10800
#define TunerLoBand    0xa0
#define TunerMedBand   0x90
#define TunerHiBand    0x30
#define TunerFMBand    0xa4
#define TunerCB1TV     0xce
#define TunerAddr      0xc2

// 0xce
#define TunerCB1FM     0x88
*/

APIRET BTSetFreq(PTVCARD pbttv,ULONG frequency, USHORT type);
void BTSelectTuner(PTVCARD pbttv,UINT tuner_type);
void BTSelectCard(PTVCARD pbttv,UINT card_type);
UCHAR BTGetTunerStatus(PTVCARD pbttv);

#if __cplusplus
}
#endif


#endif  /*__BTTUNER_H*/

