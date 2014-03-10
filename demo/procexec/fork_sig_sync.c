#include <signal.h>
#include "light.h"
#include "curr_time.h" 	

#define SYNC_SIG SIGUSR1 /* Synchronization signal */ 	

static void handler(int sig)
{	
}


int main(int argc, char *argv[])
{
	pid_t child_pid;
	sigset_t block_mask, orig_mask, empty_mask;
	struct sigaction sa;

	setbuf(stdout, NULL); /* Disable buffering of stdout */

	sigemptyset(&block_mask);
	sigaddset(&block_mask, SYNC_SIG);
	if (sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) == -1)
		err_exit("sigprocmask");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if (sigaction(SYNC_SIG, &sa, NULL) == -1)
		err_exit("sigaction");

	switch(child_pid = fork())
	{
		case -1:
			err_exit("fork");

		case 0:
			printf("[%s %ld] Child started - doing some work\n", curr_time("%T"), (long)getpid());
			sleep(2);	/* Stimulate time spent doing some work */

			/* And then signals parent that it's done */
			printf("[%s %ld] Child about to signal parent\n", curr_time("%T"), (long)getpid());
			if (kill(getppid(), SYNC_SIG) == -1)
				err_exit("kill");

			_exit(EXIT_SUCCESS);

		default:
			printf("[%s %ld] Parent about to wait for signal\n", curr_time("%T"), (long) getpid());
			sigemptyset(&empty_mask);
			if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
				err_exit("sigsuspend");
			printf("[%s %ld] Parent got signal\n", curr_time("%T"), (long)getpid());

			/* If required, return signal mask to its original state */
			if (sigprocmask(SIG_SETMASK, &orig_mask, NULL) == -1)
				err_exit("sigprocmask");

			exit(EXIT_SUCCESS);
	}

}