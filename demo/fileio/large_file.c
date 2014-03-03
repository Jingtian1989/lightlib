#define _LARGEFILE64_SOURCE

#include <sys/stat.h>
#include <fcntl.h>
#include "light.h"

/*
 *	$ ./large_file x 10111222333
 */

int main(int argc, char *argv[])
{
	
	int fd;
	off64_t off;
	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage_err("%s pathname offset\n", argv[0]);

	if (fd == -1)
		err_exit("open64");

	off = atoll(argv[2]);
	if (lseek64(fd, off, SEEK_SET) == -1)
		err_exit("lseek64");

	if (write(fd, "test", 4) == -1)
		err_exit("write");

	exit(EXIT_SUCCESS);
}