/*
** Module   :BTFUNC.H
** Abstract :bt8x8 functions
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  26/02/2002 Created
**
*/


#ifndef __BTFUNC_H
#define __BTFUNC_H

#if __cplusplus
extern "C" {
#endif

#include "card.h"
#include "memory.h"

//extern volatile UCHAR *BTBaseComAddr;
APIRET BTFindCard(PTVCARD pbttv);
APIRET BTAllocBuffers(PTVCARD pbttv, UINT width, UINT height, UCHAR bpp);
APIRET BTFreeBuffers(PTVCARD pbttv);
APIRET BTFreeCard(PTVCARD pbttv);
ULONG BTReadRegister(PTVCARD pbttv, USHORT btreg, UCHAR size, APIRET rc);
ULONG BTWriteRegister(PTVCARD pbttv, USHORT btreg, ULONG value, UCHAR size);
//field - 0 all, 1 odd, 2 even
ULONG BTStartCapture(PTVCARD pbttv, ULONG risc_start, BOOL dithering, BOOL vbi, UCHAR field);
ULONG BTStopCapture(PTVCARD pbttv, BOOL vbi, UCHAR field);
ULONG BTSetColorFormat(PTVCARD pbttv, UCHAR color_fmt, UCHAR field);
void  BTReset(PTVCARD pbttv);
//1 disable interrupt
void  BTDisableInt(PTVCARD pbttv, ULONG int_mask);
APIRET BTSelectInput(PTVCARD pbttv, UCHAR input);
APIRET BTSelectColorSystem(PTVCARD pbttv, UCHAR colorsystem);
// brightness - signed byte, 0x00 - 0, 0x7f +50%, 0x80 -50%, 0xff -0.39%
void   BTSetBrightness(PTVCARD pbttv, UCHAR brightness);
UCHAR  BTGetBrightness(PTVCARD pbttv);
// contract - 9bit. 0x00 - 0%, 0x1ff - 236.57%, default 0xD8 - 100%
APIRET BTSetContrast(PTVCARD pbttv, ULONG contrast);
ULONG  BTGetContrast(PTVCARD pbttv);
// saturation - 9bit, 0 - 0%, 511 (0x1ff) - max
APIRET BTSetSaturation(PTVCARD pbttv, USHORT saturation);
USHORT BTGetSaturation(PTVCARD pbttv);
void BTColorBar(PTVCARD pbttv, BOOL flag); //true - enabled
void BTSetADELAY(PTVCARD pbttv, UCHAR adelay);
UCHAR BTGetADELAY(PTVCARD pbttv);
void BTSetBDELAY(PTVCARD pbttv, UCHAR bdelay);
UCHAR BTGetBDELAY(PTVCARD pbttv);
void BTEnablePLL(PTVCARD pbttv);
void BTDisablePLL(PTVCARD pbttv);
void BTSetHSCALE(PTVCARD pbttv, USHORT hscale);
void BTSetVSCALE(PTVCARD pbttv, USHORT vscale);
void BTSetHDELAY(PTVCARD pbttv, USHORT hdelay);
void BTSetVDELAY(PTVCARD pbttv, USHORT vdelay);
void BTSetHACTIVE(PTVCARD pbttv, USHORT hactive);
void BTSetVACTIVE(PTVCARD pbttv, USHORT vactive);
void BTColorClockPAL(PTVCARD pbttv);
void BTColorClockNTSC(PTVCARD pbttv);
void BTMakeProgram(PTVCARD pbttv,ULONG startaddrphys,char * startaddrvirt,ULONG startaddrbufphys,UINT xpix,UINT ypix,UINT bpp);
void BTWaitFrame(PTVCARD pbttv);
void BTMakeProgram2(PTVCARD pbttv,ULONG startaddrphys,char * startaddrvirt,ULONG startaddrbufphys,UINT xpix,UINT ypix,UINT bpp);
void BTMakeProgramFrame(PTVCARD pbttv);
void BTCopyDualFrame(PTVCARD pbttv,PVOID pBuffer);
void BTCopySingleFrame(PTVCARD pbttv,PVOID pBuffer,BOOL bEven);
void BTRiscDump(ULONG phys,char* virt);
#if __cplusplus
}
#endif

#endif  /*__BTFUNC_H*/

