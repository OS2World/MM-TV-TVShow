/*
** Module   :FMFUNCS.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Mon  21/10/2002 Created
**
*/
#ifndef __FMFUNCS_H
#define __FMFUNCS_H

#if __cplusplus
extern "C" {
#endif


APIRET FMReadChannelList(PTVCARD bttv, char * channfilename);
APIRET FMWriteChannelList(PTVCARD bttv, char * channfilename);
void FMSelectChannel(PTVCARD pbttv,UCHAR channum);
void FMSelectNextChannel(PTVCARD pbttv);
void FMSelectPrevChannel(PTVCARD pbttv);
void FMScanChannels(PTVCARD pbttv);

#if __cplusplus
}
#endif

#endif  /*__FMFUNCS_H*/

