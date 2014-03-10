#include <signal.h>
#include "light.h"

#define BUF_SIZE 200

/* SIGALRM handler: used to interrupts blocked system call */
static void handler(int sig)
{
	printf("Caught signal\n"); 	/* UNSAFE*/
}
/* Performing a read() with timeout */
int main(int argc, char *argv[])
{
	struct sigaction sa;
	char buf[BUF_SIZE];
	ssize_t num_read;
	int saved_errno;

	if (argc > 1 && strcmp(argv[1], "--help") == 0)
		usage_err("%s [num-secs [restart-flag]]\n", argv[0]);

	/* Set up handler for SIGALRM. Allow system calls to be interrupted, 
	   unless second command-line argument supplied */
	sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler;
	if (sigaction(SIGALRM, &sa, NULL) == -1)
		err_exit("sigaction");

	alarm((argc > 1) ? get_int(argv[1], GN_NONNEG, "num-secs") : 10);
	num_read = read(STDIN_FILENO, buf, BUF_SIZE-1);

	saved_errno = errno;/* In case alarm() changes it */
	alarm(0);  /* Ensure timer is turned off */
	errno = saved_errno;

	/* Determine result of read() */
	if (num_read == -1)
	{
		if (errno == EINTR)
			printf("Read time out\n");
		else
			err_msg("read");
	}else
	{
		printf("Successful read (%ld bytes): %.*s", (long)num_read, (int)num_read, buf);
	}

	exit(EXIT_SUCCESS);


}