/*
** Module   :CFGFILE.CPP
** Abstract :
**
** Copyright (C) Sergey I. Yevtushenko
**
** Log: Mon  01/02/1999 Created
**
*/

#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "cfgfile.h"

//-------------------------------------------------------------------

static char line[2048];
static char varname[2048];
static char varvalue[2048];
static char section[2048];

//-------------------------------------------------------------------

int LoadConfig(char *cName, PVar pItemList)
{
    FILE *fin;
    char *pline;
    char *str;
    char *ptr;

    int errors = 0;

    fin = fopen(cName, "r");

    if (!fin)
    {
        //syslog(LOG_WARNING, "Unable to load file %s\n", cName);
        return 1;
    }

    line[0] = varname[0] = section[0] = 0;

    while (!feof(fin))
    {
        int i;

        if(!fgets(line, sizeof(line), fin))
            break;

        pline = line;

        while(*pline && (*pline == ' ' || *pline == '\t'))
            pline++;

        if(!*pline         ||
            *pline == '\r' ||
            *pline == '\n' ||
            *pline == '#'  ||
            *pline == ';')
            continue;

        if(*pline == '[')   //start of new section
        {
            str=section;

            pline++;

            while(*pline && *pline != ']')
            {
                *str++ = (*pline++);
            }
            *str = 0;
            continue;
        }

        //construct name of variable

        str=varname;
        ptr=section;

        while(*ptr)
            *str++ = *ptr++;

        *str++ = '.';

        while(*pline &&
              *pline != ' '  &&
              *pline != '\t' &&
              *pline != '='  &&
              *pline != ':')
        {
            *str++ = *pline++;
        }

        *str=0;

        str = varvalue;

        while(*pline &&
                (*pline == ' '  ||
                 *pline == '\t' ||
                 *pline == '='  ||
                 *pline == ':'))
            pline++;

        while(*pline && *pline != '\r' && *pline != '\n')
            *str++ = *pline++;

        //Strip spaces at end of line

        while((str > varvalue) &&
              (str[-1] == ' ' ||
               str[-1] == '\t'))
            str--;

        *str = 0;

        //Check if variable name is 'include'.
        //If so, process that file

        if(strstr(varname, ".!"))
        {
            if(!stricmp(strstr(varname, ".!"), ".!include"))
            {
                LoadConfig(varvalue, pItemList);
                continue;
            }
        }

        for(i = 0; pItemList[i].cName; i++)
        {
            if(!stricmp(pItemList[i].cName, varname))
            {
                if(pItemList[i].pParser &&
                   pItemList[i].pData)
                {
                    if(pItemList[i].pParser(&pItemList[i], varvalue))
                    {
                        /*
                        syslog(LOG_WARNING, "Wrong variable value <%s> for variable <%s>\n",
                                varvalue,
                                varname);
                        */
                    }
                }
                else
                {
                    /*
                    syslog(LOG_ERR, "Internal error: missing parser or data pointer for <%s>\n",
                            varname);
                    */
                }
                break;
            }
        }

        if(!pItemList[i].cName)
        {
            //syslog(LOG_WARNING, "Unknown variable %s\n", varname);
            continue;
        }
    }

    fclose(fin);

    return 0;
}

dword AtoUL(char *str, int& rc)
{
    dword ulRes = 0;
    rc = 0;

    if(!str)
    {
        rc = 1;
        return 0;
    }

    //Skip spaces
    while(*str && (*str == ' ' || *str == '\t'))
    {
        str++;
    }

    if(!*str)
    {
        rc = 1;
        return 0;
    }
    long tmpres;
    char *stopstring;
    stopstring = str+1;
    if (*str == '0' && *stopstring == 'x') // hex value
    {
        tmpres = strtoul(str, &stopstring, 16);
        if (*stopstring != *(str + strlen(str)))
        {
                rc = 1;
                return 0;
            }
         return tmpres;
    }

    while(*str)
    {
        if(*str < '0' || *str > '9')
        {
            rc = 1;
            break;
        }

        ulRes *= 10;
        ulRes += *str - '0';
        str++;
    }

    return ulRes;
}

int AtoL(char *str, int& rc)
{
    rc = 0;

    if(!str)
    {
        rc = 1;
        return 0;
    }

    //Skip spaces
    while(*str && (*str == ' ' || *str == '\t'))
    {
        str++;
    }

    if(!*str)
    {
        rc = 1;
        return 0;
    }

    if(*str == '-')
        return -AtoUL(str+1, rc);

    return AtoUL(str, rc);
}

char *CutString(char *cStr, char *cDst, char *cDelimChrs)
{
    //Skip whitespaces

    while(*cStr && (*cStr == ' ' || *cStr == '\t'))
    {
        cStr++;
    }

    if(*cStr == '\'' || *cStr == '"') //quoted string
    {
        char cDelim = *cStr++;

        while(*cStr && *cStr != cDelim)
        {
            if(*cStr == '\\') //Escape sequence
                cStr++;

            *cDst++ = *cStr++;
        }

        *cDst = 0;

        if(*cStr == cDelim)
            cStr++;

        return cStr;
    }

    //not quoted string, split it by ':', ';' or ','

    while(*cStr && !strchr(cDelimChrs, *cStr))
    {
        *cDst++ = *cStr++;
    }

    if(*cStr)   //cStr points to delimiter
        cStr++;
    *cDst = 0;

    return cStr;
}

//---------------------------------------------------
// Parsers

int Plong(PVar pVar, char* cStr)
{
    int rc = 0;
    long var = AtoL(cStr, rc);

    if(rc)
        return rc;

    *(long *)pVar->pData = var;

    return 0;
}

int Pshort(PVar pVar, char* cStr)
{
    int rc = 0;
    long var = AtoL(cStr, rc);

    if(rc)
        return rc;

    if(var > SHRT_MAX || var < SHRT_MIN)
        return 1;

    *(short *)pVar->pData = var;

    return 0;
}

int Pword (PVar pVar, char* cStr)
{
    int rc = 0;
    dword var = AtoUL(cStr, rc);

    if(rc)
        return rc;

    if(var > USHRT_MAX)
        return 1;

    *(word *)pVar->pData = var;

    return 0;
}

int Pdword(PVar pVar, char* cStr)
{
    int rc = 0;
    dword var = AtoUL(cStr, rc);

    if(rc)
        return rc;

    *(dword *)pVar->pData = var;

    return 0;
}

int Pstr(PVar pVar, char* cStr)
{
    if(*cStr == '"' || *cStr == '\'')   /* Quoted string */
    {
        char cDelim = *cStr++;
        char *cPtr  = cStr;

        while(*cPtr && *cPtr != cDelim)
        {
            if(*cPtr == '\\') //Escape sequence
            {
                int iLen = strlen(cPtr+1);

                if(iLen)
                    memmove(cPtr, cPtr+1, iLen+1);
                else
                    return 2;   //Wrong escaping
            }

            cPtr++;
        }

        if(*cPtr == cDelim)
            *cPtr = 0;      //terminate string
        else
            return 2;       //missing quote
    }

    if(pVar->iLen)
    {
        int iLn = strlen(cStr);

        if(iLn >= pVar->iLen)
            return 2;

        strncpy((char *)pVar->pData, cStr, pVar->iLen);
    }
    else
    {
        char *cData;

        cData = new char[strlen(cStr)+1];

        strcpy(cData, cStr);

        *((char **)pVar->pData) = cData;
    }

    return 0;
}

int Pbool(PVar pVar, char* cStr)
{
    if(!stricmp(cStr, "0")         ||

       !stricmp(cStr, "no")        ||
       !stricmp(cStr, "off")       ||
       !stricmp(cStr, "false")     ||

       !stricmp(cStr, "'no'")      ||
       !stricmp(cStr, "'off'")     ||
       !stricmp(cStr, "'false'")   ||

       !stricmp(cStr, "\"no\"")    ||
       !stricmp(cStr, "\"off\"")   ||
       !stricmp(cStr, "\"false\"") ||
       0)
    {
        *(int *)pVar->pData = 0;
        return 0;
    }

    if(!stricmp(cStr, "1")         ||

       !stricmp(cStr, "on")        ||
       !stricmp(cStr, "yes")       ||
       !stricmp(cStr, "true")      ||

       !stricmp(cStr, "'on'")      ||
       !stricmp(cStr, "'yes'")     ||
       !stricmp(cStr, "'true'")    ||

       !stricmp(cStr, "\"on\"")    ||
       !stricmp(cStr, "\"yes\"")   ||
       !stricmp(cStr, "\"true\"")  ||
       0)
    {
        *(int *)pVar->pData = 1;
        return 0;
    }

    return 1;
}

