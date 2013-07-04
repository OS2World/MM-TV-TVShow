#include <string.h>
#include <time.h>

//#include "fmfuncs.h"
#include "base.h"
#include "logger.h"

#include "globals.h"
HFILE hDriver;


#include "\projects\warpdriver\ioctl.h"
#include "driver.h"
#include "globals.h"
#include "memory.h"
#define SAFETY_SPACE 32768

ULONG videolen,physaddrframe,physaddrrisc;

int main()
{
//    IString error;
    ULONG fRC;
    UCHAR red, green, blue;
    APIRET rc;
    int pipetid;
    VIDEOFRAME testframe;
ULONG ulAction;
    LogLevel=9;
    strcpy(LogFile,"grabber.log");
    rc=BTOpenDriver(&hDriver);
    rc=MemCreateFrame(pbttv,&testframe,768,576,2);
    rc=MemDeleteFrame(pbttv,&testframe);
/*for (videolen=4096;videolen<2048000;videolen+=4096)
{

    dprintf(LOG_DEBUG,"Allocating %d bytes \n",videolen);
    rc=BTAllocMapMem(&hDriver,videolen,&physaddrframe,(PPVOID)&framebuf);
    dprintf(LOG_DEBUG,"mem mapped to %xh\n", framebuf);


    dprintf(LOG_DEBUG,"Free frame Buffer Mapping at %xh \n",framebuf);
    BTFreeMem(&hDriver,physaddrframe,framebuf);

} */
    return 0;
} /* end main */


