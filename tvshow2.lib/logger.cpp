#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "logger.h"

char LogLevel;
char LogFile[255];
void myctime(time_t *tt_in, char *tt_out)
{
//    static char temp[30];
    struct tm tt1;
static char *months[] =
{
    "Янв",
    "Фев",
    "Мар",
    "Апр",
    "Май",
    "Июн",
    "Июл",
    "Авг",
    "Сен",
    "Окт",
    "Ноя",
    "Дек"
};


    tt1 = *localtime(tt_in);
    sprintf(tt_out,"%3d %.3s %.4d %.2d:%.2d:%.2d ",
            tt1.tm_mday,months[tt1.tm_mon],tt1.tm_year+1900,
            tt1.tm_hour,tt1.tm_min,tt1.tm_sec);

//    return temp;
 }

void dprintf(char MessLogLevel, const char *fmt,...)
{

    FILE *wv_log;
    va_list arg_ptr;
    time_t curtime;
    char temp[30];

    time(&curtime);
    myctime(&curtime,temp);
	if (MessLogLevel<=LogLevel)
    {
        wv_log = fopen(LogFile,"a");
        va_start(arg_ptr, fmt);
        fprintf(wv_log,"%s ",temp);
        switch (MessLogLevel)
            {
            case LOG_MESSAGE:
                break;
            case LOG_ERROR:
                fprintf(wv_log,"ERROR: ");
                printf("ERROR: ");
                break;
            case LOG_INFO:
                fprintf(wv_log,"INFO: ");
                printf("INFO: ");
                break;
            case LOG_DEBUG:
                fprintf(wv_log,"DEBUG: ");
                printf("DEBUG: ");
                break;
            case LOG_DEBUG2:
                fprintf(wv_log,"DEBUG2: ");
                printf("DEBUG2: ");
                break;
            case LOG_DEBUG3:
                fprintf(wv_log,"DEBUG3: ");
                printf("DEBUG3: ");
                break;

            }

        vfprintf(wv_log, fmt, arg_ptr);
	    vprintf(fmt, arg_ptr);
	    va_end(arg_ptr);
        fclose(wv_log);
    }
    return;
}
