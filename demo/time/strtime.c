#define _XOPEN_SOURCE
#include <time.h>
#include <locale.h>
#include "light.h"

/*
 *  usage:
 *      $ ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y"
 *          calendar time (seconds since Epoch): 1296592786
 *          strftime() yields: 21:39:46 Tuesday, 01 February 2011 CET
 *  
 *      The following usage is similar, but this time we explic itly specify a format for strftime():
 *
 *      $ ./strtime "9:39:46pm 1 Feb 2011" "%I:%M:%S%p %d %b %Y" "%F %T"
 *          calendar time (seconds since Epoch): 1296592786
 *          strftime() yields: 2011-02-01 21:39:46
 */
#define SBUF_SIZE 1000
int main(int argc, char *argv[])
{
    struct tm tm;
    char sbuf[SBUF_SIZE];
    char *ofmt;
    if (argc < 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s input-date-time in-format [out-format]\n", argv[0]);
    if (setlocale(LC_ALL, "") == NULL)
        errExit("setlocale");   /* Use locale settings in conversions */
    memset(&tm, 0, sizeof(struct tm));          /* Initialize 'tm' */
    if (strptime(argv[1], argv[2], &tm) == NULL)
        fatal("strptime");
    tm.tm_isdst = -1;           /* Not set by strptime(); tells mktime()
                                   to determine if DST is in effect */
    printf("calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));
    ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
    if (strftime(sbuf, SBUF_SIZE, ofmt, &tm) == 0)
        fatal("strftime returned 0");
    printf("strftime() yields: %s\n", sbuf);
    exit(EXIT_SUCCESS);
}