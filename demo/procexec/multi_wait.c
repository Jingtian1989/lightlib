#include <sys/wait.h>
#include <time.h>
#include "curr_time.h"
#include "light.h"
/*
 *	$ ./multi_wait 7 1 4
 *	[13:41:00] child 1 started with PID 21835, sleeping 7 seconds
 *	[13:41:00] child 2 started with PID 21836, sleeping 1 seconds
 *	[13:41:00] child 3 started with PID 21837, sleeping 4 seconds
 *	[13:41:01] wait() returned child PID 21836 (numDead=1)
 *	[13:41:04] wait() returned child PID 21837 (numDead=2)
 *	[13:41:07] wait() returned child PID 21835 (numDead=3)
 *	No more children - bye!
 */


int main(int argc, char *argv[])
{
	int num_dead; /* Number of children so far waited for */
	pid_t child_pid;
	int j;

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s sleep-time...\n", argv[0]);
	setbuf(stdout, NULL); /* Disable buffering of stdout */

	for (j = 1; j < argc; j++)
	{
		switch(fork())
		{
			case -1:
				err_exit("fork");

			case 0:		/* Child sleeps for a while then exits */
				printf("[%s] child %d started with PID %ld, sleeping %s ", "seconds\n", 
					curr_time("%T"), j, (long)getpid(), argv[j]);
				sleep(get_int(argv[j], GN_NONNEG, "sleep-time"));
				_exit(EXIT_SUCCESS);

			default:
				break;			
		}
	}

	num_dead = 0;
	for (;;)
	{
		child_pid = wait(NULL);
		if (child_pid == -1)
		{
			if (errno == ECHILD)
			{
				printf("No More children - bye!\n");
				exit(EXIT_SUCCESS);
			}else
			{ 			/* Some other (unexpected) error */
				err_exit("wait");
			}
		}

		num_dead++;
		printf("[%s] wait() returned child PID %ld (num_dead=%d)\n", curr_time("%T"), (long)child_pid, num_dead);
	}


}