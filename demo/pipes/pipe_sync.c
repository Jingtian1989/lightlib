#include "curr_time.h"
#include "light.h"

/*
 *	Using a pipe to synchronize multiple processes.
 *		usage:
 *			$ ./pipe_sync 4 2 6
 *			08:22:16  Parent started
 *			08:22:18  Child 2 (PID=2445) closing pipe
 *			08:22:20  Child 1 (PID=2444) closing pipe
 *			08:22:22  Child 3 (PID=2446) closing pipe
 *			08:22:22  Parent ready to go
 */

int main(int argc, char *argv[])
{
	int pfd[2]; 	/* Process synchronization pipe */
	int j, dummy;

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s sleep-time...\n", argv[0]);

	setbuf(stdout, NULL);

	printf("%s Parent started\n", curr_time("%T"));

	if (pipe(pfd) == -1)
		err_exit("pipe");

	for (j = 1; j < argc; j++)
	{
		switch(fork())
		{
			case -1:
				err_exit("fork %d", j);

			case 0: /* Child */
				if (close(pfd[0]) == -1) /* Read end is unused */
					err_exit("close");

					/* Child does some work, and lets parent know it's done */

					sleep(get_int(argv[j], GN_NONNEG, "sleep-time"));
					printf("%s Child %d (PID=%ld) closing pipe\n", curr_time("%T"), j, (long)getpid());

					_exit(EXIT_SUCCESS);

			default:
				break;
		}
	}	
	/* Parent comes here; close write end of pipe so we can see EOF */
	if (close(pfd[1]) == -1) 	/* Write end is unused*/
		err_exit("close");

	if (read(pfd[0], &dummy, 1) != 0)
		fatal("parent didn't get EOF");

	exit(EXIT_SUCCESS);

}