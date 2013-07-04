/*
** Module   :PHCARD.C
** Abstract :Cards details
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  15/10/2002 Created
**
*/

#include "phcard.h"

/* tuners array */
TUNERPH tuners[] = {
    {  "Unknown", /* tuner name */
        TUNER_UNKNOWN, /* tuner code */
        { 0,0,0,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0,0,0,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {0,0,0,0,0,0,0,0} //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
    },
    {  "Philips fm1246", /* tuner name */ /* PAL I */
        TUNER_PHILIPS1246, /* tuner code */
        { 0xce,0xce,0xce,0x88}, //1 control byte. pll control. lo,med.hi,fm band
        { 0xa0,0x90,0x30,0xa4}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "TCL2002MB", /* tuner name */
        TUNER_TCL2002MB, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med,hi,fm band
        { 1,2,8,0}, //2 control byte. band switching. lo,med,hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
            //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Philips fm1236", /* tuner name */ /*NTSC M,N, FM*/
        TUNER_PHILIPS1236, /* tuner code */
        { 0xce,0xce,0xce,0x88}, //1 control byte. pll control. lo,med.hi,fm band
        { 0xa0,0x90,0x30,0xa4}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {55250000,157250000,163250000,451250000,457250000,801250000,76000000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Philips fi1216", /* tuner name */ /* PAL B/G */
        TUNER_PHILIPS1216, /* tuner code */
        { 0xce,0xce,0xce,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0xa0,0x90,0x30,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {48250000,168250000,175250000,447250000,455250000,855250000,0,0}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Themic 4002FH5", /* tuner name */ /* PAL  */
        TUNER_THEMIC4002FH5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x02,0x04,0x01,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Themic 4032FY5", /* tuner name */ /* NTSC  */
        TUNER_THEMIC4032FY5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x02,0x04,0x01,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
        {  "Themic 4062FY5", /* tuner name */ /* PAL  */
        TUNER_THEMIC4062FY5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x02,0x04,0x01,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Themic 4036FY5", /* tuner name */ /* NTSC  */
        TUNER_THEMIC4036FY5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x02,0x04,0x01,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS HSBH1", /* tuner name */ /* NTSC  */
        TUNER_ALPSHSBH1, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x01,0x02,0x08,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS TSBE1", /* tuner name */ /* PAL */
        TUNER_ALPSTSBE1, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x01,0x02,0x08,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS TSBB5", /* tuner name */ /* PAL */
        TUNER_ALPSTSBB5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x01,0x02,0x08,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS TSBE5", /* tuner name */ /* PAL */
        TUNER_ALPSTSBE5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x01,0x02,0x08,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS TSBC5", /* tuner name */ /* PAL */
        TUNER_ALPSTSBC5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x01,0x02,0x08,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "ALPS TSCH6", /* tuner name */ /* NTSC */
        TUNER_ALPSTSCH6, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0x14,0x12,0x11,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },
    {  "Themic 4006FH5", /* tuner name */ /* PAL */
        TUNER_THEMIC4006FH5, /* tuner code */
        { 0x8e,0x8e,0x8e,0}, //1 control byte. pll control. lo,med.hi,fm band
        { 0xa0,0x90,0x30,0}, //2 control byte. band switching. lo,med.hi,fm band
                                 // band (0x00=NoBand).
        {45750000,168250000,175250000,447250000,455250000,855250000,87500000,108000000}
        //band limits lolo,lohi,medlo,medhi,hilo,hihi,fmlo,fmhi
                          //Hz
    },


};

/* cards array */
CARDPH cards[] = {

    {  CARD_UNKNOWN,       /* card id (from #define's) */
      "Unknown",           /* name */
       &tuners[TUNER_UNKNOWN],/* Tuner details */
       0,                  /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_NONE,         /* PAL/Secam clock source */
       CLOCK_NONE,         /* NTSC clock source */

       { 0, 0, 0, 0, 0 },  /* tuner, ext (line-in) */
                           /* int/unused (radio) */
                           /* mute, present */
       0,                  /* GPIO mask for audio mux */
       0 },                /* Remote Control on GPIO mask */

    {  CARD_AVERTVP98,     /* card id (from #define's) */
      "AverMedia TVPhone98",    /* name */
       &tuners[TUNER_PHILIPS1246],               /* Tuner details */
       0xc2,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_PLLFROMNTSC0, /* PAL/Secam clock source */
       CLOCK_XTAL0,        /* NTSC clock source */
       { 0x0c, 8, 4, 0, 1 },  /* tuner, ext (line-in) */
                           /* int/unused (radio) */
                           /* mute, present */
       0x0f,               /* GPIO mask for audio mux */
       0xfff000},          /* Remote Control on GPIO mask */
    {  CARD_KWORLD,        /* card id (from #define's) */
      "KWORLD TV878",           /* name */
       &tuners[TUNER_TCL2002MB],               /* Tuner details */
       0xc0,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_PLLFROMNTSC0,     /* PAL/Secam clock source */
       CLOCK_XTAL0,        /* NTSC clock source */
       { 0, 02, 01, 4, 1 },  /* tuner, ext (line-in) */
                           /* int/unused (radio) */
                           /* mute, present */
       0x0f,                /* GPIO mask for audio mux */
       0xff00},          /* Remote Control on GPIO mask */
    {  CARD_AVERTVP,     /* card id (from #define's) */
      "AverMedia TVPhone",    /* name */
//       TUNER_PHILIPS1246,  /* tuner id */
       &tuners[TUNER_PHILIPS1246],               /* Tuner details */
       0xc2,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_XTAL0,     /* PAL/Secam clock source */
       CLOCK_PLLFROMPAL0,        /* NTSC clock source */
       { 0x0c, 8, 4, 0, 1 },  /* tuner, ext (line-in) */
                           /* int/unused (radio) */
                           /* mute, present */
       0x0f,               /* GPIO mask for audio mux */
       0xfff000},           /* Remote Control on GPIO mask */
    {  CARD_HAUPPAGEWCAST,     /* card id (from #define's) */
      "Hauppage WinCast PCI",    /* name */
       &tuners[TUNER_PHILIPS1246],               /* Tuner details */
       0xc2,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0x80,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_PLLFROMNTSC0,     /* PAL/Secam clock source */
       CLOCK_XTAL0,         /* NTSC clock source */
       { 0x20, 0x21, 0x22, 0x23, 1 },  /* tuner, ext (line-in) */
                            /* int/unused (radio) */
                            /* mute, present */
       0x23 ,               /* GPIO mask for audio mux */
       0x0},           /* Remote Control on GPIO mask */
    {  CARD_FLYVIDEO98FM,     /* card id (from #define's) */
      "FlyVideo98FM",    /* name */
       &tuners[TUNER_PHILIPS1246],               /* Tuner details */
       0xc2,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_PLLFROMNTSC0, /* PAL/Secam clock source */
       CLOCK_XTAL0,        /* NTSC clock source */
       { 0, 0x800, 0x400, 0x8dff00, 1 },  /* tuner, ext (line-in) */
                           /* int/unused (radio) */
                           /* mute, present */
       0x8dff00,               /* GPIO mask for audio mux */
       0xfff000},          /* Remote Control on GPIO mask */
    {  CARD_LIFETEC9415,     /* card id (from #define's) */
      "LifeTec 9415",    /* name */
       &tuners[TUNER_PHILIPS1246],               /* Tuner details */
       0xc2,               /* Tuner i2c address */ //0x00 - not present
       0,                  /* Has DBX chip? */
       0,                  /* Has msp3400c chip? */
       0,                  /* Has dpl3518a chip? */
       0,                  // eeprom addr
       0,                  /* bytes / EEPROMBLOCKSIZE */
       CLOCK_PLLFROMNTSC0, /* PAL/Secam clock source */
       CLOCK_XTAL0,        /* NTSC clock source */
       { 0, 0x1000, 0x0800, 0x18e0, 1 },  /* tuner, ext (line-in) */
                                  /* int/unused (radio) */
                           /* mute, present */
                 /* 0x0000: Tuner normal stereo
                  0x0080: Tuner A2 SAP (second audio program = Zweikanalton)
                  0x0880: Tuner A2 stereo */
       0x018e0,               /* GPIO mask for audio mux */
       0xfff000},          /* Remote Control on GPIO mask */

};


