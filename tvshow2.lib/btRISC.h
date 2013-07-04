/*
** Module   :BTRISC.H
** Abstract :BT878 Risc codes
**
** Copyright (C) Alex Cherkaev
**
** Log: Mon  04/03/2002 Created
**
*/


#ifndef __BTRISC_H
#define __BTRISC_H

#if __cplusplus
extern "C" {
#endif


#include "base.h"

#define FIFO_EOL4   1
#define FIFO_SOL    2
#define FIFO_FM1    6
#define FIFO_FM3    0xe
#define FIFO_EOL3   13
#define FIFO_EOL2   9
#define FIFO_EOL1   5
#define FIFO_VRE    4
#define FIFO_VRO    12
#define FIFO_RESYNC 0x8000
#define OC_WRITE    1
#define OC_SYNC     8
#define OC_JMP      7

typedef struct
{
    int byte_count:12;
    int byte_en:4;
    int rstat:8;
    int irq:1;
    int res:1;
    int eol:1;
    int sol:1;
    int ocode:4;
    ULONG target;
} CMD_WRITE;
typedef struct
{
    USHORT status;
    UCHAR rstat;
    int irq:1;
    int res:3;
    int ocode:4;
    ULONG res2;
} CMD_SYNC;
typedef struct
{
    USHORT reserved;
    UCHAR rstat;
    int irq:1;
    int res:3;
    int ocode:4;
    ULONG jaddr;
} CMD_JMP;

#if __cplusplus
}
#endif


#endif  /*__BTRISC_H*/

