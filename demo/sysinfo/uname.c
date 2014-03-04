#define _GNU_SOURCE

#include <sys/utsname.h>
#include "light.h"

/*
 *	usage:
 *		$ ./t_uname
 *		Node name:   tekapo
 *		System name: Linux
 *		Release:     2.6.30-default
 *		Version:     #3 SMP Fri Jul 17 10:25:00 CEST 2009
 *		Machine:     i686
 */

int main(int argc, char *argv[])
{
	struct utsname uts;

	if (uname(&uts) == -1)
		err_exit("uname");

	printf("Node name: 		%s\n", uts.nodename);
	printf("System name:	%s\n", uts.sysname);
	printf("Release:		%s\n", uts.release);
	printf("Version:		%s\n", uts.version);
	printf("Machine:		%s\n", uts.machine);

	exit(EXIT_SUCCESS);
}
