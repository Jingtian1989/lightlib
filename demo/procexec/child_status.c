#include <sys/wait.h>
#include "wait_status.h"
#include "light.h"

int main(int argc, char *argv[])
{
	int status;
	pid_t child_pid;

	if (argc > 1 && strcmp(argv[1], "--help") == 0)
		usage_err("%s [exit-status]\n", argv[0]);

	switch(fork())
	{
		case -1:
			err_exit("fork");

		case 0: 			/* Child: either exits immediately with given
							 * status or loops waiting for signals
							 */
			printf("Child started with PID = %ld\n", (long)getpid());
			if (argc > 1)			/* Status supplied on command line? */
				_exit(get_int(argv[1], 0, "exit-status"));
			else
				for (;;)
					pause();
			_exit(EXIT_FAILURE);

		default:
			for (;;)
			{
				child_pid = waitpid(-1, &status, WUNTRACED
				#ifdef WCONTINUED 	/* Not present on older versions of Linux */
						| WCONTINUED
				#endif
					);
				if (child_pid == -1)
					err_exit("waitpid");

				/* Print status in hex, and as seperate decimal bytes */
				printf("waitpid() return: PID=%ld; status=0x%0x4x (%d,%d)\n",
					(long)child_pid, (unsigned int)status, status >> 8, status & 0xff);
				print_wait_status(NULL, status);

				if (WIFEXITED(status) || WIFSIGNALED(status))
					exit(EXIT_SUCCESS);
			}
	}
}