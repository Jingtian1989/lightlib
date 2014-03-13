#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "light.h"

/*
 * 	Demonstrates the use of mmap() to create a private file mapping. This
 *	program is a simple version of cat(1). It maps the (entire) file named 
 *	in its command-line argument, and then writes the conten ts of the 
 *	mapping to standard output.
 */

int main(int argc, char *argv[])
{
	char *addr;
	int fd;
	struct stat sb;
	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s file\n", argv[0]);
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
		err_exit("open");

	/* Obtain the size of the file and use it to specify the size of 
     * mapping and the size of the buffer to be written.
	 */

	if (fstat(fd, &sb) == -1)
		err_exit("fstat");

	addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (addr == MAP_FAILED)
		err_exit("mmap");

	if (write(STDOUT_FILENO, addr, sb.st_size) != sb.st_size)
		fatal("partial/failed write");

	if (munmap(addr, sb.st_size) == -1)
		err_exit("munmap");
	
	exit(EXIT_SUCCESS);
}
