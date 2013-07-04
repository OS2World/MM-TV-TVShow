/*
** Module   :PHCARD.H
** Abstract :Physical card & tuner
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  15/10/2002 Created
**
*/
#ifndef __PHCARD_H
#define __PHCARD_H
#if __cplusplus
extern "C" {
#endif

#include "base.h"

/* description of the PHYSICAL tuner */
#define TUNER_UNKNOWN       0
#define TUNER_PHILIPS1246   1
#define TUNER_TCL2002MB     2
#define TUNER_PHILIPS1236   3
#define TUNER_PHILIPS1216   4
#define TUNER_THEMIC4002FH5 5
#define TUNER_THEMIC4032FY5 6
#define TUNER_THEMIC4062FY5 7
#define TUNER_THEMIC4036FY5 8
#define TUNER_ALPSHSBH1     9
#define TUNER_ALPSTSBE1     10
#define TUNER_ALPSTSBB5     11
#define TUNER_ALPSTSBE5     12
#define TUNER_ALPSTSBC5     13
#define TUNER_ALPSTSCH6     14
#define TUNER_THEMIC4006FH5 15
#define TUNER_MAX           64

typedef struct {
   char*       name;        /* tuner name */
   UCHAR       type;        /* tuner code */
   UCHAR       TunerCB1[4]; //1 control byte. pll control. lo,med.hi,fm band
   UCHAR       TunerCB2[4]; //1 control byte. band switching. lo,med.hi,fm band
                            // band (0x00=NoBand).
   ULONG       BandLim[8];  //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
} TUNERPH;
typedef TUNERPH *PTUNERPH;

/* card code*/
#define CARD_UNKNOWN            0
#define CARD_AVERTVP98          1
#define CARD_KWORLD             2
#define CARD_AVERTVP            3
#define CARD_HAUPPAGEWCAST      4
#define CARD_FLYVIDEO98FM       5
#define CARD_LIFETEC9415        6
#define CARD_MAX                7

/* clock source */
#define CLOCK_NONE          0
#define CLOCK_XTAL0         1
#define CLOCK_XTAL1         2
#define CLOCK_PLLFROMNTSC0  3
#define CLOCK_PLLFROMPAL0   4

/* mixer def */
#define AMUX_TUNER          0
#define AMUX_EXT            1
#define AMUX_INT            2
#define AMUX_MUTE           3

typedef struct {
    UINT            card_id;      /* card id (from #define's) */
    char*           name;         /* name */
    PTUNERPH        ptuner;       /* pointer to Tuner details */
    UCHAR           tunerAddr;    /* Tuner i2c address */ //0x00 - not present
    UCHAR           dbxAddr;      /* Has DBX chip? */
    UCHAR           mspAddr;      /* Has msp3400c chip? */
    UCHAR           dplAddr;      /* Has dpl3518a chip? */
    UCHAR           eepromAddr;   /* eeprom addr */
    UCHAR           eepromSize;   /* bytes / EEPROMBLOCKSIZE */
    UCHAR           palclock;     /* PAL/Secam clock source */
    UCHAR           ntscclock;    /* NTSC clock source */
    UINT            audiomuxs[5]; /* tuner, ext (line-in) */
                                  /* int/unused (radio) */
                                  /* mute, present */
    UINT            gpiomask;     /* GPIO mask for audio mux */
    UINT            rcmask;       /* Remote Control on GPIO mask */
} CARDPH;
typedef CARDPH *PCARDPH;

/* Various defines */
#define HAUP_REMOTE_INT_WADDR   0x30
#define HAUP_REMOTE_INT_RADDR   0x31

#define HAUP_REMOTE_EXT_WADDR   0x34
#define HAUP_REMOTE_EXT_RADDR   0x35

/* address of BTSC/SAP decoder chip */
#define TDA9850_WADDR           0xb6
#define TDA9850_RADDR           0xb7

/* address of MSP3400C chip */
#define MSP3400C_WADDR          0x80
#define MSP3400C_RADDR          0x81

/* address of DPL3518A chip */
#define DPL3518A_WADDR          0x84
#define DPL3518A_RADDR          0x85

/* EEProm (128 * 8) on an STB card */
#define X24C01_WADDR            0xae
#define X24C01_RADDR            0xaf


/* EEProm (256 * 8) on a Hauppauge card */
/* and on most BT878s cards to store the sub-system vendor id */
#define PFC8582_WADDR           0xa0
#define PFC8582_RADDR		0xa1

extern TUNERPH tuners[];
extern CARDPH cards[];

#if __cplusplus
}
#endif


#endif  /*__PHCARD_H*/

