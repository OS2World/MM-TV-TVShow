/*
** Module   :BTI2C.H
** Abstract :I2C support for Bt chip
**
** Copyright (C) Alex Cherkaev
**
** Log: Fri  10/05/2002 Created
**
*/

#ifndef __BTI2C_H
#define __BTI2C_H

#if __cplusplus
extern "C" {
#endif


#include "base.h"
#include "card.h"
//read one byte from i2c
//rc=0 - all ok, returned data
//rc=-1 - can't read, returned 0

UCHAR BTI2CRead1(PTVCARD pbttv,UCHAR i2caddr, APIRET &rc);
APIRET BTI2CWrite4(PTVCARD pbttv,UCHAR i2caddr, UCHAR data1, UCHAR data2, UCHAR data3, UCHAR data4);
APIRET BTI2CWrite2(PTVCARD pbttv,UCHAR i2caddr, UCHAR data1, UCHAR data2);
UCHAR BTI2CReadFirst(PTVCARD pbttv, UCHAR i2caddr, APIRET &rccode);
UCHAR BTI2CRead(PTVCARD pbttv, APIRET &rccode);
UCHAR BTI2CReadLast(PTVCARD pbttv, APIRET &rccode);
APIRET BTI2CWriteFirst(PTVCARD pbttv, UCHAR i2caddr, UCHAR data);
APIRET BTI2CWrite(PTVCARD pbttv, UCHAR data);
APIRET BTI2CWriteLast(PTVCARD pbttv, UCHAR data);

APIRET BTI2CWriteAddr(PTVCARD pbttv,UCHAR i2caddr, UCHAR addr);
#if __cplusplus
}
#endif

#endif  /*__BTI2C_H*/

