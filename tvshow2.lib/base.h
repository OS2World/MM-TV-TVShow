/*
** Module   :BASE.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Tue  15/10/2002 Created
**
*/
#ifndef __BASE_H
#define __BASE_H

#if __cplusplus
extern "C" {
#endif

#include "logger.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define INCL_BASE
#define INCL_DOS
#define INCL_DOSDEVICES
#define INCL_DOSDEVIOCTL
#define INCL_DOSERRORS
#define INCL_DOSNMPIPES
#define INCL_DOSPROCESS
#include <os2.h>


#define ASZ(s)  (sizeof(s)/sizeof(s[0]))

#if __cplusplus
}
#endif


#endif  /*__BASE_H*/

