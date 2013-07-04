//#define INCL_DOS
#define INCL_DOSMODULEMGR
#include <os2.h>

#include <string.h>

#include "hwvideo.h"
#include "woverlay.h"
#include "btfuncs.h"


// WarpOverlay! functions pointers
ULONG (VIDEOCALL *pHWVIDEOInit)(void);
ULONG (VIDEOCALL *pHWVIDEOCaps)(PHWVIDEOCAPS);
ULONG (VIDEOCALL *pHWVIDEOSetup)(PHWVIDEOSETUP);
ULONG (VIDEOCALL *pHWVIDEOBeginUpdate)(PVOID,PULONG);
ULONG (VIDEOCALL *pHWVIDEOEndUpdate)(void);
ULONG (VIDEOCALL *pHWVIDEOGetAttrib)(ULONG,PHWATTRIBUTE);
ULONG (VIDEOCALL *pHWVIDEOSetAttrib)(ULONG,PHWATTRIBUTE);
ULONG (VIDEOCALL *pHWVIDEOClose)(void);
// WarpOverlay!-related global variables
//HWVIDEOSETUP Setup={0};
//HWVIDEOCAPS  OverlayCaps={0};
//SIZEL  MovieSize={0}; //dimension of movie picture
//RECTL  MovieRect={0}; //movie's viewable rectangle
HMODULE HWVideoHandle=NULLHANDLE;
ULONG  fRc;

BOOL LoadOverlay()
{
    char TempStr[255];
      //Load WarpOverlay! API DLL
    if (DosLoadModule(TempStr,sizeof(TempStr),"hwvideo.dll",&HWVideoHandle)) return FALSE;
      //Get all functions entry points
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOInit",(PFN*)&pHWVIDEOInit)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOCaps",(PFN*)&pHWVIDEOCaps)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOSetup",(PFN*)&pHWVIDEOSetup)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOBeginUpdate",(PFN*)&pHWVIDEOBeginUpdate)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOEndUpdate",(PFN*)&pHWVIDEOEndUpdate)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOGetAttrib",(PFN*)&pHWVIDEOGetAttrib)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOSetAttrib",(PFN*)&pHWVIDEOSetAttrib)) return FALSE;
    if  (DosQueryProcAddr(HWVideoHandle,0,"HWVIDEOClose",(PFN*)&pHWVIDEOClose)) return FALSE;
    return TRUE;
}
//Decode WarpOverlay! return code into text string
char * RCDecode(ULONG rc) {
      switch (rc) {
         case HWVIDEO_DONE:
              return "No Error";
         case HWVIDEO_ERROR:
              return "Unspecified error";
         case HWVIDEO_ERROR_BACKGROUND:
              return "PM in background";
         case HWVIDEO_ERROR_NO_HW:
              return "Unsupported HW";
         case HWVIDEO_ERROR_PARAMETER:
              return "Incorrect parameter passed";
         case HWVIDEO_ERROR_LOW_VRAM:
              return "Too low VRAM for requested setup";
         case HWVIDEO_ERROR_USED:
              return "HW owned by another process";
         default:
              return "Undefined error code!";
      }
}
ULONG HWVideoCaps(PHWVIDEOCAPS pOverlayCaps)
{
    // Query overlay capabilities
    pOverlayCaps->ulNumColors=0;
//    pOverlayCaps->ulLength=sizeof((HWVIDEOCAPS)*(pOverlayCaps));
    pOverlayCaps->ulLength=sizeof(HWVIDEOCAPS);
    fRc=pHWVIDEOCaps(pOverlayCaps);
//    if (fRc) return fRc;
    // this time OverlayCaps.ulNumColors filled with actual count of supported FOURCCs
    DosAllocMem((PPVOID)&(pOverlayCaps->fccColorType),pOverlayCaps->ulNumColors*4,PAG_COMMIT|PAG_WRITE);
    return pHWVIDEOCaps(pOverlayCaps);
}
ULONG HWVideoInit(void)
{
    return pHWVIDEOInit();
}
ULONG HWVideoSetup(PHWVIDEOSETUP pSetup)
{
    return pHWVIDEOSetup(pSetup);
}
void HWVideoClose(PHWVIDEOSETUP pSetup,PHWVIDEOCAPS pOverlayCaps)
{
    DosFreeMem(pOverlayCaps->fccColorType);
    pHWVIDEOClose();
    DosFreeModule(HWVideoHandle);
}
void HWVideoTest(void) {
PUCHAR Buffer;
ULONG temp,rc,i,j;
   rc=pHWVIDEOBeginUpdate((PPVOID)&Buffer,&temp);
   //it's important to check return code,
   //because moving window or starting FS session both invalidate setup.
   //
   if (!rc)
   {
//    DosBeep(1000,100);
    //do something      temp=
    memset(Buffer,0xff,768*576*2);
//      for (j=0;j<256;j++) {
//          for (i=0;i<64;i++) {
//              Buffer[j*Setup.ulSrcPitch/2+i]=0x001f;
//              Buffer[j*Setup.ulSrcPitch/2+i+64]=0x07e0;
//              Buffer[j*Setup.ulSrcPitch/2+i+128]=0xf800;
//              Buffer[j*Setup.ulSrcPitch/2+i+192]=0xffff;
 //         }
//      }

   }
   pHWVIDEOEndUpdate();
}

void HWVideoBlitNormal(PTVCARD pbttv, BOOL bEvenField)
{
    PVOID Buffer;
	ULONG rc,temp;
   rc=pHWVIDEOBeginUpdate(&Buffer,&temp);
   //it's important to check return code,
   //because moving window or starting FS session both invalidate setup.
   if (!rc)
    {
        // real blit
        //do copy only on even field, copy both field
        //even+odd;even+odd;
        BTCopyDualFrame(pbttv,Buffer);
    }
    pHWVIDEOEndUpdate();
}
void HWVideoBlitBob(PTVCARD pbttv, BOOL bEvenField)
{
    PVOID Buffer;
    ULONG rc,temp;
   rc=pHWVIDEOBeginUpdate(&Buffer,&temp);
   //it's important to check return code,
   //because moving window or starting FS session both invalidate setup.
   if (!rc)
    {
    // real blit
        //do copy single field, even,odd;
        BTCopySingleFrame(pbttv,Buffer,bEvenField);
    }
    pHWVIDEOEndUpdate();
}
void HWVideoBlitWeave(PTVCARD pbttv, BOOL bEvenField)
{
    PVOID Buffer;
    ULONG rc,temp;
   rc=pHWVIDEOBeginUpdate(&Buffer,&temp);
   //it's important to check return code,
   //because moving window or starting FS session both invalidate setup.
   if (!rc)
    {
    // real blit
        //do copy both field on each field
        //even+odd,even+odd;even+odd,even+odd;
        BTCopyDualFrame(pbttv,Buffer);
    }
    pHWVIDEOEndUpdate();
}






