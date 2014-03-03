#include "light.h"

extern char **environ;

int main(int argc, char *argv[])
{
	char **ep;

	for (ep = environ; *ep != NULL; ep++)
	{
		puts(*ep);
	}

	exit(EXIT_SUCCESS);
}