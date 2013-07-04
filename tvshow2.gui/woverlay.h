#if __cplusplus
extern "C" {
#endif
#include "hwvideo.h"
#include "card.h"
#define FOURCC_Y422 0x32323459
#define FOURCC_LUT8 0x3854554c
#define FOURCC_R565 0x35363552
BOOL LoadOverlay();
char * RCDecode(ULONG);
ULONG HWVideoInit();
ULONG HWVideoCaps(PHWVIDEOCAPS pOverlayCaps);
ULONG HWVideoSetup(PHWVIDEOSETUP pSetup);
void HWVideoClose(PHWVIDEOSETUP pSetup,PHWVIDEOCAPS pOverlayCaps);
void HWVideoTest();
//if bEvenField true, now process even field, else odd field
void HWVideoBlitNormal(PTVCARD pbttv, BOOL bEvenField); //if bEvenField true, now process even field, else odd field
void HWVideoBlitBob(PTVCARD pbttv, BOOL bEvenField);
void HWVideoBlitWeave(PTVCARD pbttv, BOOL bEvenField);
#if __cplusplus
}
#endif

