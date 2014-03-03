#define _GNU_SOURCE

#include <stdlib.h>
#include "light.h"

/*
 *	usage:
 *		$ ./modify_env "GREET=Guten Tag" SHELL=/bin/bash BYE=Ciao
 */

extern char **environ;

int main(int argc, char *argv[])
{
	int j;
	char **ep;

	clearenv(); /* Erase entire environment */

	for (j = 0; j < argc; j++)
		if (putenv(argv[j]) != 0)
			err_exit("putenv: %s", argv[j]);

	if (setenv("GREET", "Hello world", 0) == -1)
		err_exit("setenv");

	unsetenv("BYE");

	for (ep = environ; *ep != NULL; ep++)
		puts(*ep);

	exit(EXIT_SUCCESS);

}