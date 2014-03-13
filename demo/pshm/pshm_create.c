#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "light.h"


static void usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-cx] name size [octal-perms]\n", prog_name);
	fprintf(stderr, " -c  	Create shared memory (O_CREAT)\n");
	fprintf(stderr, " -x 	Create exclusively (O_EXCL)\n");
	exit(EXIT_FAILURE);
}

/* Creating a POSIX shared memory objec 
 * usage:
 *		$ ./pshm_create -c /demo_shm 10000
 *		$ ls -l /dev/shm
 *		total 0
 *		-rw-------    1 mtk      users       10000 Jun 20 11:31 demo_shm
 *	
 */
int main(int argc, char *argv[])
{
	int flags, opt, fd;
	mode_t perms;
	size_t size;
	void *addr;

	flags = O_RDWR;
	while((opt = getopt(argc, argv, "cx")) != -1)
	{
		switch (opt) 
		{
        	case 'c':   flags |= O_CREAT;           break;
        	case 'x':   flags |= O_EXCL;            break;
        	default:    usage_error(argv[0]);
        }
	}

	if (optind + 1 >= argc)
		usage_error(argv[0]);

	size = get_long(argv[optind + 1], GN_ANY_BASE, "size");
	perms = (argc <= optind + 2) ? (S_IRUSR | S_IWUSR) : get_long(argv[optind + 2], GN_BASE_8, "octal-perms");

	/* Create shared memory object and set its size */
	fd = shm_open(argv[optind], flags, perms);
	if (fd == -1)
		err_exit("shm_open");

	if (ftruncate(fd, size) == -1)
		err_exit("ftruncate");

	/* Map shared memory object */
	addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr = MAP_FAILED)
		err_exit("mmap");

	/* Do things here */


	if (shm_unlink(argv[optind]) == -1)
		err_exit("shm_unlink");

	if (munmap(addr, size) == -1)
		err_exit("munmap");
	exit(EXIT_SUCCESS);
}