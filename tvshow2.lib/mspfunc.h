/*
** Module   :MSPFUNC.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Wed  23/07/2003 Created
**
*/
#ifndef __MSPFUNC_H
#define __MSPFUNC_H


#if __cplusplus
extern "C" {
#endif

#include "card.h"

void MSPInit(PTVCARD pbttv,UCHAR pin);
void MSPReset(PTVCARD pbttv, UCHAR i2caddr);
void MSPReadId(PTVCARD pbttv,UCHAR addr);
UINT MSPRead(PTVCARD pbttv, UCHAR i2caddr, UCHAR dev, UINT addr);
void MSPWrite(PTVCARD pbttv, UCHAR i2caddr, UCHAR dev, UINT addr, UINT data);
#define VOL_MUTE 0
#define VOL_FASTMUTE 0x0FFE
#define CLIP_VOLUME     0
#define CLIP_TONE       1
#define CLIP_COMPROMISE 2
APIRET MSPSetLoudspeakerVolume(PTVCARD pbttv, UINT volume, UCHAR clip);
#define LINEAR       0
#define LOGARITHMIC  1
APIRET MSPSetLoudspeakerBalance(PTVCARD pbttv, UINT balance, UCHAR mode);
APIRET MSPSetLoudspeakerBass(PTVCARD pbttv, UINT bass);
APIRET MSPSetLoudspeakerTreble(PTVCARD pbttv, UINT treble);
#define BASS_NORMAL  0
#define BASS_SUPER   4
APIRET MSPSetLoudspeakerLoudness(PTVCARD pbttv, UINT loudness, UCHAR mode);
#define SPATIAL_OFF  0
#define SPATIAL_A    0
#define SPATIAL_B    2
#define SPATIAL_MAX  0
#define SPATIAL_23   2
#define SPATIAL_13   4
#define SPATIAL_MIN  6
#define SPATIAL_AUTO 8
APIRET MSPSetLoudspeakerSpatial(PTVCARD pbttv, UINT strength, UCHAR mode, UCHAR coeff);
APIRET MSPSetSCART1Volume(PTVCARD pbttv, UINT volume, UCHAR mode);
#define OUT_LOUDSPEAKER 0
#define OUT_SCART1      1
#define OUT_QPDETECTOR  2
#define S_FMAM          0
#define S_NICAM         1
#define S_SCART         2
#define M_CHAN2         0x00
#define M_CHAN1         0x10
#define M_STEREO        0x20
#define M_MONO          0x30
APIRET MSPSetChannel(PTVCARD pbttv, UCHAR output, UCHAR source, UCHAR matrix);
#define PRESCALE_OFF 0
#define PRESCALE_0   0x19
#define PRESCALE_14  0x7f
APIRET MSPSetSCARTPrescale(PTVCARD pbttv, UINT prescale);
#define FM_NOMATRIX 0
#define FM_GSTEREO  1
#define FM_KSTEREO  2
APIRET MSPSetFMPrescale(PTVCARD pbttv, UINT prescale, UINT matrix);
#define FM_FIXEDD_50     0
#define FM_FIXEDD_75     1
#define FM_FIXEDD_J17    4
#define FM_FIXEDD_OFF    0x3f
#define FM_ADAPTIVED_OFF 0
#define FM_ADAPTIVED_WP1 0x3f
APIRET MSPSetFMDeempasis(PTVCARD pbttv, UINT fixed, UINT adaptive);
#define NICAM_PRESCALE_OFF 0
#define NICAM_PRESCALE_0   0x20
#define NICAM_PRESCALE_12  0x7f
APIRET MSPSetNICAMPrescale(PTVCARD pbttv, UINT prescale);
#define ACB_IN_SC1 0
#define ACB_IN_MONO 1
#define ACB_IN_SC2 2
#define ACB_IN_MUTE 3
#define ACB_OUT_SC2 4
#define ACB_OUT_MONO 5
#define ACB_OUT_SCART1 6
#define ACB_OUT_SC1 7
#define ACB_OUT_MUTE 8
APIRET MSPSetACB(PTVCARD pbttv, UCHAR in, UCHAR out, BOOL dctl0, BOOL dctl1);
#define BEEPER_OFF 0
#define BEEPER_MAX 0x7f
#define BEEPER_16HZ 1
#define BEEPER_1KHZ 0x40
#define BEEPER_4KHZ 0xFF
APIRET MSPSetBeeper(PTVCARD pbttv, UINT volume, UINT freq);
#define IDENT_BG 0
#define IDENT_M 1
#define IDENT_RESET 0x3f
APIRET MSPSetIdent(PTVCARD pbttv, UINT ident);
#define FM_DC_NOTCH_ON 0
#define FM_DC_NOTCH_OFF 0x3f
APIRET MSPSetFMDCNotch(PTVCARD pbttv, UINT notch);
#define AVC_OFF 0
#define AVC_ON  8
#define AVC_DECAY_8S 8
#define AVC_DECAY_4S 4
#define AVC_DECAY_2S 2
#define AVC_DECAY_20MS 1
APIRET MSPSetAVC(PTVCARD pbttv, UCHAR mode, UCHAR decay);
#define FM_STEREO_MONO 0
#define FM_STEREO_STEREO 1
#define FM_STEREO_BILINGUAL 2
UCHAR MSPReadStereoDetection(PTVCARD pbttv);
#define QPD_LEFT 0
#define QPD_RIGHT 1
UINT MSPReadQPDetector(PTVCARD pbttv, UCHAR channel);
#define DCLEVEL_FM1 0
#define DCLEVEL_FM2 1
UINT MSPReadDCLevel(PTVCARD pbttv, UCHAR channel);

#define AUDIO_AUTODETECT 0x0001
#define AUDIO_M_Dual_FM   0x0002
#define AUDIO_BG_Dual_FM  0x0003
#define AUDIO_DK1_Dual_FM 0x0004
#define AUDIO_DK2_Dual_FM 0x0005
#define AUDIO_BG_NICAM_FM 0x0008
#define AUDIO_L_NICAM_AM  0x0009
#define AUDIO_I_NICAM_FM  0x000A
#define AUDIO_DK_NICAM_FM 0x000B
APIRET MSPSetAudioMode(PTVCARD pbttv, ULONG mode);
#define NICAM_ONLY 0
#define NICAM_FM_AUTO 1
#define FM_ONLY 0x801
APIRET MSPSetNICAMFMMode(PTVCARD pbttv, ULONG mode);
#define DETECT_WORKING 0x07ff
#define DETECT_NOTDETECTED 0
#define DETECT_M_DUAL_FM   2
#define DETECT_BG_DUAL_FM  3
#define DETECT_BG_NICAM    8
#define DETECT_L_DK_NICAM  9
#define DETECT_I_NICAM     0x0a
UINT MSPReadAudioStatus(PTVCARD pbttv);
UINT MSPReadCADBits(PTVCARD pbttv);
UINT MSPReadADDBits(PTVCARD pbttv);
UINT MSPReadCIBBits(PTVCARD pbttv);
UINT MSPReadErrorRate(PTVCARD pbttv);




#if __cplusplus
}
#endif

#endif  /*__MSPFUNC_H*/

