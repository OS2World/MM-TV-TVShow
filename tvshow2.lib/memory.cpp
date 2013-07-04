/*
** Module   :MEMORY.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Fri  09/04/2004 Created
**
*/
#include "memory.h"
#include "driver.h"

APIRET MemCreateChunk(PTVCARD pbttv, PMEMCHUNK pmemchunk, USHORT size)
{
    APIRET rc;
    rc=BTAllocMapMem(pbttv->hDriver,size,&pmemchunk->ulPhysAddr,&pmemchunk->pVirtAddr,&pmemchunk->ulLinAddr);
    if (rc!=NO_ERROR) return 1;
    pmemchunk->usSize=size;
    return NO_ERROR;
}
APIRET MemDeleteChunk(PTVCARD pbttv, PMEMCHUNK pmemchunk)
{
    APIRET rc;
    rc=BTFreeMem(pbttv->hDriver,pmemchunk->ulPhysAddr,pmemchunk->pVirtAddr,pmemchunk->ulLinAddr);
    if (rc!=NO_ERROR) return 1;
    pmemchunk->usSize=0;
    return NO_ERROR;
}
APIRET MemCreateFrame(PTVCARD pbttv, PVIDEOFRAME pframe, USHORT pixels, USHORT lines, UCHAR bpp)
{
    APIRET rc;
    int i;
    pframe->pixels=pixels;
    pframe->lines=lines;
    pframe->bpp=bpp;
    pframe->linesize=pixels*bpp;
    pframe->linesperchunk=(USHORT)61000/pframe->linesize;
    pframe->numchunks=(USHORT)pframe->lines/pframe->linesperchunk;
    if (pframe->lines%pframe->linesperchunk) pframe->numchunks++;
    for (i=0;i<pframe->numchunks;i++)
    {
        rc=MemCreateChunk(pbttv,&pframe->chunks[i],pframe->linesize*pframe->linesperchunk);
        if (rc!=NO_ERROR)
        {
            dprintf(LOG_DEBUG,"Create chunk number %d failed, rc %d\n",i,rc);
            return 1;
        }
    }
    return NO_ERROR;
}
APIRET MemDeleteFrame(PTVCARD pbttv, PVIDEOFRAME pframe)
{
    APIRET rc;
    int i;
    for (i=0;i<pframe->numchunks;i++)
    {
        rc=MemDeleteChunk(pbttv,&pframe->chunks[i]);
        if (rc!=NO_ERROR)
        {
            dprintf(LOG_DEBUG,"Delete chunk number %d failed, rc %d\n",i,rc);
            return 1;
        }
    }
    pframe->pixels=0;
    pframe->lines=0;
    pframe->bpp=0;
    pframe->linesize=0;
    pframe->linesperchunk=0;
    pframe->numchunks=0;
    return NO_ERROR;
}


