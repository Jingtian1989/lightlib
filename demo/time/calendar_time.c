#include <locale.h>
#include <time.h>
#include <sys/time.h>
#include "light.h"

#define SECONDS_IN_TROPICAL_YEAR (365.24219 * 24 * 60 * 60)


/*
 *	usage:
 *		$ ./calendar_time
 */
int main(int argc, char *argv[])
{
	time_t t;
	struct tm *gmp, *locp;
	struct tm gm, loc;
	struct timeval tv;

	t = time(NULL);
	printf("Seconds since the Epoch (1 Jan 1970): %ld", (long) t);
	printf(" (about %6.3f year)\n", t / SECONDS_IN_TROPICAL_YEAR);

	if (gettimeofday(&tv, NULL) == -1)
		err_exit("gettimeofday");
	printf("	gettimeofday() return %ld secs, %ld microsecs\n", (long) tv.tv_sec, (long) tv.tv_usec);

	gmp = gmtime(&t);
	if (gmp == NULL)
		err_exit("gmtime");

	gm = *gmp; /* Save local copy, since *gmp may be modified by asctime() or gmtime() */
	printf("Broken down by gmtime(): \n");
	printf("	year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", gm.tm_year, 
		gm.tm_mon, gm.tm_mday, gm.tm_hour, gm.tm_min, gm.tm_sec);
	printf("wday=%d yday=%d isdst=%d\n", gm.tm_wday, gm.tm_yday, gm.tm_isdst);

	locp = localtime(&t);
	if (locp == NULL) 
		err_exit("localtime");

	loc = *locp; /* Save local copy */

	printf("Broken down by localtime():\n");
	printf("	year=%d mon=%d mday=%d hour=%d min=%d sec=%d ", loc.tm_year,
		loc.tm_mon, loc.tm_mday, loc.tm_hour, loc.tm_min, loc.tm_sec);
	printf("wday=%d yday=%d isdst=%d\n\n", loc.tm_wday, loc.tm_yday, loc.tm_isdst);

	printf("asctime() formats the gmtime value as: %s", asctime(&gm));
	printf("ctime() formats the time() value as:   %s", ctime(&t));

	printf("mktime() of gmtime() value: 	%ld secs\n", (long) mktime(&gm));
	printf("mktime() of localtime() value:	%ld secs\n", (long) mktime(&loc));

	exit(EXIT_SUCCESS);
}