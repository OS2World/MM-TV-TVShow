/*
** Module   :CHANSET.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Thu  20/02/2003 Created
**
*/

#ifndef __CHANSET_H
#define __CHANSET_H
typedef struct {
    int basechannel;
    int basechannelfreq;
    int channelsoffset;
} CHANSUBSET;
typedef struct {
    char[40] Name;
    int MaxChannel;
    int IF_Freq;
    CHANSUBSET[100] channels;
} CHANSET;
#define CHANSET_TOTAL 1

#endif  /*__CHANSET_H*/

