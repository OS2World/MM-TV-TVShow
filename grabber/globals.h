/*
** Module   :GLOBALS.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Wed  22/01/2003 Created
**
*/
#ifndef __GLOBALS_H
#define __GLOBALS_H

#if __cplusplus
extern "C" {
#endif
#include "card.h"
#include "videocfg.h"
#include "memory.h"

extern TVCARD bttv;
extern PTVCARD pbttv;
extern CFGDATA cfg;
extern char *framebuf;
extern char *riscbuf;
extern VIDEOFRAME OddFrame,EvenFrame;
#if __cplusplus
}
#endif
#endif  /*__GLOBALS_H*/

