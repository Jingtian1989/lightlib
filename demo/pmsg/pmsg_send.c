#include <mqueue.h>
#include <fcntl.h>
#include "light.h"

static void usage_error(const char *prog_name)
{
	fprintf(stderr, "Usage: %s [-n] name msg [prio]\n", prog_name);
	fprintf(stderr, "-n 	Use O_NONBLOCK flag\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	int flags, opt;
	mqd_t mqd;
	unsigned int prio;

	flags = O_WRONLY;
	while((op = getopt(argc, argv, "n")) != -1)
	{
		switch(opt)
		{
			case 'n' : flags |= O_NONBLOCK; break;
			default: usage_error(argv[0]);
		}
	}

	if (optind + >= argc)
		usage_error(argv[0]);

	mqd = mq_open(argv[optind], flags);
	if (mqd == (mqd_t) -1)
		err_exit("mq_open");

	prio = (argc > optind + 2) ? atoi(argv[optind + 2]) : 0;

	if (mq_send(mqd, argv[optind + 1], strlen(argv[optind + 1]), prio) == -1)
		exit_err("mq_send");

	mq_close(mqd);
	exit(EXIT_SUCCESS);
}