/*
** Module   :PREGACC.H
** Abstract :PCI Access Function
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  26/02/2002 Created
**
*/

#ifndef __REGACC_H
#define __REGACC_H
#if __cplusplus
extern "C" {
#endif


ULONG InitOEMHlp();
ULONG PCIReadRegister(UCHAR confreg, UCHAR size, APIRET &rc);
ULONG PCIWriteRegister(UCHAR confreg, ULONG value, UCHAR size);
ULONG PCIUseDevice(USHORT vendor, USHORT device, UCHAR index);
ULONG CloseOEMHlp();

#if __cplusplus
}
#endif


#endif  /*__REGACC_H*/

