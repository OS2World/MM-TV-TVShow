/*
** Module   :BTAUDIO.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  28/01/2003 Created
**
*/
#ifndef __BTAUDIO_H
#define __BTAUDIO_H
#include "phcard.h"
#include "card.h"

void TVMuxControl(PTVCARD pbttv,UCHAR mode);
void TVSetAudioSystem(PTVCARD pbttv,UCHAR system);

#endif  /*__BTAUDIO_H*/

