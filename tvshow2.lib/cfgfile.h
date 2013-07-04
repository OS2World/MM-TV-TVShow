/*
** Module   :CFGFILE.H
** Abstract :
**
** Copyright (C) Sergey I. Yevtushenko
**
** Log: Mon  01/02/1999 Created
**
*/

#include "cfgbase.h"

#if __cplusplus
extern "C" {
#endif


#ifndef __CFGFILE_H
#define __CFGFILE_H

struct Variable;
typedef Variable* PVar;

typedef int (*Parser)(PVar, char *);

struct Variable
{
    char   *cName;    	/* variable name */
    int     iLen;      	/* variable size */
    Ptr     pData;     	/* variable itself */
    Parser  pParser; 	/* parser function */
    char   *pDelim;   	/* delimiters */
};

/* Various variable type parsers */

int Plong (PVar, char*);
int Pshort(PVar, char*);
int Pword (PVar, char*);
int Pdword(PVar, char*);
int Pstr  (PVar, char*);
int Pbool (PVar, char*);

/* Config loader */
int LoadConfig(char *cName, PVar pItemList);

#if __cplusplus
}
#endif


#endif  /*__CFGFILE_H*/

