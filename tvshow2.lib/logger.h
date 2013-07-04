/*
** Module   :LOGGER.H
** Abstract :
**
** Copyright (C) Alex Cherkaev
**
** Log: Thu  24/10/2002 Created
**
*/
#ifndef __LOGGER_H
#define __LOGGER_H

#if __cplusplus
extern "C" {
#endif


#define LOG_MESSAGE 0
#define LOG_ERROR   1
#define LOG_INFO    2
#define LOG_DEBUG   3
#define LOG_DEBUG2  4
#define LOG_DEBUG3  5

extern char LogLevel;
extern char LogFile[255];


void dprintf(char LogLevel, const char *fmt,...);

#if __cplusplus
}
#endif

#endif  /*__LOGGER_H*/

