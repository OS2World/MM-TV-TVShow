/*
** Module   :VIDEOCFG.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Sat  11/01/2003 Created
**
*/

#include "phcard.h"
#include "card.h"
#include "cfgfile.h"
#include "videocfg.h"


void TVParseConfig(PCFGDATA pconfig)
{
    Variable Vars[]=
    {
        {"video.windowwidth" , sizeof(pconfig->windowwidth) , &pconfig->windowwidth    , Pdword },
        {"video.windowheight", sizeof(pconfig->windowheight), &pconfig->windowheight   , Pdword },
        {"video.videowidth" , sizeof(pconfig->videowidth),  &pconfig->videowidth    , Pdword },
        {"video.videoheight", sizeof(pconfig->videoheight), &pconfig->videoheight   , Pdword },
        {"video.connector"  , sizeof(pconfig->connector)  , &pconfig->connector     , Pshort },
        {"video.blitter"    , sizeof(pconfig->blittermode), &pconfig->blittermode   , Pshort },
        {"video.deinterlace", sizeof(pconfig->deinterlace), &pconfig->deinterlace   , Pshort },
        {"video.colorkey"   , sizeof(pconfig->colorkey)   , &pconfig->colorkey      , Pdword },
        {"video.colorsystem", sizeof(pconfig->colorsystem), &pconfig->colorsystem   , Pshort },
        {"video.contrast"   , sizeof(pconfig->contrast)   , &pconfig->contrast      , Pshort },
        {"video.brightness" , sizeof(pconfig->brightness) , &pconfig->brightness    , Pshort },
        {"video.saturation" , sizeof(pconfig->saturation) , &pconfig->saturation    , Pshort },
        {"video.volume"     , sizeof(pconfig->volume)     , &pconfig->volume        , Pshort },
        {"video.iscolor"    , sizeof(pconfig->iscolor)    , &pconfig->iscolor       , Pbool  },
        {"card.cardcode"    , sizeof(pconfig->cardtype)   , &pconfig->cardtype      , Pshort },
        {"card.tunercode"   , sizeof(pconfig->tunertype)  , &pconfig->tunertype     , Pshort },
        {"usercard.name"    , sizeof(cards[CARD_UNKNOWN].name), &cards[CARD_UNKNOWN].name, Pstr},
//        {"usercard.tuner",sizeof(cards[CARD_UNKNOWN].tuner), &cards[CARD_UNKNOWN].tuner, Pword},
//        {"usercard.tuner",sizeof(usertunertype), &usertunertype, Pword},
        {"usercard.tuneraddr",sizeof(cards[CARD_UNKNOWN].tunerAddr), &cards[CARD_UNKNOWN].tunerAddr, Pword},
        {"usercard.mspaddr",sizeof(cards[CARD_UNKNOWN].mspAddr), &cards[CARD_UNKNOWN].mspAddr, Pword},
        {"usercard.palclock",sizeof(cards[CARD_UNKNOWN].palclock), &cards[CARD_UNKNOWN].palclock, Pword},
        {"usercard.ntscclock",sizeof(cards[CARD_UNKNOWN].ntscclock), &cards[CARD_UNKNOWN].ntscclock, Pword},
        {"usercard.audiomuxtuner",sizeof(cards[CARD_UNKNOWN].audiomuxs[AMUX_TUNER]), &cards[CARD_UNKNOWN].audiomuxs[AMUX_TUNER], Pword},
        {"usercard.audiomuxext",sizeof(cards[CARD_UNKNOWN].audiomuxs[AMUX_EXT]), &cards[CARD_UNKNOWN].audiomuxs[AMUX_EXT], Pword},
        {"usercard.audiomuxint",sizeof(cards[CARD_UNKNOWN].audiomuxs[AMUX_INT]), &cards[CARD_UNKNOWN].audiomuxs[AMUX_INT], Pword},
        {"usercard.audiomuxmute",sizeof(cards[CARD_UNKNOWN].audiomuxs[AMUX_MUTE]), &cards[CARD_UNKNOWN].audiomuxs[AMUX_MUTE], Pword},
        {"usercard.muxgpiomask",sizeof(cards[CARD_UNKNOWN].gpiomask), &cards[CARD_UNKNOWN].gpiomask, Pword},
        {"usertuner.name",sizeof(tuners[TUNER_UNKNOWN].name), &tuners[TUNER_UNKNOWN].name, Pstr},
        {"usertuner.cb1lo",sizeof(tuners[TUNER_UNKNOWN].TunerCB1[LoBand]), &tuners[TUNER_UNKNOWN].TunerCB1[LoBand], Pword},
        {"usertuner.cb1med",sizeof(tuners[TUNER_UNKNOWN].TunerCB1[MedBand]), &tuners[TUNER_UNKNOWN].TunerCB1[MedBand], Pword},
        {"usertuner.cb1hi",sizeof(tuners[TUNER_UNKNOWN].TunerCB1[HiBand]), &tuners[TUNER_UNKNOWN].TunerCB1[HiBand], Pword},
        {"usertuner.cb1fm",sizeof(tuners[TUNER_UNKNOWN].TunerCB1[FMBand]), &tuners[TUNER_UNKNOWN].TunerCB1[FMBand], Pword},
        {"usertuner.cb2lo",sizeof(tuners[TUNER_UNKNOWN].TunerCB2[LoBand]), &tuners[TUNER_UNKNOWN].TunerCB2[LoBand], Pword},
        {"usertuner.cb2med",sizeof(tuners[TUNER_UNKNOWN].TunerCB2[MedBand]), &tuners[TUNER_UNKNOWN].TunerCB2[MedBand], Pword},
        {"usertuner.cb2hi",sizeof(tuners[TUNER_UNKNOWN].TunerCB2[HiBand]), &tuners[TUNER_UNKNOWN].TunerCB2[HiBand], Pword},
        {"usertuner.cb2fm",sizeof(tuners[TUNER_UNKNOWN].TunerCB2[FMBand]), &tuners[TUNER_UNKNOWN].TunerCB2[FMBand], Pword},
        {"usertuner.BLlolo",sizeof(tuners[TUNER_UNKNOWN].BandLim[0]), &tuners[TUNER_UNKNOWN].BandLim[0], Plong},
        {"usertuner.BLlohi",sizeof(tuners[TUNER_UNKNOWN].BandLim[1]), &tuners[TUNER_UNKNOWN].BandLim[1], Plong},
        {"usertuner.BLmedlo",sizeof(tuners[TUNER_UNKNOWN].BandLim[2]), &tuners[TUNER_UNKNOWN].BandLim[2], Plong},
        {"usertuner.BLmedhi",sizeof(tuners[TUNER_UNKNOWN].BandLim[3]), &tuners[TUNER_UNKNOWN].BandLim[3], Plong},
        {"usertuner.BLhilo",sizeof(tuners[TUNER_UNKNOWN].BandLim[4]), &tuners[TUNER_UNKNOWN].BandLim[4], Plong},
        {"usertuner.BLhihi",sizeof(tuners[TUNER_UNKNOWN].BandLim[5]), &tuners[TUNER_UNKNOWN].BandLim[5], Plong},
        {"usertuner.BLfmlo",sizeof(tuners[TUNER_UNKNOWN].BandLim[6]), &tuners[TUNER_UNKNOWN].BandLim[6], Plong},
        {"usertuner.BLfmhi",sizeof(tuners[TUNER_UNKNOWN].BandLim[7]), &tuners[TUNER_UNKNOWN].BandLim[7], Plong},
	{"logger.logname",sizeof(pconfig->logname),&pconfig->logname,Pstr},
	{"logger.loglevel",sizeof(pconfig->loglevel),&pconfig->loglevel,Pshort},
        {0}
    };

    LoadConfig("video.cfg", Vars);
}
