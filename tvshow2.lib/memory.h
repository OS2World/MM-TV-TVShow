/*
** Module   :MEMORY.H
** Abstract : memory related functions
**
** Copyright (C) Alex Cherkaev
**
** Log: Mon  05/04/2004 Created
**
*/

#ifndef __MEMORY_H
#define __MEMORY_H

#if __cplusplus
extern "C" {
#endif


#include "base.h"
#include "card.h"

APIRET MemCreateChunk(PTVCARD pbttv, PMEMCHUNK pmemchunk, USHORT size);
APIRET MemDeleteChunk(PTVCARD pbttv, PMEMCHUNK pmemchunk);
APIRET MemCreateFrame(PTVCARD pbttv, PVIDEOFRAME pframe, USHORT pixels, USHORT lines, UCHAR bpp);
APIRET MemDeleteFrame(PTVCARD pbttv, PVIDEOFRAME pframe);
#if __cplusplus
}
#endif

#endif  /*__MEMORY_H*/

