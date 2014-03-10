#include <signal.h>
#include <sys/wait.h>
#include "wait_status.h"
#include "curr_time.h"
#include "light.h"

/*
 * 	usage:
 *		$ ./multi_SIGCHLD 1 2 4
 */

static volatile int num_live_children = 0;

static void sig_child_handler(int sig)
{
	int status, saved_errno;
	pid_t child_pid;

	/* UNSAFE: This handler uses non-async-signal-safe functions 
	 * (printf(), print_wait_status(), curr_time())
	 */
	 saved_errno = errno; /* In case we modify 'errno' */
	 printf("%s handler: Caught SIGCHLD\n", curr_time("%T"));

	 while((child_pid = waitpid(-1, &status, WNOHANG)) > 0)
	 {
	 	printf("%s handler: Reaped child %ld - ", curr_time("%T"), (long)child_pid);
	 	print_wait_status(NULL, status);
	 	num_live_children--;
	 }
	 if (child_pid == -1 && errno != ECHILD)
	 	err_msg("waitpid");

	 printf("%s handler: return\n", curr_time("%T"));
}

int main(int argc, char *argv[])
{
	int j, sig_cnt;
	sigset_t block_mask, empty_mask;
	struct sigaction sa;

	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s child-sleep-time...\n", argv[0]);

	setbuf(stdout, NULL);
	sig_cnt = 0;
	num_live_children = argc - 1;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = sig_child_handler;
	if (sigaction(SIGCHLD, &sa, NULL) == -1)
		err_exit("sigaction");

	/* 
	 * Block SIGCHLD to prevent its delivery if a child terminates
     * before the parent commences the sigsuspend() loop below
	 */

	 sigemptyset(&block_mask);
	 sigaddset(&block_mask, SIGCHLD);
	 if (sigprocmask(SIG_SETMASK, &block_mask, NULL) == -1)
	 	err_exit("sigprocmask");

	 for (j = 1; j < argc; j++)
	 {
	 	switch(fork())
	 	{
	 		case -1:
	 			err_exit("fork");

	 		case 0:
	 			sleep(get_int(argv[j], GN_NONNEG, "child-sleep-time"));
	 			printf("%s Child %d (PID=%ld) exiting\n", curr_time("%T"), j, (long)getpid());
	 			_exit(EXIT_SUCCESS);

	 		default:
	 			break;
	 	}
	 }

	 /* Parent come here: wait for SIGCHLD until all children are dead */
	 while(num_live_children > 0)
	 {
	 	if (sigsuspend(&empty_mask) == -1 && errno != EINTR)
	 		err_exit("sigsuspend");
	 	sig_cnt++;
	 }

	 printf("%s All %d children have terminated; 
	 	SIGCHLD was caught %d times\n", curr_time("%T"), argc - 1, sig_cnt);
}