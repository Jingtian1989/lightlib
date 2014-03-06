#include <signal.h>
#include "light.h"

static void sig_handler(int sig)
{
	static int count = 0;
	/* UNSAFE: This handler uses non-async-signal-safe functions
       (printf(), exit();*/
	if (sig == SIGINT)
	{
		count++;
		printf("Caught SIGINT (%d)\n", count);
		return;
	}

	printf("Caught SIGQUIT - that's all folks!\n");
	exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[])
{
	if (signal(SIGINT, sig_handler) == SIG_ERR)
		err_exit("signal");

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		err_exit("signal");

	for(;;)
		pause();
}