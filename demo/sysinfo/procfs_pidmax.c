#include <fcntl.h>
#include "light.h"

#define MAX_LINE 100

/*
 *	usage:
 *		$ su
 *		Password:
 *		# ./procfs_pidmax 10000
 *		Old value: 32768
 *		/proc/sys/kernel/pid_max now contains 10000
 */

int main(int argc, char *argv[])
{
	int fd;
	char line[MAX_LINE];
	ssize_t n;

	fd = open("/proc/sys/kernel/pid_max", (argc > 1) ? O_RDWR : O_RDONLY);
	if (fd == -1)
		err_exit("open");

	n = read(fd, line, MAX_LINE);
	if (n == -1)
		err_exit("read");

	if (argc > 1)
		printf("Old value: ");
	printf("%.*s\n", (int)n, line);

	if (argc > 1)
	{
		if (write(fd, argv[1], strlen(argv[1])) != strlen(argv[1]))
			fatal("write( failed");

		system("echo /proc/sys/kernel/pid_max now contains "
			"`cat /proc/sys/kernel/pid_max");
	}

	exit(EXIT_SUCCESS);
}