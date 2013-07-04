/*
** Module   :TVFUNCS.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Mon  21/10/2002 Created
**
*/
#ifndef __TVFUNCS_H
#define __TVFUNCS_H

#if __cplusplus
extern "C" {
#endif

#include "card.h"

APIRET TVReadChannelList(PTVCARD bttv, char * channfilename);
APIRET TVWriteChannelList(PTVCARD bttv, char * channfilename);
APIRET TVSelectChannel(PTVCARD pbttv,UCHAR channum);
void TVSelectNextChannel(PTVCARD pbttv);
void TVSelectPrevChannel(PTVCARD pbttv);
void TVScanChannels(PTVCARD pbttv);
void TVSelectInput(PTVCARD pbttv,UCHAR input);
//0-100 value
APIRET TVSetBrightness(PTVCARD pbttv,UCHAR brightness);
APIRET TVSetContrast(PTVCARD pbttv,UCHAR contrast);
APIRET TVSetSaturation(PTVCARD pbttv,UCHAR saturation);
APIRET TVSetVolume(PTVCARD pbttv,UCHAR volume);
APIRET TVSetColorSystem(PTVCARD pbttv, UCHAR colorsystem);
void TVIncBrightness(PTVCARD pbttv);
void TVDecBrightness(PTVCARD pbttv);
void TVIncContrast(PTVCARD pbttv);
void TVDecContrast(PTVCARD pbttv);
void TVIncSaturation(PTVCARD pbttv);
void TVDecSaturation(PTVCARD pbttv);
void TVIncVolume(PTVCARD pbttv);
void TVDecVolume(PTVCARD pbttv);
void TVUpdateCurrentChannel(PTVCARD pbttv);
void TVAddChannel(PTVCARD pbttv, PTVCHANNEL newchannel);

#if __cplusplus
}
#endif

#endif  /*__FMFUNCS_H*/

