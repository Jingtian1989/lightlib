#include <fcntl.h>
#include "light.h"

/* Setting the close-on-exec flag for a file descriptor */

/* When dup(), dup2(), or fcntl() is used to create a duplicate of a file descriptor, 
 * the close-on-exec flag is always cleared for the duplicate descriptor.
 */
int main(int argc, char *argv[])
{
	int flags;

	if (argc > 1)
	{
		flags = fcntl(STDOUT_FILENO, F_GETFD);
		if (flags == -1)
			err_exit("fcntl - F_GETFD");

		flags |= FD_CLOEXEC; 		/* Turn on FD_CLOEXEC */

		if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
			err_exit("fcntl - F_SETFD");
	}
}