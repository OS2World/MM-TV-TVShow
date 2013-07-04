/*
** Module   :BTAUDIO.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  28/01/2003 Created
**
*/
#include "tvaudio.h"
#include "btfuncs.h"
#include "bti2cs.h"
#include "bti2c.h"
#include "btregs.h"
#include "mspfunc.h"

void TVMuxControl(PTVCARD pbttv,UCHAR mode)
{
// classic simple mux
   BTWriteRegister(pbttv,GPIO_DATA,pbttv->pcard->audiomuxs[mode],4);
   //msp34 mute
//   msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0000, 0x0000); /* volume to MUTE */
   //msp34 tv
 //  msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0000, 0x7300); /* 0 db volume */
 //  if (bktr->msp_source_selected != 0) msp_autodetect(bktr);  /* setup TV audio mode */
  // bktr->msp_source_selected = 0;
   //if(bktr->audio_mux_select == 1) { /* Line In */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0000, 0x7300); /* 0 db volume */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x000d, 0x1900); /* scart prescale */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0008, 0x0220); /* SCART | STEREO */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0013, 0x0000); /* DSP In = SC1_IN_L/R */
//      bktr->msp_source_selected = 1;
//    }
//   if(bktr->audio_mux_select == 2) { /* FM Radio */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0000, 0x7300); /* 0 db volume */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x000d, 0x1900); /* scart prescale */
///      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0008, 0x0220); /* SCART | STEREO */
//      msp_dpl_write(bktr, bktr->msp_addr, 0x12, 0x0013, 0x0200); /* DSP In = SC2_IN_L/R */
//      bktr->msp_source_selected = 2;
//         }
}
void TVSetAudioSystem(PTVCARD pbttv,UCHAR system)
{
    if (system>=AUDIO_BG_NICAM_FM) //nicam
    {
        MSPSetChannel(pbttv, OUT_LOUDSPEAKER,S_NICAM,M_STEREO);
        MSPSetNICAMPrescale(pbttv, 0x5a);

    }
    else
        MSPSetChannel(pbttv, OUT_LOUDSPEAKER,S_FMAM,M_MONO);
    MSPSetAudioMode(pbttv, system);
    pbttv->audiosystem=system;
}

