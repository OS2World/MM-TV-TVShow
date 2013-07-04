/*
** Module   :BTI2C.CPP
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Wed  15/05/2002 Created
**
*/
#include "bti2c.h"
#include "base.h"
#include "card.h"
#include "btregs.h"
#include "btfuncs.h"
APIRET rc;

UCHAR BTI2CRead1(PTVCARD pbttv, UCHAR i2caddr, APIRET &rccode)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword|1; //read mode
    tempdword=tempdword<<24; //shift
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword&=(~BT848_DATA_CTL_I2CNOSTOP);
    tempdword&=(~BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword&=(~BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;

    tempdword=tempdword&(~BT848_DATA_CTL_I2CNOSTOP)&(~BT848_DATA_CTL_I2CNOS1B);
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC|BT848_DATA_CTL_I2CW3BRA|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
//    dprintf(LOG_DEBUG,"i2c data for read %x\n",tempdword);
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 rccode=0;
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 tempdword=BTReadRegister(pbttv,I2C_DATA_CTL,4,rc);
                 dprintf(LOG_DEBUG3,"i2c %x: %x \n",i2caddr,(UCHAR)tempdword>>8);
                 return (UCHAR)(tempdword>>8);
             }
             else
             {
                 rccode=-1;
                 tempdword&=BT848_INT_I2CDONE;
                 dprintf(LOG_DEBUG3,"i2c %x: not readed - i2c Done without i2c ACK\n",i2caddr);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return 0;
             }
         }
      }
      rccode=-1;
      return 0;
}

APIRET BTI2CWrite4(PTVCARD pbttv,UCHAR i2caddr, UCHAR data1, UCHAR data2, UCHAR data3, UCHAR data4)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    dprintf(LOG_DEBUG3,"i2c %x: write %x %x %x %x\n",i2caddr,data1,data2,data3,data4);
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword&0xfffffffe; //write mode
    tempdword=tempdword<<24; //shift
    tempdword|=(data1<<16);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword|BT848_DATA_CTL_I2CNOSTOP&(~BT848_DATA_CTL_I2CNOS1B);
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC&(~BT848_DATA_CTL_I2CW3BRA);
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 break;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return -1;
             }
         }
      }

    tempdword=(data2<<24);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword|BT848_DATA_CTL_I2CNOSTOP|BT848_DATA_CTL_I2CNOS1B;
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC;
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 break;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return -1;
             }
         }
      }
    tempdword=(data3<<24);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword|BT848_DATA_CTL_I2CNOSTOP|BT848_DATA_CTL_I2CNOS1B;
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC;
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 break;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return -1;
             }
         }
      }

    tempdword=(data4<<24);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=(tempdword&(~BT848_DATA_CTL_I2CNOSTOP))|BT848_DATA_CTL_I2CNOS1B;
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC;
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 dprintf(LOG_DEBUG3,"i2c write %x: ACK \n",i2caddr);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 break;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 dprintf(LOG_DEBUG3,"i2c write %x: i2c Done without i2c ACK\n",i2caddr);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return -1;
             }
         }
      }
   return NO_ERROR;
}
APIRET BTI2CWrite2(PTVCARD pbttv,UCHAR i2caddr, UCHAR data1, UCHAR data2)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    dprintf(LOG_DEBUG3,"i2c %x: write %x %x\n",i2caddr,data1,data2);
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword&0xfffffffe; //write mode
    tempdword=tempdword<<24; //shift
    tempdword|=(data1<<16);
    tempdword|=(data2<<8);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword&(~BT848_DATA_CTL_I2CNOSTOP)&(~BT848_DATA_CTL_I2CNOS1B);
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC|BT848_DATA_CTL_I2CW3BRA;
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--) {
       DosSleep(1);
      tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
      tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
      if (tempdword) {
         tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
         BTWriteRegister(pbttv,INT_STAT,tempdword,4);
         dprintf(LOG_DEBUG3,"i2c write %x: ACK \n",i2caddr);
         return 0;}
      }
      return NO_ERROR;
}
APIRET BTI2CWrite1(PTVCARD pbttv,UCHAR i2caddr, UCHAR data1)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    dprintf(LOG_DEBUG3,"i2c %x: write %x %x\n",i2caddr,data1);
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword&0xfffffffe; //write mode
    tempdword=tempdword<<24; //shift
    tempdword|=(data1<<16);
    tempdword|=(0<<8);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword&(~BT848_DATA_CTL_I2CNOSTOP)&(~BT848_DATA_CTL_I2CNOS1B);
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC&(~BT848_DATA_CTL_I2CW3BRA);
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--) {
       DosSleep(1);
      tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
      tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
      if (tempdword) {
         tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
         BTWriteRegister(pbttv,INT_STAT,tempdword,4);
         dprintf(LOG_DEBUG3,"i2c write %x: ACK \n",i2caddr);
         return 0;}
      }
      return NO_ERROR;
}
UCHAR BTI2CReadFirst(PTVCARD pbttv, UCHAR i2caddr, APIRET &rccode)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword|1; //read mode
    tempdword=tempdword<<24; //shift
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword|=(BT848_DATA_CTL_I2CNOSTOP);
    tempdword&=(~BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword|=(BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 rccode=0;
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 tempdword=BTReadRegister(pbttv,I2C_DATA_CTL,4,rc);
                 return (UCHAR)(tempdword>>8);
             }
             else
             {
                 rccode=-1;
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return 0;
             }
         }
      }
      rccode=-1;
      return 0;
}

UCHAR BTI2CRead(PTVCARD pbttv, APIRET &rccode)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=0;
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword|=(BT848_DATA_CTL_I2CNOSTOP);
    tempdword|=(BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword|=(BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 rccode=0;
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 tempdword=BTReadRegister(pbttv,I2C_DATA_CTL,4,rc);
                 return (UCHAR)(tempdword>>8);
             }
             else
             {
                 rccode=-1;
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return 0;
             }
         }
      }
      rccode=-1;
      return 0;
}

UCHAR BTI2CReadLast(PTVCARD pbttv, APIRET &rccode)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=0;
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword&=(~BT848_DATA_CTL_I2CNOSTOP);
    tempdword|=(BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword&=(~BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 rccode=0;
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 tempdword=BTReadRegister(pbttv,I2C_DATA_CTL,4,rc);
                 return (UCHAR)(tempdword>>8);
             }
             else
             {
                 rccode=-1;
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return 0;
             }
         }
      }
      rccode=-1;
      return 0;
}
APIRET BTI2CWriteAddr(PTVCARD pbttv,UCHAR i2caddr, UCHAR addr)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword&0xfffffffe; //write mode
    tempdword=tempdword<<24; //shift
    tempdword|=(addr<<16);
    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
    tempdword=tempdword|BT848_DATA_CTL_I2CNOSTOP&(~BT848_DATA_CTL_I2CNOS1B);
    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC&(~BT848_DATA_CTL_I2CW3BRA);
    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return 0;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 return -1;
             }
         }
      }
      return -1;
}
APIRET BTI2CWriteFirst(PTVCARD pbttv, UCHAR i2caddr, UCHAR data)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    // clear int stat
    dprintf(LOG_DEBUG3,"i2c write START %x %x \n",i2caddr,data);
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=i2caddr;
    tempdword=tempdword&0xfffffffe; //write mode
    tempdword=tempdword<<24; //shift
    tempdword|=(data<<16);
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword|=(BT848_DATA_CTL_I2CNOSTOP);
    tempdword&=(~BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword&=(~BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;
//    dprintf(LOG_DEBUG,"i2c data %x\n",tempdword);
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write got ACK \n",i2caddr);
                 return 0;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write %x: error: DONE without ACK \n",i2caddr);
                 return -1;
             }
         }
      }
      return -1;
}
APIRET BTI2CWrite(PTVCARD pbttv, UCHAR data)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    dprintf(LOG_DEBUG3,"i2c write %x \n",data);
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=data<<24; //shift
//    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
//    tempdword=tempdword|(BT848_DATA_CTL_I2CNOSTOP)|BT848_DATA_CTL_I2CNOS1B;
//    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC&(~BT848_DATA_CTL_I2CW3BRA);
//    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword|=(BT848_DATA_CTL_I2CNOSTOP);
    tempdword|=(BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword&=(~BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;

//    dprintf(LOG_DEBUG,"i2c data %x\n",tempdword);
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write got ACK\n");

                 return 0;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write error: DONE without ACK\n");
                 return -1;
             }
         }
      }
      return -1;
}
APIRET BTI2CWriteLast(PTVCARD pbttv, UCHAR data)
{
    UCHAR tempbyte;
    ULONG tempdword,i;
    dprintf(LOG_DEBUG3,"i2c write %x STOP\n",data);
    // clear int stat
    tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
    tempdword=tempdword&(BT848_INT_RACK|BT848_INT_I2CDONE);
    BTWriteRegister(pbttv,INT_STAT,tempdword,4);
    tempdword=data<<24; //shift
//    tempdword=tempdword|BT848_DATA_CTL_I2CMODE&(~BT848_DATA_CTL_I2CRATE);
//    tempdword=tempdword&(~BT848_DATA_CTL_I2CNOSTOP)|BT848_DATA_CTL_I2CNOS1B;
//    tempdword=tempdword|BT848_DATA_CTL_I2CSYNC&(~BT848_DATA_CTL_I2CW3BRA);
//    tempdword=tempdword|BT848_DATA_CTL_I2CSCL|BT848_DATA_CTL_I2CSDA;
    tempdword|=BT848_DATA_CTL_I2CMODE;
    tempdword&=(~BT848_DATA_CTL_I2CRATE);
    tempdword&=(~BT848_DATA_CTL_I2CNOSTOP);
    tempdword|=(BT848_DATA_CTL_I2CNOS1B);
    tempdword|=BT848_DATA_CTL_I2CSYNC;
    tempdword&=(~BT848_DATA_CTL_I2CW3BRA);
    tempdword|=BT848_DATA_CTL_I2CSCL;
    tempdword|=BT848_DATA_CTL_I2CSDA;

//    dprintf(LOG_DEBUG,"i2c data %x\n",tempdword);
    BTWriteRegister(pbttv,I2C_DATA_CTL,tempdword,4);
    /* wait for complete transaction */
    for (i=0x7fffffff;i;i--)
     {
       DosSleep(1);
       tempdword=BTReadRegister(pbttv,INT_STAT,4,rc);
       if (tempdword&BT848_INT_I2CDONE) //i2cdone
         {
             if (tempdword&BT848_INT_RACK) //i2cACK
             {
                 tempdword&=(BT848_INT_RACK|BT848_INT_I2CDONE);
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write got ACK\n");
                 return 0;
             }
             else
             {
                 tempdword&=BT848_INT_I2CDONE;
                 BTWriteRegister(pbttv,INT_STAT,tempdword,4);
                 dprintf(LOG_DEBUG3,"i2c write error: DONE without ACK\n");
                 return -1;
             }
         }
      }
      return -1;
}

