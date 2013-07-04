/*
** Module   :BTI2CS.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  22/07/2003 Created
**
*/

#ifndef __BTI2CS_H
#define __BTI2CS_H

#if __cplusplus
extern "C" {
#endif

#include "base.h"
#include "card.h"
#include "btregs.h"
#include "btfuncs.h"


#define I2C_DELAY 16
#define MSPWAITACK 16
#define I2C_TIMING (0x7<<4)
#define I2C_COMMAND (I2C_TIMING | BT848_I2C_SCL | BT848_I2C_SDA)

#define I2C_TSA5522        0xc2
#define I2C_TDA7432        0x8a
#define I2C_TDA8425        0x82
#define I2C_TDA9840        0x84
#define I2C_TDA9850        0xb6 // also used by 9855,9873
#define I2C_TDA9875        0xb0
#define I2C_HAUPEE         0xa0
#define I2C_STBEE          0xae
#define I2C_VHX            0xc0
#define I2C_MSP3400        0x80
#define I2C_TEA6300        0x80
#define I2C_DPL3518        0x84

void BTI2CBusSet(PTVCARD pbttv,BOOL SCL, BOOL SDA);
BOOL BTI2CBusGet(PTVCARD pbttv);
void BTI2CBusStart(PTVCARD pbttv);
void BTI2CBusStop(PTVCARD pbttv);
void BTI2CBusOne(PTVCARD pbttv);
void BTI2CBusZero(PTVCARD pbttv);
BOOL BTI2CBusAck(PTVCARD pbttv);
BOOL BTI2CBusSendByte(PTVCARD pbttv,BYTE nData, int nWaitForAck);
BYTE BTI2CBusReadByte(PTVCARD pbttv,BOOL bLast);
void BTI2CBusWait(int us);

#if __cplusplus
}
#endif
#endif  /*__BTI2CS_H*/

