/*
** Module   :BTFUNCS.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  26/02/2002 Created
**
*/
#include "btregs.h"
#include <stdlib.h>
// #include "cards.h"
#include "btrisc.h"
#include "btfuncs.h"
#include "pcifuncs.h"
#include "logger.h"
//#include "MAPHYMEM.h"
#include "pciregs.h"
#include "driver.h"

//volatile UCHAR *BTBaseComAddr;
//APIRET rc;
#define MAX_SINGLE_TRANSFER 64
//64
APIRET BTFindCard(PTVCARD pbttv)
{
    int devnum;
    const struct
    {
        USHORT vendor;
        USHORT device;
        char *name;
        UINT chip;
    } knowndevices[] =
    {
        {0x109E, 0x350, "bt848",0x848},
        {0x109E, 0x351, "bt849",0x849},
        {0x109E, 0x36E, "bt878",0x878},
        {0x109E, 0x36F, "bt879",0x879}
    };

    ULONG comaddrphys, comaddrlen, val, latency;
    APIRET rc;

    rc = InitOEMHlp();
    if (rc) {dprintf(LOG_ERROR,"Can not access OEMHLP\n");return 1;}
    dprintf(LOG_MESSAGE,"Looking for device\n");
    for (devnum = 0; devnum < ASZ(knowndevices); devnum++)
        if (NO_ERROR == PCIUseDevice(knowndevices[devnum].vendor, knowndevices[devnum].device, 0))
            break;

    if (devnum == ASZ(knowndevices)){dprintf(LOG_ERROR,"No known devices found\n");return 2;}
    dprintf(LOG_MESSAGE,"Found chip %s\n", knowndevices[devnum].name);
//
//    * Enable bus mastering and Memory Mapped device
//
    val = PCIReadRegister(PCIR_COMMAND, 4 , rc);
    if (rc)
    {
       dprintf(LOG_ERROR,"Can not read PCI command register. rc=%u\n",rc);
       return 3;
    }
    val |= (PCIM_CMD_MEMEN|PCIM_CMD_BUSMASTEREN);
    rc = PCIWriteRegister(PCIR_COMMAND, val, 4);
    if (rc)
    {
       dprintf(LOG_ERROR,"Can not write PCI command register. rc=%u\n",rc);
       return 3;
    }
    dprintf(LOG_MESSAGE,"Looking for command memory address\n");
    comaddrphys = PCIReadRegister(0x10, 4 , rc);
    if (rc) {dprintf(LOG_ERROR,"Can not access PCI space\n");return 3;}
    comaddrlen = comaddrphys & 0xfff;
    if (comaddrlen != 0x8)
    {
       dprintf(LOG_ERROR," Invalid command memory block length (%u != 8)\n", comaddrlen);
       return 4;
    }
    comaddrphys &= ~0xfff;
    dprintf(LOG_INFO,"Physical address is %08x\n", comaddrphys);
//
// * PCI latency timer.  32 is a good value for 4 bus mastering slots, if
// * you have more than four, then 16 would probably be a better value.
//
#ifndef BROOKTREE_DEF_LATENCY_VALUE
#define BROOKTREE_DEF_LATENCY_VALUE     16
#endif
    latency = PCIReadRegister(PCIR_LATTIMER, 1, rc);
    if (rc)
    {
        dprintf(LOG_ERROR,"Can not read PCI latency register. rc=%u\n",rc);
        return 3;
    }
    if (latency)
        dprintf(LOG_INFO,"PCI bus latency is %d.\n", (int) latency);
      else
        dprintf(LOG_INFO,"PCI bus latency was 0 changing to %d.\n", (int) latency);
    if (!latency)
    {
       latency = BROOKTREE_DEF_LATENCY_VALUE;
       rc=PCIWriteRegister(PCIR_LATTIMER, latency, 1);
       if (rc)
        {
            dprintf(LOG_ERROR,"Can not write PCI latency register. rc=%u\n",rc);
            return 3;
        }
    }
    pbttv->tvchip.chipcode=knowndevices[devnum].chip;
    pbttv->tvchip.vendor=knowndevices[devnum].vendor;
    pbttv->tvchip.device=knowndevices[devnum].device;
    pbttv->tvchip.latency=latency;
    pbttv->tvchip.IRQLine=PCIReadRegister(PCIR_INTLINE, 1, rc);
    if (rc)
    {
        dprintf(LOG_ERROR,"Can not read PCI Interrupt line register. rc=%u\n",rc);
        return 3;
    }
    pbttv->tvchip.sybsystemID=0;
    pbttv->tvchip.sybsystemvendorID=0;
    if (pbttv->tvchip.chipcode>849)
    {
        pbttv->tvchip.sybsystemID=PCIReadRegister(PCIR_SUBDEV_0, 2, rc);
        if (rc)
        {
            dprintf(LOG_ERROR,"Can not read PCI SybSystemID register. rc=%u\n",rc);
            return 3;
        }
        pbttv->tvchip.sybsystemvendorID=PCIReadRegister(PCIR_SUBVEND_0, 2, rc);
        if (rc)
        {
            dprintf(LOG_ERROR,"Can not read PCI SybSystemVendorID register. rc=%u\n",rc);
            return 3;
        }
    }



    rc = CloseOEMHlp();
    if (rc)
    {
       dprintf(LOG_ERROR," Can not close OEMHLP\n");
       return 1;
    }

    dprintf(LOG_INFO,"found tv chip %x \n",pbttv->tvchip.chipcode);
    dprintf(LOG_INFO,"vendor ID %04x \n",pbttv->tvchip.vendor);
    dprintf(LOG_INFO,"device ID %04x \n",pbttv->tvchip.device);
    dprintf(LOG_INFO,"sybsystem ID %04x \n",pbttv->tvchip.sybsystemID);
    dprintf(LOG_INFO,"sybsystem vendor ID %04x \n",pbttv->tvchip.sybsystemvendorID);
    dprintf(LOG_INFO,"latency %d \n",pbttv->tvchip.latency);
    dprintf(LOG_INFO,"IRQ Line %X \n",pbttv->tvchip.IRQLine);

//    rc=DosMapPhysMem((PPVOID)&pbttv->pbtbasecomaddr,4096,comaddrphys);
    rc=BTOpenDriver(&pbttv->hDriver);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"OpenDriver() return code %u \n", rc); return 6;};
    rc=BTMapMem(pbttv->hDriver,comaddrphys,4096,(PPVOID)&pbttv->pbtbasecomaddr);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"MapPhysMem() return code %u \n", rc); return 6;};
    return 0;
}
APIRET BTAllocBuffers(PTVCARD pbttv, UINT width, UINT height, UCHAR bpp)
{
    APIRET rc;
	#define SAFETY_SPACE 32768
    const progperline = 64;
	int progneeded;

    progneeded = progperline*height+4*8+8+SAFETY_SPACE;
    // allocate frames
    dprintf(LOG_DEBUG,"Allocate frame buffers \n");
    rc=MemCreateFrame(pbttv,&(pbttv->EvenFrame),width,height/2,bpp);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"CreateFrame even failed, return code %u \n", rc); return 6;};
    rc=MemCreateFrame(pbttv,&(pbttv->OddFrame),width,height/2,bpp);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"CreateFrame odd failed, return code %u \n", rc); return 6;};
// allocate risc
    dprintf(LOG_DEBUG,"AllocMap risc buffer \n");
    ULONG ulphys;
    PVOID pvirt;
    rc=BTAllocMapContMem(pbttv->hDriver,progneeded,&ulphys,(PPVOID)&pvirt);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"BTAllocMapMem failed, return code %u \n", rc); return 6;};
    pbttv->pRiscVirt=pvirt;
    pbttv->ulRiscPhys=ulphys;
    memset(pbttv->pRiscVirt,0,progneeded);
    return NO_ERROR;
}
APIRET BTFreeBuffers(PTVCARD pbttv)
{
    APIRET rc;
    dprintf(LOG_DEBUG,"Free frame buffer\n");
    rc=MemDeleteFrame(pbttv,&pbttv->OddFrame);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"DeleteFrame odd failed, return code %u \n", rc); return 6;};
    rc=MemDeleteFrame(pbttv,&pbttv->EvenFrame);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"DeleteFrame even failed, return code %u \n", rc); return 6;};
    dprintf(LOG_DEBUG,"Free RISC buffer\n");
    BTFreeContMem(pbttv->hDriver,pbttv->ulRiscPhys,pbttv->pRiscVirt);
    return NO_ERROR;
}
APIRET BTFreeCard(PTVCARD pbttv)
{
    APIRET rc;

//    rc=DosUnmapPhysMem((PVOID)pbttv->pbtbasecomaddr);
    rc=BTUnMapMem(pbttv->hDriver,(PVOID)pbttv->pbtbasecomaddr);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"UnMapPhysMem() return code %u \n", rc); return 6;};
    rc=BTCloseDriver(pbttv->hDriver);
    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"CloseDriver() return code %u \n", rc); return 6;};
    return 0;

}
ULONG BTReadRegister(PTVCARD pbttv, USHORT btreg, UCHAR size, APIRET rc)
{
    DosEnterCritSec();
    ULONG retlong;
    volatile UCHAR *PBTByte=(pbttv->pbtbasecomaddr + btreg);
    volatile USHORT *PBTWord=(volatile USHORT *)(pbttv->pbtbasecomaddr + btreg);
    volatile ULONG *PBTDWord=(volatile ULONG *)(pbttv->pbtbasecomaddr + btreg);
    rc=NO_ERROR;
    switch (size)
     {
         case 1:
             retlong=(ULONG)*PBTByte;
             break;
         case 2:
             retlong=(ULONG)*PBTWord;
             break;
         case 4:
             retlong=(ULONG)*PBTDWord;
             break;
         default:
             rc=ERROR_INVALID_FUNCTION;
             retlong=0;
     }
    DosExitCritSec();
    return retlong;
}

ULONG BTWriteRegister(PTVCARD pbttv, USHORT btreg, ULONG value, UCHAR size)
{
    ULONG retlong;
    DosEnterCritSec();
    volatile UCHAR *PBTByte=(pbttv->pbtbasecomaddr + btreg);
    volatile USHORT *PBTWord=(volatile USHORT *)(pbttv->pbtbasecomaddr + btreg);
    volatile ULONG *PBTDWord=(volatile ULONG *)(pbttv->pbtbasecomaddr + btreg);
    switch (size)
     {
         case 1:
            {
                if (value>0xff) {DosExitCritSec();return ERROR_INVALID_DATA;}
                *PBTByte=value;
                DosExitCritSec();
                return NO_ERROR;
            }
         case 2:
            {
                if (value>0xffff) {DosExitCritSec();return ERROR_INVALID_DATA;}
                *PBTWord=value;
            	DosExitCritSec();
                return NO_ERROR;
            }
         case 4:
            {
                *PBTDWord=value;
                DosExitCritSec();
                return NO_ERROR;
            }
         default:
            DosExitCritSec();
            return ERROR_INVALID_FUNCTION;
     }

}
ULONG BTStartCapture(PTVCARD pbttv, ULONG risc_start, BOOL dithering, BOOL vbi, UCHAR field)
{
    UCHAR controlbyte;
    UCHAR controlbyte2;
    APIRET rc;
    controlbyte=BTReadRegister(pbttv, CAP_CTL,1,rc);
    if (!dithering) controlbyte|=BT848_CAP_CTL_DITH_FRAME;
    if (vbi)
    {
        switch (field)
            {
                case 1:
                    controlbyte|=BT848_CAP_CTL_VBI_ODD;
                    break;
                case 2:
                    controlbyte|=BT848_CAP_CTL_VBI_EVEN;
                    break;
                case 0:
                    controlbyte|=BT848_CAP_CTL_VBI_ODD;
                    controlbyte|=BT848_CAP_CTL_VBI_EVEN;
                    break;
                default: return ERROR_INVALID_FUNCTION;
            }
    }
    else
    {
        switch (field)
            {
                case 1:
                    controlbyte|=BT848_CAP_CTL_ODD;
                    break;
                case 2:
                    controlbyte|=BT848_CAP_CTL_EVEN;
                    break;
                case 0:
                    controlbyte|=BT848_CAP_CTL_ODD;
                    controlbyte|=BT848_CAP_CTL_EVEN;
                    break;
                default: return ERROR_INVALID_FUNCTION;
            }
     }
     BTWriteRegister(pbttv, RISC_STRT_ADD, risc_start, 4);//fill program start
     controlbyte2=BTReadRegister(pbttv, GPIO_DMA_CTL,1,rc);
     controlbyte2|=BT848_DMA_CTL_RISC_EN;           //risc enabled
     controlbyte2|=BT848_DMA_CTL_FIFO_EN;           //fifo enabled
     BTWriteRegister(pbttv,GPIO_DMA_CTL,controlbyte2,1);
     BTWriteRegister(pbttv,CAP_CTL,controlbyte,1);        //capture control
     return NO_ERROR;
}
ULONG BTStopCapture(PTVCARD pbttv, BOOL vbi, UCHAR field)
{
    UCHAR controlbyte;
    UCHAR controlbyte2;
    APIRET rc;
    controlbyte=BTReadRegister(pbttv,CAP_CTL,1,rc);
    controlbyte2=BTReadRegister(pbttv,GPIO_DMA_CTL,1,rc);
    if (vbi)
    {
        switch (field)
            {
                case 1:
                    controlbyte&=~BT848_CAP_CTL_VBI_ODD;
                    break;
                case 2:
                    controlbyte&=~BT848_CAP_CTL_VBI_EVEN;
                    break;
                case 0:
                    controlbyte&=~BT848_CAP_CTL_VBI_ODD;
                    controlbyte&=~BT848_CAP_CTL_VBI_EVEN;
                    break;
                default: return ERROR_INVALID_FUNCTION;
            }
    }
    else
    {
        switch (field)
            {
                case 1:
                    controlbyte&=~BT848_CAP_CTL_ODD;
                    break;
                case 2:
                    controlbyte&=~BT848_CAP_CTL_EVEN;
                    break;
                case 0:
                    controlbyte&=~BT848_CAP_CTL_ODD;
                    controlbyte&=~BT848_CAP_CTL_EVEN;
                    break;
                default: return ERROR_INVALID_FUNCTION;
            }
     }

    controlbyte2&=(~BT848_DMA_CTL_RISC_EN);           //risc disabled
    controlbyte2&=(~BT848_DMA_CTL_FIFO_EN);           //fifo disabled
    BTWriteRegister(pbttv,GPIO_DMA_CTL,controlbyte2,1);
    BTWriteRegister(pbttv,RISC_STRT_ADD, 0L, 4);//fill program start
    BTWriteRegister(pbttv,CAP_CTL,controlbyte,1);
    return NO_ERROR;
}
ULONG BTSetColorFormat(PTVCARD pbttv,UCHAR color_fmt, UCHAR field)
{
    UCHAR controlbyte;
    APIRET rc;
    controlbyte=BTReadRegister(pbttv,COLOR_FMT,1,rc);
    switch (field)
    {
         case 1:
            controlbyte&=0xf;
            controlbyte|=(color_fmt<<4);
            break;
         case 2:
            controlbyte&=0xf0;
            controlbyte|=color_fmt;
            break;
         case 0:
            controlbyte=0;
            controlbyte|=color_fmt<<4;
            controlbyte|=color_fmt;
            break;
         default: return ERROR_INVALID_FUNCTION;
    }
    BTWriteRegister(pbttv,COLOR_FMT,controlbyte,1);
    return NO_ERROR;
}
void BTReset(PTVCARD pbttv)
{
    BTWriteRegister(pbttv,SRESET,1,1);
    return;
}
void  BTDisableInt(PTVCARD pbttv,ULONG int_mask)
{
    ULONG int_stat;
    APIRET rc;
    int_stat=BTReadRegister(pbttv,INT_MASK,4,rc);
    int_stat&=(~int_mask);
    BTWriteRegister(pbttv,INT_MASK,int_stat,4); //disable int
    int_stat=BTReadRegister(pbttv,INT_STAT,4,rc);
    int_stat&=(~int_mask);
    BTWriteRegister(pbttv,INT_STAT,~int_stat,4); //clear pending int
}
void  BTEnableInt(PTVCARD pbttv,ULONG int_mask)
{
    ULONG int_stat;
    APIRET rc;
    int_stat=BTReadRegister(pbttv,INT_MASK,4,rc);
    int_stat|=int_mask;
    BTWriteRegister(pbttv,INT_MASK,int_stat,4); //disable int
}

APIRET  BTSelectInput(PTVCARD pbttv, UCHAR input)
{
    UCHAR iformmux;
    APIRET rc;
    iformmux=BTReadRegister(pbttv,IFORM,1,rc);
    iformmux&=(~BT848_IFORM_MUXSEL);
    switch (input)
        {

            case 0:
                iformmux|=BT848_IFORM_M_MUX0;
                break;
            case 1:
                iformmux|=BT848_IFORM_M_MUX1;
                break;
            case 2:
                iformmux|=BT848_IFORM_M_MUX2;
                break;
            case 3:
                iformmux|=BT848_IFORM_M_MUX3;
                break;
            default: return ERROR_INVALID_FUNCTION;
        }
    BTWriteRegister(pbttv,IFORM,iformmux,1);
    return NO_ERROR;
}
APIRET BTSelectColorSystem(PTVCARD pbttv,UCHAR colorsystem)
{
    UCHAR iformmux;
    APIRET rc;
    iformmux=BTReadRegister(pbttv,IFORM,1,rc);
    iformmux&=(~BT878_IFORM_FORMAT);
    switch (colorsystem)
        {

            case BT878_IFORM_F_AUTO:
                break;
            case BT878_IFORM_F_NTSCM:
                iformmux|=BT878_IFORM_F_NTSCM;
                BTColorClockNTSC(pbttv);
                BTSetADELAY(pbttv,0x70);
                BTSetBDELAY(pbttv,0x5d);
                BTSetHSCALE(pbttv,0x02ac);
                BTSetHDELAY(pbttv,0x0078);
                BTSetHACTIVE(pbttv,0x0280);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0016);
                BTSetVACTIVE(pbttv,0x01e0);
                break;
            case BT878_IFORM_F_NTSCJ:
                iformmux|=BT878_IFORM_F_NTSCJ;
                BTColorClockNTSC(pbttv);
                BTSetADELAY(pbttv,0x70);
                BTSetBDELAY(pbttv,0x5d);
                BTSetHSCALE(pbttv,0x02ac);
                BTSetHDELAY(pbttv,0x0078);
                BTSetHACTIVE(pbttv,0x0280);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0016);
                BTSetVACTIVE(pbttv,0x01e0);

                break;
            case BT878_IFORM_F_PAL:
                iformmux|=BT878_IFORM_F_PAL;
                BTColorClockPAL(pbttv);
                BTSetADELAY(pbttv,0x7f);
                BTSetBDELAY(pbttv,0x72);
                BTSetHSCALE(pbttv,0x033c);
                BTSetHDELAY(pbttv,0x009a);
                BTSetHACTIVE(pbttv,0x0300);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0020); //1e
                BTSetVACTIVE(pbttv,0x023f);

                break;
            case BT878_IFORM_F_PALM:
                iformmux|=BT878_IFORM_F_PALM;
                BTColorClockNTSC(pbttv);
                BTSetADELAY(pbttv,0x70);
                BTSetBDELAY(pbttv,0x5d);
                BTSetHSCALE(pbttv,0x02aa);
                BTSetHDELAY(pbttv,0x0078);
                BTSetHACTIVE(pbttv,0x0280);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0016);
                BTSetVACTIVE(pbttv,0x01e0);
                break;
            case BT878_IFORM_F_PALN:
                iformmux|=BT878_IFORM_F_PALN;
                BTColorClockPAL(pbttv);
                BTSetADELAY(pbttv,0x7f);
                BTSetBDELAY(pbttv,0x72);
                BTSetHSCALE(pbttv,0x033c);
                BTSetHDELAY(pbttv,0x009a);
                BTSetHACTIVE(pbttv,0x0300);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0020);
                BTSetVACTIVE(pbttv,0x023f);

                break;
            case BT878_IFORM_F_SECAM:
                iformmux|=BT878_IFORM_F_SECAM;
                BTColorClockPAL(pbttv);
                BTSetADELAY(pbttv,0x7F);
                BTSetBDELAY(pbttv,0xb0);
//                BTSetBDELAY(pbttv,0xa0);
                BTSetHSCALE(pbttv,0x033c);
                BTSetHDELAY(pbttv,0x009a);
                BTSetHACTIVE(pbttv,0x0300);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0020);//15
                BTSetVACTIVE(pbttv,0x023f);

                break;
            case BT878_IFORM_F_PALNC:
                iformmux|=BT878_IFORM_F_PALNC;
                BTColorClockPAL(pbttv);
                BTSetADELAY(pbttv,0x7F);
                BTSetBDELAY(pbttv,0x72);
                BTSetHSCALE(pbttv,0x033c);
                BTSetHDELAY(pbttv,0x009a);
                BTSetHACTIVE(pbttv,0x0300);
                BTSetVSCALE(pbttv,0);
                BTSetVDELAY(pbttv,0x0020);
                BTSetVACTIVE(pbttv,0x023f);


                break;
            default: return ERROR_INVALID_FUNCTION;
        }
    BTWriteRegister(pbttv,IFORM,iformmux,1);
    return NO_ERROR;
}
void   BTSetBrightness(PTVCARD pbttv,UCHAR brightness)
{
   BTWriteRegister(pbttv,BRIGHT,brightness,1);
}
UCHAR  BTGetBrightness(PTVCARD pbttv)
{
   APIRET rc;
   return BTReadRegister(pbttv,BRIGHT,1,rc);
}
APIRET BTSetContrast(PTVCARD pbttv,ULONG contrast)
{
   UCHAR tempbyteE, tempbyteO;
   APIRET rc;
   if (contrast>0x1FF) return ERROR_INVALID_FUNCTION;
   tempbyteE=BTReadRegister(pbttv,E_CONTROL,1,rc);
   tempbyteO=BTReadRegister(pbttv,O_CONTROL,1,rc);
   if (contrast>=0x100)
      {
         BTWriteRegister(pbttv,E_CONTROL,tempbyteE|BT848_E_CONTROL_CON_MSB,1);
         BTWriteRegister(pbttv,O_CONTROL,tempbyteO|BT848_O_CONTROL_CON_MSB,1);
         }
      else {
         BTWriteRegister(pbttv,E_CONTROL,tempbyteE&(~BT848_E_CONTROL_CON_MSB),1);
         BTWriteRegister(pbttv,O_CONTROL,tempbyteO&(~BT848_O_CONTROL_CON_MSB),1);
         }
   BTWriteRegister(pbttv,CONTRAST_LO,(UCHAR)contrast&0xFF,1);
   return NO_ERROR;
}
ULONG  BTGetContrast(PTVCARD pbttv)
{
   UCHAR tempbyte, tempbyte2;
   USHORT tempword;
   APIRET rc;
   tempbyte=BTReadRegister(pbttv,E_CONTROL,1,rc);
   tempbyte2=BTReadRegister(pbttv,CONTRAST_LO,1,rc);
   if (tempbyte&BT848_E_CONTROL_CON_MSB) tempword=0x100+tempbyte2;
      else tempword=tempbyte2;
   return (ULONG)tempword;
}
void BTColorBar(PTVCARD pbttv,BOOL flag)
{
   UCHAR tempbyte;
   APIRET rc;
   tempbyte=BTReadRegister(pbttv,COLOR_CTL,1,rc);
   if (flag) tempbyte|=BT848_COLOR_CTL_COLOR_BARS;
      else tempbyte&=(~BT848_COLOR_CTL_COLOR_BARS);
   BTWriteRegister(pbttv,COLOR_CTL,tempbyte,1);
}
void BTSetADELAY(PTVCARD pbttv,UCHAR adelay)
{
   BTWriteRegister(pbttv,ADELAY,adelay,1);
}
UCHAR BTGetADELAY(PTVCARD pbttv)
{
APIRET rc;
   return BTReadRegister(pbttv,ADELAY,1,rc);
}
void BTSetBDELAY(PTVCARD pbttv,UCHAR bdelay)
{
   BTWriteRegister(pbttv,BDELAY,bdelay,1);
}
UCHAR BTGetBDELAY(PTVCARD pbttv)
{
APIRET rc;
   return BTReadRegister(pbttv,BDELAY,1,rc);
}
void BTColorClockPAL(PTVCARD pbttv)
{
   if (pbttv->pcard->palclock==CLOCK_XTAL0) BTDisablePLL(pbttv);
   if (pbttv->pcard->palclock==CLOCK_PLLFROMNTSC0) BTEnablePLL(pbttv);
      }
void BTColorClockNTSC(PTVCARD pbttv)
{
   if (pbttv->pcard->ntscclock==CLOCK_XTAL0) BTDisablePLL(pbttv);
   if (pbttv->pcard->ntscclock==CLOCK_PLLFROMPAL0) BTEnablePLL(pbttv);
      }

void BTEnablePLL(PTVCARD pbttv)
{
   UCHAR tempbyte;
   int i;
   APIRET rc;
   /* enable Crystal */
   tempbyte=BTReadRegister(pbttv,TGCTRL,1,rc);
   tempbyte&=BT848_TGCTRL_TGCKI_XTAL;
   rc=BTWriteRegister(pbttv,TGCTRL,tempbyte,1);
   /* clear DSTATUS.PLOCK */
   tempbyte=BTReadRegister(pbttv,DSTATUS,1,rc);
   tempbyte&=(~BT848_DSTATUS_PLOCK);
   BTWriteRegister(pbttv,DSTATUS,tempbyte,1);
   /* program PLL */
   BTWriteRegister(pbttv,PLL_F_LO,0xf9,1);
   BTWriteRegister(pbttv,PLL_F_HI,0xdc,1);
   BTWriteRegister(pbttv,PLL_XCI,0x8e,1);

   /* check PLOCK */
   for (i=0x7fffffff;i;i--) {
      tempbyte=BTReadRegister(pbttv,DSTATUS,1,rc);
      tempbyte&=BT848_DSTATUS_PLOCK;
      if (tempbyte) {
         tempbyte=BTReadRegister(pbttv,DSTATUS,1,rc);
         tempbyte&=(~BT848_DSTATUS_PLOCK);
         BTWriteRegister(pbttv,DSTATUS,tempbyte,1);
         }
         else break;
      }

    /* enable PLL */
   tempbyte=BTReadRegister(pbttv,TGCTRL,1,rc);
   tempbyte|=BT848_TGCTRL_TGCKI_PLL;
   rc=BTWriteRegister(pbttv,TGCTRL,tempbyte,1);
}
void BTDisablePLL(PTVCARD pbttv)
{
   UCHAR tempbyte;
   APIRET rc;
   /* enable Crystal */
   tempbyte=BTReadRegister(pbttv,TGCTRL,1,rc);
   tempbyte&=BT848_TGCTRL_TGCKI_XTAL;
   rc=BTWriteRegister(pbttv,TGCTRL,tempbyte,1);
   /* clear DSTATUS.PLOCK */
   tempbyte=BTReadRegister(pbttv,DSTATUS,1,rc);
   tempbyte&=(~BT848_DSTATUS_PLOCK);
   BTWriteRegister(pbttv,DSTATUS,tempbyte,1);
}
void BTSetHSCALE(PTVCARD pbttv,USHORT hscale)
{
    UCHAR tempbyte;
    APIRET rc;
    tempbyte=(UCHAR)hscale;
    BTWriteRegister(pbttv,E_HSCALE_LO,tempbyte,1);
    BTWriteRegister(pbttv,O_HSCALE_LO,tempbyte,1);
    tempbyte=(UCHAR)(hscale>>8);
    BTWriteRegister(pbttv,E_HSCALE_HI,tempbyte,1);
    BTWriteRegister(pbttv,O_HSCALE_HI,tempbyte,1);
}
void BTSetVSCALE(PTVCARD pbttv,USHORT vscale)
{}
void BTSetHDELAY(PTVCARD pbttv,USHORT hdelay)
{
    UCHAR tempbyte,tempbyte2;
    APIRET rc;
    tempbyte=(UCHAR)hdelay;
    BTWriteRegister(pbttv,E_HDELAY_LO,tempbyte,1);
    BTWriteRegister(pbttv,O_HDELAY_LO,tempbyte,1);
    tempbyte=(UCHAR)(hdelay>>8);
    tempbyte2=BTReadRegister(pbttv,E_CROP,1,rc);
    tempbyte2&=0xF3;
    tempbyte=(tempbyte&3)<<2;
    tempbyte=tempbyte|tempbyte2;
    BTWriteRegister(pbttv,E_CROP,tempbyte,1);
    BTWriteRegister(pbttv,O_CROP,tempbyte,1);
}

void BTSetVDELAY(PTVCARD pbttv,USHORT vdelay)
{
    UCHAR tempbyte,tempbyte2;
    APIRET rc;
    tempbyte=(UCHAR)vdelay;
    BTWriteRegister(pbttv,E_VDELAY_LO,tempbyte,1);
    BTWriteRegister(pbttv,O_VDELAY_LO,tempbyte,1);
    tempbyte=(UCHAR)(vdelay>>8);
    tempbyte2=BTReadRegister(pbttv,E_CROP,1,rc);
    tempbyte2&=0x3F;
    tempbyte=(tempbyte&3)<<6;
    tempbyte=tempbyte|tempbyte2;
    BTWriteRegister(pbttv,E_CROP,tempbyte,1);
    BTWriteRegister(pbttv,O_CROP,tempbyte,1);
}

void BTSetHACTIVE(PTVCARD pbttv,USHORT hactive)
{
    UCHAR tempbyte,tempbyte2;
    APIRET rc;
    tempbyte=(UCHAR)hactive;
    BTWriteRegister(pbttv,E_HACTIVE_LO,tempbyte,1);
    BTWriteRegister(pbttv,O_HACTIVE_LO,tempbyte,1);
    tempbyte=(UCHAR)(hactive>>8);
    tempbyte2=BTReadRegister(pbttv,E_CROP,1,rc);
    tempbyte2&=0xFC;
    tempbyte=(tempbyte&3);
    tempbyte=tempbyte|tempbyte2;
    BTWriteRegister(pbttv,E_CROP,tempbyte,1);
    BTWriteRegister(pbttv,O_CROP,tempbyte,1);
}

void BTSetVACTIVE(PTVCARD pbttv,USHORT vactive)
{
    UCHAR tempbyte,tempbyte2;
    APIRET rc;
    tempbyte=(UCHAR)vactive;
    BTWriteRegister(pbttv,E_VACTIVE_LO,tempbyte,1);
    BTWriteRegister(pbttv,O_VACTIVE_LO,tempbyte,1);
    tempbyte=(UCHAR)(vactive>>8);
    tempbyte2=BTReadRegister(pbttv,E_CROP,1,rc);
    tempbyte2&=0xCF;
    tempbyte=(tempbyte&3)<<4;
    tempbyte=tempbyte|tempbyte2;
    BTWriteRegister(pbttv,E_CROP,tempbyte,1);
    BTWriteRegister(pbttv,O_CROP,tempbyte,1);
}
APIRET BTSetSaturation(PTVCARD pbttv,USHORT saturation)
{
   UCHAR tempbyte;
   APIRET rc;
   if (saturation>0x1FF) return ERROR_INVALID_FUNCTION;
   if (saturation>0xff)
   {
      tempbyte=BTReadRegister(pbttv,E_CONTROL,1,rc);
      tempbyte=tempbyte|3;
      BTWriteRegister(pbttv,E_CONTROL,tempbyte,1);
      BTWriteRegister(pbttv,O_CONTROL,tempbyte,1);
    }
    else
       {
        tempbyte=BTReadRegister(pbttv,E_CONTROL,1,rc);
        tempbyte=tempbyte&0xfc;
        BTWriteRegister(pbttv,E_CONTROL,tempbyte,1);
        BTWriteRegister(pbttv,O_CONTROL,tempbyte,1);
       }
    tempbyte=(UCHAR)saturation;
    BTWriteRegister(pbttv,SAT_U_LO,tempbyte,1);
    BTWriteRegister(pbttv,SAT_V_LO,tempbyte,1);
    return NO_ERROR;
 }
USHORT  BTGetSaturation(PTVCARD pbttv)
{
   UCHAR tempbyte, tempbyte2;
   APIRET rc;
   tempbyte=BTReadRegister(pbttv,E_CONTROL,1,rc);
   tempbyte2=BTReadRegister(pbttv,SAT_V_LO,1,rc);
   if (tempbyte&BT848_E_CONTROL_SAT_V_MSB) return 0x100+tempbyte2;
      else return tempbyte2;
}
void BTMakeProgram(PTVCARD pbttv,ULONG startaddrphys,char * startaddrvirt,ULONG startaddrbufphys,UINT xpix,UINT ypix,UINT bpp)
{

        int tocopy,i,l;
        ULONG vlinesz;
        ULONG proglen = 0;
        vlinesz=xpix*bpp;
        //Make program
        //1. Wait for field start with SYNC
        // Wait for EvenField field VSYNC
        //SYNC|RESYNC|VRE
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_VRE | FIFO_RESYNC;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }
        //SYNC|FM1
        //Wait for packed data
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_FM1;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        // n lines
        //WRITE|SOL|EOL|bytesperline
        for (l = 0; l<ypix; l+=2)
        {
                CMD_WRITE *cw = (CMD_WRITE *)(startaddrvirt + proglen);
                proglen += sizeof(CMD_WRITE);
                cw->byte_count = vlinesz;
                cw->byte_en = 0;
                cw->rstat = 0;//0xC3;
                cw->irq = 0;
                cw->res = 0;
//                if (tocopy == vlinesz - i)
                    cw->eol = 1;
//                if (i == 0)
                    cw->sol = 1;
                cw->ocode = OC_WRITE;
                cw->target = startaddrbufphys + (l)*vlinesz;

        }

        //SYNC|RESYNC|VRO
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_VRO | FIFO_RESYNC;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        //SYNC|FM1
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_FM1;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        //n lines
        //WRITE|SOL|EOL|bytesperline
        for (l = 1; l<ypix; l+=2)
        {
                CMD_WRITE *cw = (CMD_WRITE *)(startaddrvirt + proglen);
                proglen += sizeof(CMD_WRITE);
                cw->byte_count = vlinesz;
                cw->byte_en = 0;
                cw->rstat = 0;//0xC3;
                cw->irq = 0;
                cw->res = 0;
//                if (tocopy == vlinesz - i)
                    cw->eol = 1;
//                if (i == 0)
                    cw->sol = 1;
                cw->ocode = OC_WRITE;
                cw->target = startaddrbufphys + (l)*vlinesz;

        }

        //JUMP to start
        {
            CMD_JMP *cj = (CMD_JMP *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_JMP);
            cj->reserved = 0;
            cj->rstat = 0xf0; //Indicate program that we've finished
            cj->irq = 0;
            cj->res = 0;
            cj->ocode = OC_JMP;
            cj->jaddr = startaddrphys;
            //jump to start
            //             + proglen;
        }

        //Ready. Now let's start
        dprintf(LOG_INFO,"Program start at %08xh\n", startaddrphys);
        dprintf(LOG_INFO,"Program len %08xh\n", proglen);
}
void BTWaitFrame(PTVCARD pbttv)
{
    int x;
    ULONG reg;
    APIRET rc;
    for ( x = 0x7fffffff; x; --x ) {
        reg=BTReadRegister(pbttv,INT_STAT,4,rc);
        //printf("count : %x, reg: %x \n",x,reg);
        if (reg&BT848_INT_VSYNC)
        {
            rc=BTWriteRegister(pbttv,INT_STAT,BT848_INT_VSYNC,4);
            break;
        }
        DosSleep(1);
        }
}
void BTMakeProgram2(PTVCARD pbttv,ULONG startaddrphys,char * startaddrvirt,ULONG startaddrbufphys,UINT xpix,UINT ypix,UINT bpp)
{

        int tocopy,i,l;
        ULONG vlinesz;
        ULONG proglen = 0;
        vlinesz=xpix*bpp;
        //Make program
        //1. Wait for field start with SYNC
        // Wait for EvenField field VSYNC
        //SYNC|RESYNC|VRE
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_VRE | FIFO_RESYNC;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }
        //SYNC|FM1
        //Wait for packed data
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_FM1;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        // n lines
        //WRITE|SOL|EOL|bytesperline
        for (l = 0; l<ypix/2; l++)
        {
                CMD_WRITE *cw = (CMD_WRITE *)(startaddrvirt + proglen);
                proglen += sizeof(CMD_WRITE);
                cw->byte_count = vlinesz;
                cw->byte_en = 0;
                cw->rstat = 0;//0xC3;
                cw->irq = 0;
                cw->res = 0;
//                if (tocopy == vlinesz - i)
                    cw->eol = 1;
//                if (i == 0)
                    cw->sol = 1;
                cw->ocode = OC_WRITE;
                cw->target = startaddrbufphys + (l)*vlinesz;

        }

        //SYNC|RESYNC|VRO
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_VRO | FIFO_RESYNC;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        //SYNC|FM1
        {
            CMD_SYNC *cs = (CMD_SYNC *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_FM1;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }

        //n lines
        //WRITE|SOL|EOL|bytesperline
        for (l = ypix/2; l<ypix; l++)
        {
                CMD_WRITE *cw = (CMD_WRITE *)(startaddrvirt + proglen);
                proglen += sizeof(CMD_WRITE);
                cw->byte_count = vlinesz;
                cw->byte_en = 0;
                cw->rstat = 0;//0xC3;
                cw->irq = 0;
                cw->res = 0;
//                if (tocopy == vlinesz - i)
                    cw->eol = 1;
//                if (i == 0)
                    cw->sol = 1;
                cw->ocode = OC_WRITE;
                cw->target = startaddrbufphys + l*vlinesz;

        }

        //JUMP to start
        {
            CMD_JMP *cj = (CMD_JMP *)(startaddrvirt + proglen);
            proglen += sizeof(CMD_JMP);
            cj->reserved = 0;
            cj->rstat = 0xf0; //Indicate program that we've finished
            cj->irq = 0;
            cj->res = 0;
            cj->ocode = OC_JMP;
            cj->jaddr = startaddrphys;
        }
        //Ready. Now let's start
        dprintf(LOG_DEBUG,"Program start at %08xh\n", startaddrphys);
        dprintf(LOG_DEBUG,"Program len %08xh\n", proglen);
}
void BTMakeProgramFrame(PTVCARD pbttv)
{
    int tocopy,i,l;
    ULONG proglen = 0;
    ULONG currchunk,chunkline;
    //Make program
    //1. Wait for field start with SYNC
    // Wait for EvenField field VSYNC
    //SYNC|RESYNC|VRE
    {
        CMD_SYNC *cs = (CMD_SYNC *)((PUCHAR)pbttv->pRiscVirt+proglen);
        proglen += sizeof(CMD_SYNC);
        cs->status = FIFO_VRE | FIFO_RESYNC;
        cs->rstat = 0;
        cs->irq = 0;
        cs->res = 0;
        cs->ocode = OC_SYNC;
        cs->res2 = 0;
    }
    //SYNC|FM1
    //Wait for packed data
    {
        CMD_SYNC *cs = (CMD_SYNC *)((PUCHAR)pbttv->pRiscVirt+proglen);
        proglen += sizeof(CMD_SYNC);
        cs->status = FIFO_FM1;
        cs->rstat = 0;
        cs->irq = 0;
        cs->res = 0;
        cs->ocode = OC_SYNC;
        cs->res2 = 0;
    }

    // n lines
    //WRITE|SOL|EOL|bytesperline
    //Even frame четный 0,2,4...
    for (currchunk=0; currchunk<pbttv->EvenFrame.numchunks;currchunk++)
    {
        for (chunkline=0; chunkline<pbttv->EvenFrame.linesperchunk; chunkline++)
        {
            //check if last chunk and real lines less chunk lines
            if (currchunk==pbttv->EvenFrame.numchunks-1)
                if (chunkline==pbttv->EvenFrame.lines-pbttv->EvenFrame.linesperchunk*currchunk)
                    break;
            CMD_WRITE *cw = (CMD_WRITE *)((char*)pbttv->pRiscVirt+proglen);
            proglen += sizeof(CMD_WRITE);
            cw->byte_count = pbttv->EvenFrame.linesize;
            cw->byte_en = 0;
            cw->rstat = 0;//0xC3;
            cw->irq = 0;
            cw->res = 0;
            cw->eol = 1;
            cw->sol = 1;
            cw->ocode = OC_WRITE;
            cw->target = pbttv->EvenFrame.chunks[currchunk].ulPhysAddr+(chunkline)*pbttv->EvenFrame.linesize;
            }
        }
        //SYNC|RESYNC|VRO
        {
            CMD_SYNC *cs = (CMD_SYNC *)((PUCHAR)pbttv->pRiscVirt+proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_VRO | FIFO_RESYNC;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }
        //SYNC|FM1
        {
            CMD_SYNC *cs = (CMD_SYNC *)((PUCHAR)pbttv->pRiscVirt+proglen);
            proglen += sizeof(CMD_SYNC);
            cs->status = FIFO_FM1;
            cs->rstat = 0;
            cs->irq = 0;
            cs->res = 0;
            cs->ocode = OC_SYNC;
            cs->res2 = 0;
        }
        //n lines
        //WRITE|SOL|EOL|bytesperline
        //Odd Frame нечетный 1,3,5...
        for (currchunk=0; currchunk<pbttv->OddFrame.numchunks;currchunk++)
        {
            for (chunkline=0; chunkline<pbttv->OddFrame.linesperchunk; chunkline++)
            {
    	        //check if last chunk and real lines less chunk lines
                if (currchunk==pbttv->OddFrame.numchunks-1)
                    if (chunkline==pbttv->OddFrame.lines-pbttv->OddFrame.linesperchunk*currchunk)
            	        break;
                CMD_WRITE *cw = (CMD_WRITE *)((PUCHAR)pbttv->pRiscVirt+proglen);
                proglen += sizeof(CMD_WRITE);
                cw->byte_count = pbttv->OddFrame.linesize;
                cw->byte_en = 0;
                cw->rstat = 0;//0xC3;
                cw->irq = 0;
                cw->res = 0;
                cw->eol = 1;
                cw->sol = 1;
                cw->ocode = OC_WRITE;
                cw->target = pbttv->OddFrame.chunks[currchunk].ulPhysAddr+(chunkline)*pbttv->OddFrame.linesize;
            }
        }
        //JUMP to start
//        {
            CMD_JMP *cj = (CMD_JMP *)((PUCHAR)pbttv->pRiscVirt+proglen);
            proglen += sizeof(CMD_JMP);
            cj->reserved = 0;
            cj->rstat = 0; //Indicate program that we've finished
            cj->irq = 0;
            cj->res = 0;
            cj->ocode=OC_JMP;
            cj->jaddr=pbttv->ulRiscPhys;
//        }
        //Ready. Now let's start
        dprintf(LOG_DEBUG,"Program start at %xh virt, %xh phys\n", pbttv->pRiscVirt, pbttv->ulRiscPhys);
        dprintf(LOG_DEBUG,"Program len %08xh, jaddr %08xh\n", proglen,cj->jaddr);
}
void BTCopyDualFrame(PTVCARD pbttv,PVOID pBuffer)
{
    ULONG currchunk,chunkline;
    ULONG currbuferpos,currchunkpos,linepos;
    UCHAR tempbyte;
    currchunk=0;
    currbuferpos=0;
    currchunkpos=0;
    for (currchunk=0; currchunk<pbttv->EvenFrame.numchunks;currchunk++)
    {
        currchunkpos=0;
        for (chunkline=0; chunkline<pbttv->EvenFrame.linesperchunk; chunkline++)
        {
            //check if last chunk and real lines less chunk lines
            if (currchunk==pbttv->EvenFrame.numchunks-1)
                if (chunkline==pbttv->EvenFrame.lines-pbttv->EvenFrame.linesperchunk*currchunk)
                    break;
            // copy lines
            // if not color - make yuv by hand
                if (pbttv->EvenFrame.bpp==2)
                {
                    memcpy((PUCHAR)pBuffer+currbuferpos,(PUCHAR)pbttv->EvenFrame.chunks[currchunk].pVirtAddr+currchunkpos,pbttv->EvenFrame.linesize);
                    currbuferpos+=pbttv->EvenFrame.linesize;
//            currchunkpos+=pbttv->EvenFrame.linesize;
                    memcpy((PUCHAR)pBuffer+currbuferpos,(PUCHAR)pbttv->OddFrame.chunks[currchunk].pVirtAddr+currchunkpos,pbttv->OddFrame.linesize);
                    currbuferpos+=pbttv->OddFrame.linesize;
                    currchunkpos+=pbttv->EvenFrame.linesize;

                }
    	        else
        	    {
                    for (linepos=0;linepos<pbttv->EvenFrame.pixels;linepos++)
                    {
                        tempbyte=((PUCHAR)pbttv->EvenFrame.chunks[currchunk].pVirtAddr)[currchunkpos+linepos];
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2))=tempbyte;
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2+1))=0;
            	        ((PUCHAR)pBuffer)[currbuferpos+linepos*2]=tempbyte;
        	            ((PUCHAR)pBuffer)[currbuferpos+linepos*2+1]=0x80;
                    }
                    currbuferpos+=(pbttv->EvenFrame.linesize*2);

                    for (linepos=0;linepos<pbttv->OddFrame.pixels;linepos++)
                    {
                        tempbyte=((PUCHAR)pbttv->OddFrame.chunks[currchunk].pVirtAddr)[currchunkpos+linepos];
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2))=tempbyte;
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2+1))=0;
                        ((PUCHAR)pBuffer)[currbuferpos+linepos*2]=tempbyte;
                        ((PUCHAR)pBuffer)[currbuferpos+linepos*2+1]=0x80;
                    }
                    currbuferpos+=(pbttv->OddFrame.linesize*2);
                    currchunkpos+=pbttv->EvenFrame.linesize;
            }

//            if (cfg.iscolor)
        }
    }
}
void BTCopySingleFrame(PTVCARD pbttv,PVOID pBuffer,BOOL bEven)
{
    ULONG currchunk,chunkline;
    ULONG currbuferpos,currchunkpos,linepos;
    UCHAR tempbyte;
    currchunk=0;
    currbuferpos=0;
    currchunkpos=0;
    PVIDEOFRAME pFrame;
    if (bEven) pFrame=&(pbttv->EvenFrame); else pFrame=&(pbttv->OddFrame);
    for (currchunk=0; currchunk<pFrame->numchunks;currchunk++)
    {
        currchunkpos=0;
        for (chunkline=0; chunkline<pFrame->linesperchunk; chunkline++)
        {
            //check if last chunk and real lines less chunk lines
            if (currchunk==pFrame->numchunks-1)
                if (chunkline==pFrame->lines-pFrame->linesperchunk*currchunk)
                    break;
            // copy lines
/*            if ((currchunk==0)&&(chunkline==0)&&!bEven)
            {
                memcpy((PUCHAR)pBuffer+currbuferpos,(PUCHAR)pFrame->chunks[currchunk].pVirtAddr+currchunkpos,pbttv->EvenFrame.linesize);
                currbuferpos+=pFrame->linesize;
            }

*/
            // if not color - make yuv by hand
            if (pbttv->EvenFrame.bpp==2)
            {
                memcpy((PUCHAR)pBuffer+currbuferpos,(PUCHAR)pFrame->chunks[currchunk].pVirtAddr+currchunkpos,pbttv->EvenFrame.linesize);
                currbuferpos+=pFrame->linesize;
                currchunkpos+=pFrame->linesize;
            }
            else
            {
                for (linepos=0;linepos<pbttv->EvenFrame.pixels;linepos++)
                {
                    tempbyte=((PUCHAR)pFrame->chunks[currchunk].pVirtAddr)[currchunkpos+linepos];
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2))=tempbyte;
//                    (*((PUCHAR)pBuffer+currbuferpos+linepos*2+1))=0;
                    ((PUCHAR)pBuffer)[currbuferpos+linepos*2]=tempbyte;
                    ((PUCHAR)pBuffer)[currbuferpos+linepos*2+1]=0x80;

                }
            currbuferpos+=(pFrame->linesize*2);
            currchunkpos+=pFrame->linesize;
            }

        }
    }
}
void BTRiscDump(ULONG phys,char* virt)
{
    int i;
    i=0;
    ULONG val;
    CMD_WRITE write;
    CMD_SYNC sync;
    CMD_JMP jump;
    for (;;)
    {
        val=*((ULONG*)(virt+i));
        dprintf(LOG_DEBUG,"phys - %xh, virt - %xh, val %xh\n",phys+i,virt+i,val);
        if ((val&0xf0000000)==(OC_JMP<<28))
        {
            jump=*(CMD_JMP*)(virt+i);
            dprintf(LOG_DEBUG,"JUMP operation: reserved %x, rstat %x, irq %x, res %x, ocode %x, jaddr %x \n",jump.reserved,jump.rstat,jump.irq,jump.res,jump.ocode,jump.jaddr);
            i=i+8;
            continue;
        }
        if ((val&0xf0000000)==(OC_WRITE<<28))
        {
            write=*(CMD_WRITE*)(virt+i);
            dprintf(LOG_DEBUG,"WRITE operation. byte_count %x, byte_en %x, rstat %x, irq %x, res %x, eol %x, sol %x, ocode %x \n",write.byte_count,write.byte_en,write.rstat,write.irq,write.res,write.eol,write.sol,write.ocode);
            i=i+4;
            dprintf(LOG_DEBUG,"WRITE operation. target %x \n",write.target);
            i=i+4;
            continue;
        }
        if ((val&0xf0000000)==OC_SYNC<<28)
        {
            sync=*(CMD_SYNC*)(virt+i);
            dprintf(LOG_DEBUG,"SYNC operation. status: %x, rstat %x, irq %x, res %x, ocode %x, res2 %x \n",sync.status,sync.rstat,sync.irq,sync.res,sync.ocode,sync.res2);
            i=i+8;
            continue;
        }
        dprintf(LOG_DEBUG,"invalid or unknown opcode \n");
        break;
    }
}
