#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "errno.h"

int system(const char *command)
{
	sigset_t block_mask, orig_mask;
	struct sigaction sa_ignore, sa_orig_quit, sa_orig_int, sa_default;

	pid_t child_pid;
	int status, saved_errno;

	if (command == NULL) /* Is a shell available */
		return system(":") == 0;

	sigemtpyset(&block_mask); 	/* Parent should block SIGCHLD, let child handle SIGCHLD  */
	sigaddset(&block_mask, SIGCHLD);
	sigprocmask(SIG_BLOCK, &block_mask, &orig_mask);

	sa_ignore.sa_handler = SIG_IGN; /* Parent should ignore SIGINT and SIGQUIT */
	sa_ignore.sa_flags = 0;
	sigemtpyset(&sa_ignore.sa_mask);
	sigaction(SIGINT, &sa_ignore, &sa_orig_int);
	sigaction(SIGQUIT, &sa_ignore, &sa_orig_quit);

	switch(child_pid = fork())
	{
		case -1:
			status = -1;
			break;

		case 0: /* Child: exec command */
			sa_default.sa_handler = SIG_DFL; /* Before exec command, should restore its default signal handler for portablity */
			sa_default.sa_flags = 0;
			sigemtpyset(&sa_default.sa_mask);

			if (sa_orig_int.sa_handler != SIG_IGN) 
				sigaction(SIGINT, &sa_default, NULL);
			if (sa_orig_quit.sa_handler != SIG_IGN)
				sigaction(SIGQUIT, &sa_default, NULL);

			sigprocmask(SIG_SETMASK, &orig_mask, NULL);

			execl("/bin/sh", "sh", "-c", command, (char *)NULL);
			_exit(127);

		default: /* Parent: wait for our child to terminate */
			while(waitpid(child_pid, &status, 0) == -1)
			{
				if (errno != EINTR) /* Error other than EINTR*/
				{
					status = -1;
					break;
				}
			}
			break;

	}

	saved_errno = errno;
	sigprocmask(SIG_SETMASK, &orig_mask, NULL);
	sigaction(SIGINT, &sa_orig_int, NULL);
	sigaction(SIGQUIT, &sa_orig_quit, NULL);

	errno = saved_errno;

	return status;


}