#ifdef USE_MAP_ANON
#define _BSD_SOURCE 	/* Get MAP_ANONYMOUS definition */
#endif 


#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "light.h"

/*	usage:
 *		$ ./anon_mmap
 *		Child started, value = 1
 *		In parent, value = 2
 */

int main(int argc, char *argv[])
{
	int *addr;

	#ifdef USE_MAP_ANON		/* Use MAP_ANONYMOUS */
	addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (addr == MAP_FAILED)
		err_exit("mmap");

	#else 	/* Map /dev/zero */

	int fd;
	fd = open("/dev/zero", O_RDWR);
	if (fd == -1)
		err_exit("open");
	addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (addr == MAP_FAILED)
		err_exit("mmap");
	if (close(fd) == -1) 	/* No longer needed */
		err_exit("close");
	#endif

	*addr = 1;
	switch(fork())
	{
		case -1:
			err_exit("fork");

		case 0:
			printf("Child started, value=%d\n", *addr);
			(*addr)++;
			if (munmap(addr, sizeof(int)) == -1)
				err_exit("munmap");
			_exit(EXIT_SUCCESS);

		default:
			if (wait(NULL) == -1)
				err_exit("wait");
			printf("In parent, value=%d\n", *addr);

			if (munmap(addr, sizeof(int)) == -1)
				err_exit("munmmap");

			exit(EXIT_SUCCESS);
	}

}