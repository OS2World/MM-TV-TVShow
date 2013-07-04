#include "bti2cs.h"
APIRET rcs;

void BTI2CBusSet(PTVCARD pbttv, BOOL SCL, BOOL SDA)
{
    BTWriteRegister(pbttv,I2C_DATA_CTL,((SCL<<1)|SDA)&3,4);
    BTI2CBusWait(I2C_DELAY);
}
BOOL BTI2CBusGet(PTVCARD pbttv)
{
    return BTReadRegister(pbttv,I2C_DATA_CTL,4,rcs)&1;
}
void BTI2CBusStart(PTVCARD pbttv)
{
    BTI2CBusSet(pbttv,0, 1);
    BTI2CBusSet(pbttv,1, 1);
    BTI2CBusSet(pbttv,1, 0);
    BTI2CBusSet(pbttv,0, 0);
}
void BTI2CBusStop(PTVCARD pbttv)
{
    BTI2CBusSet(pbttv,0, 0);
    BTI2CBusSet(pbttv,1, 0);
    BTI2CBusSet(pbttv,1, 1);
}
void BTI2CBusOne(PTVCARD pbttv)
{
    BTI2CBusSet(pbttv,0, 1);
    BTI2CBusSet(pbttv,1, 1);
    BTI2CBusSet(pbttv,0, 1);
}
void BTI2CBusZero(PTVCARD pbttv)
{
    BTI2CBusSet(pbttv,0, 0);
    BTI2CBusSet(pbttv,1, 0);
    BTI2CBusSet(pbttv,0, 0);
}
BOOL BTI2CBusAck(PTVCARD pbttv)
{
    BOOL bAck;

    BTI2CBusSet(pbttv,0,1);
    BTI2CBusSet(pbttv,1,1);
    bAck = !BTI2CBusGet(pbttv);
    BTI2CBusSet(pbttv,0,1);
    if (bAck) dprintf(LOG_DEBUG3,"i2cs got ACK \n");
        else dprintf(LOG_DEBUG3,"i2cs got NAK \n");
    return bAck;
}
BOOL BTI2CBusSendByte(PTVCARD pbttv,BYTE nData, int nWaitForAck)
{
    dprintf(LOG_DEBUG3,"i2cs write %x\n",nData);
    BTI2CBusSet(pbttv,0,0);
    nData&0x80?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x40?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x20?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x10?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x08?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x04?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x02?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    nData&0x01?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    if (nWaitForAck)
        BTI2CBusWait(nWaitForAck);
    return BTI2CBusAck(pbttv);
}
BYTE BTI2CBusReadByte(PTVCARD pbttv,BOOL bLast)
{
    int i;
    BYTE bData = 0;
    BTI2CBusSet(pbttv,0, 1);
    for (i = 7; i >= 0; i--)
    {
        BTI2CBusSet(pbttv,1, 1);
        if (BTI2CBusGet(pbttv))
            bData |= (1 << i);
        BTI2CBusSet(pbttv,0, 1);
    }
    bLast?BTI2CBusOne(pbttv):BTI2CBusZero(pbttv);
    dprintf(LOG_DEBUG3,"i2cs read - %x\n",bData);
    return bData;
}
void BTI2CBusWait(int us)
{
//    if (us > 0)
    {
//        DosSleep(1);
        //us
//        return;
    }
    ULONG i;
//    DosSleep(16);
    for (i=0;i<0xffff;i++)
    {         }
//    DosSleep(1);
}


