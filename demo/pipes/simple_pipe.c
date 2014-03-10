#include <sys/wait.h>
#include "light.h"

#define BUF_SIZE 10

/*
 *	Using a pipe to communicate between a parent and child process
 *	usage:
 *		./simple 'This is the communication string.'
 *	This is the communication string.
 */

int main(int argc, char *argv[])
{
	int pfd[2];
	char buf[BUF_SIZE];
	ssize_t num_read;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s string\n", argv[0]);

	if (pipe(pfd) == -1) 		/* Create the pipe */
		err_exit("pipe");

	switch(fork())
	{
		case -1:
		err_exit("fork");

		case 0:	/* Child - reads from pipe */
		if (close(pfd[1]) == -1)	/* Write end is unused */
			err_exit("close - child");

		for (;;)	/* Read data from pipe, echo on stdout */
		{	
			num_read = read(pfd[0], buf, BUF_SIZE);
			if (num_read == -1)
				err_exit("read");

			if (num_read == 0)
				break; 		/* End-of-file */

			if (write(STDOUT_FILENO, buf, num_read) != num_read)
				fatal("child - partial/failed write");
		}

		write(STDOUT_FILENO, "\n", 1);
		if (close(pfd[0]) == -1)
			err_exit("close");

		_exit(EXIT_SUCCESS);


		default:	/* Parent - writes to pipe */
			if (close(pfd[0]) == -1)	/* Read end is ununsed */
				err_exit("close - parent");

			if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
				fatal("parent - partial/failed write");

			if (close(pfd[1]) == -1) 	/* Child will see EOF */
				err_exit("close");

			wait(NULL);
			exit(EXIT_SUCCESS);

	}
}
