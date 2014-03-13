#include <sys/time.h>
#include <sys/select.h>

#include "light.h"

static void usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s {timeout|-} fd-num[rw]...\n", prog_name);
	fprintf(stderr, " 	- means infinite timeout;\n");
	fprintf(stderr, "  	r = monitor for read\n");
	fprintf(stderr, "   w = monitor for write\n");
	fprintf(stderr, "   e.g. : %s - 0rw 1w\n", prog_name);
	exit(EXIT_FAILURE);
}

/* Demonstrate usage of select() 
 * 	usage:
 *		$ ./select 10 0r
 *		ready = 1
 *		0:r
 *		timeout after select():8.003
 *		$
 */
int main(int argc, char *argv[])
{
	fd_set read_fds, write_fds;
	int ready, nfds, fd, num_read, j;

	struct timeval timeout;
	struct timeval *pro;
	char buf[10]; 		/* Large enough to hold "rw\0" */

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_error(argv[0]);

	/* Timeout for select() is specified in argv[1] */

	if (strcmp(argv[1], "-") == 0)
		pro = NULL;
	else
	{
		pro = &timeout;
		timeout.tv_sec  = get_long(argv[1], 0, "timeout");
		timeout.tv_usec = 0; /* No microseconds */

		/* Process remaining arguments to build file descriptor sets */
		nfds = 0;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);

		for (j = 2; j < argc; j++)
		{
			num_read = sscanf(argv[j], "%d%2[rw]", &fd, buf);
			if (num_read != 2)
				usage_error(argv[0]);

			if (fd >= FD_SETSIZE)
				cmd_line_err("file descriptor exceeds limit (%d)\n", FD_SETSIZE);

			if (fd >= nfds)
				nfds = fd + 1;

			if (strchr(buf, 'r') != NULL)
				FD_SET(fd, &read_fds);

			if (strchr(buf, 'w') != NULL)
				FD_SET(fd, &write_fds);
		}
	}

	/* We've built all of the arguments; now call select() */
	ready = select(nfds, &read_fds, &write_fds, NULL, pto);
													/* Igonre exceptional events */

	if (ready == -1)
		err_exit("select");

	/* Display results of select */
	printf("ready = %d\n", ready);

	for (fd = 0; fd < nfds; fd++)
		printf("%d: %s%s\n", fd, FD_ISSET(fd, &read_fds) ? "r" : "", FD_ISSET(fd, &write_fds) ? "w" : "");

	if (pro != NULL)
		printf("timeout after select(): %ld.%03ld\n", (long) timeout.tv_sec, (long) timeout.tv_usec / 10000);

	exit(EXIT_SUCCESS);
}