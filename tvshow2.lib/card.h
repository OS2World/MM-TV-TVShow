/*
** Module   :CARD.H
** Abstract :logical cards and tuner
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  15/10/2002 Created
**
*/
#ifndef __CARD_H
#define __CARD_H

#if __cplusplus
extern "C" {
#endif

//#pragma pack(1)


#include "base.h"

#include "phcard.h"

#define MAX_CHUNKS 65535
typedef struct {
    USHORT usSize; //block size, bytes. <64K
    ULONG ulPhysAddr; //physical address
    PVOID pVirtAddr; // virtual address
    ULONG ulLinAddr; // linear addres, internal use
} MEMCHUNK;
typedef MEMCHUNK* PMEMCHUNK;
typedef struct {
    USHORT lines; //video lines in frame
    USHORT pixels; //pixel per line
    UCHAR  bpp;    //bytes per pixel
    USHORT linesize; //video line size
    USHORT linesperchunk; //lines in single memchunk
    UCHAR  numchunks; //actual number of chunks
    MEMCHUNK chunks[MAX_CHUNKS]; //array of chunks
} VIDEOFRAME;
typedef VIDEOFRAME* PVIDEOFRAME;

/* Logical tuner */
#define LoBand  0
#define MedBand 1
#define HiBand  2
#define FMBand  3
#define TVMode  0
#define FMMode  1
// logical channel
typedef struct {
    UCHAR   channel;
    ULONG   frequency; //Hz
    UCHAR   colorsystem;
    UCHAR   brightness; //0-100
    UCHAR   contrast;
    UCHAR   saturation;
    UCHAR   volume;
    char    name[255];
} TVCHANNEL;
typedef TVCHANNEL* PTVCHANNEL;
typedef struct {
    UCHAR   channel;
    ULONG   frequency; //Hz
    UCHAR   volume;
    char    name[255];
} FMCHANNEL;
typedef FMCHANNEL* PFMCHANNEL;

// logical tuner
typedef struct {
    ULONG   frequency; //Hz
    UCHAR   band;      //band code
    UCHAR	afc;
    UCHAR	mode;   /* current mode of the radio mode */
} TVTUNER;
typedef struct {
    USHORT vendor;
    USHORT device;
    USHORT chipcode;
    ULONG  latency;
    USHORT sybsystemID;
    USHORT sybsystemvendorID;
    UCHAR  IRQLine;
} TVCHIPINFO;

// logical card structure. one per card
typedef struct {
    TVTUNER	tuner;    //logical tuner
    PCARDPH pcard;  //pointer to card details
    TVCHIPINFO tvchip; //chip specific info
    HFILE hDriver;    // Driver handle
    volatile PUCHAR pbtbasecomaddr; //pointer to registers area
    UCHAR   videoinput;
//    ULONG   freq;
    UCHAR	colorsystem;
    UCHAR	brightness;
    UCHAR	contrast;
    UCHAR	saturation;
    UCHAR   volume;
    UCHAR   audiosystem; // for advanced sound decoders
    char    mode;
    UCHAR   channel;
    char    channelname[255];
    TVCHANNEL tvchannels[200];
    char    tvchannelcount;
    FMCHANNEL fmchannels[60];
    char    fmchannelcount;
    VIDEOFRAME OddFrame,EvenFrame; //frames
    PVOID   pRiscVirt; //RISC program buffer address virt
    ULONG   ulRiscPhys; //phys
} TVCARD;
typedef TVCARD* PTVCARD;

//#pragma pack()

#if __cplusplus
}
#endif


#endif  /*__CARD_H*/

