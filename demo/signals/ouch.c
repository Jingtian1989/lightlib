#include <signal.h>
#include "light.h"

static void sig_handler(int sig)
{
	printf("Ouch!\n");
}

int main(int argc, char *argv[])
{
	int j;

	if (signal(SIGINT, sig_handler) == SIG_ERR)
		err_exit("signal");

	for (j = 0; ; j++)
	{
		printf("%d\n", j);
		sleep(3);
	}
}