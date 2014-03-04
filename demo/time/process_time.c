#include <sys/times.h>
#include <time.h>
#include "light.h"

/*
 *	clock():
 * 		The value returned by  clock()  is measured in units of CLOCKS_PER_SEC , so we must divide
 *		by this value to arrive at the number of seconds of CPU time used by the process.
 *
 *	times():
 *		The  clock_t  data type used to type the four fields of the  'tms'  structure is an integer
 *		type that measures time in units called  clock ticks . We can call  sysconf(_SC_CLK_TCK)
 *		to obtain the number of clock ticks per second, and then divide a  clock_t  value by
 *		this number to convert to seconds.
 */

static void display_process_time(const char *msg)
{
	struct tms t; 
	clock_t clock_time;
	static long clock_ticks = 0;

	if (msg != NULL)
		printf("%s", msg);

	if (clock_ticks == 0)
	{
		clock_ticks = sysconf(_SC_CLK_TCK); /* Obtains the number of clock ticks per second */
		if (clock_ticks == -1)
			err_exit("sysconf");
	}

	clock_time = clock();
	if (clock_time == -1)
		err_exit("clock");

	printf("	clock() returns: %ld clocks-per-sec (%.2f secs)\n", (long)clock_time, 
		(double)clock_time / CLOCKS_PER_SEC);

	if (times(&t) == -1)
		err_exit("times");
	pritnf("	times() yields: user CPU=%.2f; system CPU: %.2f\n", (double) t.tms_utime / clock_ticks,
		(double) t.tms_stime / clock_ticks);

}

/*
 *	usage:
 *		$ ./process_time 10000000
 *			CLOCKS_PER_SEC=1000000  sysconf(_SC_CLK_TCK)=100
 *			At program start:
 *      		clock() returns: 0 clocks-per-sec (0.00 secs)
 *       		times() yields: user CPU=0.00; system CPU: 0.00
 *			After getppid() loop:
 *      		clock() returns: 2960000 clocks-per-sec (2.96 secs)
 *       		times() yields: user CPU=1.09; system CPU: 1.87
 */
int main(int argc, char *argv[])
{
	int num_calls, j;
	printf("CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n", (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

	display_process_time("At program start:\n");
	num_calls = (argc > 1) ? get_int(argv[1], GN_GT_0, "num-calls") : 10000000;

	for (j = 0; j < num_calls; j++)
		getppid();

	display_process_time("After getppid() loop:\n");

	exit(EXIT_SUCCESS);
}