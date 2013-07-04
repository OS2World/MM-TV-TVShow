/*
** Module   :PREGACC.CPP
** Abstract :PCI Access functions
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  26/02/2002 Created
**
*/

#if __IBMCPP__ < 400
#define INCL_DOS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#include <os2.h>

#endif
#include "pcifuncs.h"

#pragma pack(1)
static struct
{
    UCHAR subfunc;
    UCHAR busnum;
    UCHAR devfunc;
    UCHAR confreg;
    UCHAR size;
    ULONG data;
} maindata;
static HFILE   OEMHlpHandle;
static ULONG   OEMHlpDataLen;
static ULONG   OEMHlpParamLen;


ULONG PCIReadRegister(UCHAR confreg, UCHAR size, APIRET &rc)
{
        //Read current value
        struct
        {
            UCHAR rc;
            ULONG data;
        } dt;

        maindata.subfunc = 3;
        maindata.confreg = confreg; //OFFSET 48
        maindata.size = size; //Single byte
        OEMHlpParamLen = 5;
        OEMHlpDataLen = sizeof(dt);
        rc = DosDevIOCtl(OEMHlpHandle, IOCTL_OEMHLP,
            OEMHLP_PCI,
            &maindata,OEMHlpParamLen,&OEMHlpParamLen,&dt,OEMHlpDataLen,&OEMHlpDataLen);
        return dt.data;
}

ULONG PCIWriteRegister(UCHAR confreg, ULONG value, UCHAR size)
{
            //Write modified value
            UCHAR dt;
            maindata.subfunc = 4;
            maindata.data = value; //Set needed bits
            maindata.confreg = confreg; //OFFSET 48
            maindata.size = size; //Single byte
            OEMHlpParamLen = sizeof(maindata);
            OEMHlpDataLen = sizeof(dt);
            return DosDevIOCtl(OEMHlpHandle, IOCTL_OEMHLP,
                OEMHLP_PCI,
                &maindata,OEMHlpParamLen,&OEMHlpParamLen,&dt,OEMHlpDataLen,&OEMHlpDataLen);
}

ULONG InitOEMHlp()
{
    ULONG temp;
    return DosOpen("OEMHLP$", &OEMHlpHandle, &temp, 0, 0,
        OPEN_ACTION_OPEN_IF_EXISTS | OPEN_ACTION_FAIL_IF_NEW,
        OPEN_SHARE_DENYNONE, 0);
}

ULONG PCIUseDevice(USHORT vendor, USHORT device, UCHAR index)
{
            struct
            {
                UCHAR sf;
                USHORT devid;
                USHORT venid;
                UCHAR  index;
            } par = {01 /* Find PCI */, device, vendor, index};
            OEMHlpParamLen = sizeof(par);
            OEMHlpDataLen = 3;

            return DosDevIOCtl(OEMHlpHandle, IOCTL_OEMHLP,
                OEMHLP_PCI,
                &par,OEMHlpParamLen,&OEMHlpParamLen,&maindata,OEMHlpDataLen,&OEMHlpDataLen);
}
ULONG CloseOEMHlp()
{
        return  DosClose(OEMHlpHandle);
}
