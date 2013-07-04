#include "driver.h"
#include "ioctl.h"
//#include "maphymem.h"

#define OPEN_FLAG ( OPEN_ACTION_OPEN_IF_EXISTS )
#define OPEN_MODE ( OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READWRITE )
#define CallPDD(hFile,function) DoIOCtl(hFile,function, NULL, 0)
#define CallPDD2(hFile,function, parm) DoIOCtl(hFile,function, &parm, sizeof(parm))
ULONG DoIOCtl(HFILE hfile, ULONG ulFunction, PVOID pvParm, ULONG ulParmLen)
{
   ULONG ulParmInOut = 0;

   return DosDevIOCtl(hfile, GENPDD_IOCTL_CATEGORY, ulFunction, pvParm, ulParmLen, &ulParmLen, NULL, 0, &ulParmInOut);
}


APIRET BTOpenDriver(HFILE* phDriver)
{
    APIRET rc;
    ULONG ulAction;
    rc=DosOpen((PSZ)"GENPDD$ ",phDriver,&ulAction,0,0,OPEN_FLAG,OPEN_MODE,NULL);
    if (rc) {
      dprintf(LOG_ERROR,"DosOpen failed with RC = %lu\n", rc);
      return 1;
    }
    return NO_ERROR;
}
APIRET BTCloseDriver(HFILE hDriver)
{
    APIRET rc;
    rc=DosClose(hDriver);
    if (rc) {
      dprintf(LOG_ERROR,"DosClose failed with RC = %lu\n", rc);
      return 1;
    }
    return NO_ERROR;
}
/*APIRET BTGetDriverVersion()
{
    APIRET rc;
    GENPDD_GET_VERSION version;
    rc = CallPDD2(GENPDD_IOCTL_GET_VERSION, version);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_GET_VERSION failed with RC = %lu\n", rc);
      return 1;
    }
    return NO_ERROR;
} */
APIRET BTAllocMapContMem(HFILE hDriver,ULONG ulMemSize, PULONG pulPhysAddr, PPVOID ppVirtAddr)
{
    APIRET rc;
    GENPDD_ALLOC_PH allocate;

    allocate.ulSize=ulMemSize;
    dprintf(LOG_DEBUG,"Allocating %d bytes\n",allocate.ulSize);
    rc=CallPDD2(hDriver,GENPDD_IOCTL_ALLOC_PH,allocate);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_ALLOCATE_PH failed with RC = %x\n", rc);
      return 1;
    };
    *pulPhysAddr=allocate.ulPhysAddr;
    dprintf(LOG_DEBUG,"buffer size %d bytes allocated at %xh phys\n",allocate.ulSize,allocate.ulPhysAddr);
//    rc=DosMapPhysMem(ppVirtAddr,ulMemSize,*pulPhysAddr);
//    if (rc!=NO_ERROR) {
//      dprintf(LOG_ERROR,"MapPhysMem return code %u \n", rc);
//      return 6;
//    };
    GENPDD_MAP_PH map;
    map.ulPhysAddr=allocate.ulPhysAddr;
    map.ulSize=ulMemSize;
    rc=CallPDD2(hDriver,GENPDD_IOCTL_MAP_PH,map);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_MAP_PH failed with RC = %x\n", rc);
      return 1;
    };
    *ppVirtAddr=(PVOID)map.ulR3LinAddr;
    dprintf(LOG_DEBUG,"buffer mapped to %xh\n",*ppVirtAddr);
    return NO_ERROR;
}
APIRET BTFreeContMem(HFILE hDriver, ULONG ulPhysAddr, PVOID pVirtAddr)
{
    GENPDD_FREE_PH    freeph;
    GENPDD_FREE_MAP_PH unmap;
    APIRET rc;

    dprintf(LOG_DEBUG,"Free buffer mapping at %xh \n",pVirtAddr);
//    rc=DosUnmapPhysMem(pVirtAddr);
//    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"MapUnMapPhysMem framebuf return code %u \n", rc);return 1;};
    unmap.ulR3LinAddr=(ULONG)pVirtAddr;
    rc=CallPDD2(hDriver,GENPDD_IOCTL_FREE_MAP_PH,unmap);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_FREE_MAP_PH failed with RC = %x\n", rc);
      return 1;
    };

    dprintf(LOG_DEBUG,"free buffer at %xh phys\n", ulPhysAddr);
    freeph.ulPhysAddr=ulPhysAddr;
    rc = CallPDD2(hDriver,GENPDD_IOCTL_FREE_PH, freeph);
    if (rc) {
       printf("GENPDD_IOCTL_FREE_PH failed with RC = %x\n", rc);
       return 6;
    }
    return NO_ERROR;
}
APIRET BTAllocMapMem(HFILE hDriver,ULONG ulMemSize, PULONG pulPhysAddr, PPVOID ppVirtAddr, PULONG pulLinAddr)
{
    APIRET rc;
    GENPDD_ALLOCMAP_PH allocate;

    allocate.ulSize=ulMemSize+(0x1000-(ulMemSize&0xFFF));//align to 4k
    dprintf(LOG_DEBUG,"Allocating %d bytes\n",allocate.ulSize);
    rc=CallPDD2(hDriver,GENPDD_IOCTL_ALLOCMAP_PH,allocate);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_ALLOCMAP_PH failed with RC = %x\n", rc);
      return 1;
    };
    *pulPhysAddr=allocate.ulPhysAddr;
    dprintf(LOG_DEBUG,"buffer size %d bytes allocated at %xh phys, %xh lin, mapped to %xh virt\n",allocate.ulSize,allocate.ulPhysAddr,allocate.ulGLinAddr,allocate.ulR3LinAddr);
    *ppVirtAddr=(PVOID)allocate.ulR3LinAddr;
    *pulLinAddr=allocate.ulGLinAddr;
    return NO_ERROR;
}
APIRET BTFreeMem(HFILE hDriver, ULONG ulPhysAddr, PVOID pVirtAddr, ULONG ulLinAddr)
{
    GENPDD_FREE_ALLOCMAP_PH    freemapph;
    APIRET rc;

    dprintf(LOG_DEBUG,"Free buffer at %xh phys ,%xh lin, mapped to %xh virt\n",ulPhysAddr,ulLinAddr,pVirtAddr);
    freemapph.ulR3LinAddr=(ULONG)pVirtAddr;
    freemapph.ulGLinAddr=ulLinAddr;
    rc=CallPDD2(hDriver,GENPDD_IOCTL_FREE_ALLOCMAP_PH,freemapph);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_FREE_MAP_PH failed with RC = %x\n", rc);
      return 1;
    };
    return NO_ERROR;
}

APIRET BTMapMem(HFILE hDriver,ULONG ulPhysAddr,ULONG ulMemSize,PPVOID ppVirtAddr)
{
    GENPDD_MAP_PH map;
    APIRET rc;

    map.ulPhysAddr=ulPhysAddr;
    map.ulSize=ulMemSize;
    rc=CallPDD2(hDriver,GENPDD_IOCTL_MAP_PH,map);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_MAP_PH failed with RC = %x\n", rc);
      return 1;
    };
    *ppVirtAddr=(PVOID)map.ulR3LinAddr;
    dprintf(LOG_DEBUG,"buffer mapped to %xh\n",*ppVirtAddr);
    return NO_ERROR;
}
APIRET BTUnMapMem(HFILE hDriver,PVOID pVirtAddr)
{
    GENPDD_FREE_MAP_PH unmap;
    APIRET rc;

    dprintf(LOG_DEBUG,"Free buffer mapping at %xh \n",pVirtAddr);
//    rc=DosUnmapPhysMem(pVirtAddr);
//    if (rc!=NO_ERROR) {dprintf(LOG_ERROR,"MapUnMapPhysMem framebuf return code %u \n", rc);return 1;};
    unmap.ulR3LinAddr=(ULONG)pVirtAddr;
    rc=CallPDD2(hDriver,GENPDD_IOCTL_FREE_MAP_PH,unmap);
    if (rc) {
      dprintf(LOG_ERROR,"GENPDD_IOCTL_FREE_MAP_PH failed with RC = %x\n", rc);
      return 1;
    };
    return NO_ERROR;
}
