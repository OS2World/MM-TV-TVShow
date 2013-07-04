/*
** Module   :DRIVER.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Fri  20/02/2004 Created
**
*/
#ifndef __DRIVER_H
#define __DRIVER_H



#include "base.h"

APIRET BTOpenDriver(HFILE* phDriver);
APIRET BTCloseDriver(HFILE hDriver);
APIRET BTAllocMapContMem(HFILE hDriver,ULONG ulMemSize, PULONG pulPhysAddr, PPVOID ppVirtAddr);
APIRET BTFreeContMem(HFILE hDriver, ULONG ulPhysAddr, PVOID pVirtAddr);
APIRET BTAllocMapMem(HFILE hDriver,ULONG ulMemSize, PULONG pulPhysAddr, PPVOID ppVirtAddr, PULONG pulLinAddr);
APIRET BTFreeMem(HFILE hDriver, ULONG ulPhysAddr, PVOID pVirtAddr, ULONG ulLinAddr);
APIRET BTMapMem(HFILE hDriver,ULONG ulMemSize, ULONG ulPhysAddr, PPVOID ppVirtAddr);
APIRET BTUnMapMem(HFILE hDriver, PVOID pVirtAddr);


#endif  /*__DRIVER_H*/

