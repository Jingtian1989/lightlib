#include <sys/types.h>
#include <sys/msg.h>

#include "light.h"

#define MAX_MTEXT 1024

struct mbuf
{
	long mtype;	/* Message type */
	char mtext[MAX_MTEXT]; /* Message body */
};

/* Using msgsnd() to send a message */

static void usage_error(const char *prog_name, const char *msg)
{
	if (msg != NULL)
		fprintf(stderr, "%s", msg);

	fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", prog_name);
	fprintf(stderr, "	-n 	Use IPC_NOWAIT flag\n");
	exit(EXIT_FAILURE);
}


int main(int argc, char *argv[])
{
	int msqid, flags, msglen;
	struct mbuf msg;

	/* Parse command-line options and arguments */
	flags = 0;
	while ((opt = getopt(argc, argv, "n")) != -1)
	{
		if (opt == 'n')
			flags |= IPC_NOWAIT;
		else
			usage_error(argv[0], NULL);
	}

	if (argc < optind + 2 || argc > optind + 3)
		usage_error(argv[0], "Wrong number of arguments\n");

	msqid = get_int(argv[optind], 0, "msqid");
	msg.mtype = get_int(argv[optind + 1], 0, "msg-type");

	if (argc > optind + 2) 	/* 'msg-text' was supplied */
	{
		msglen = strlen(argv[optind + 2]) + 1;
		if (msglen > MAX_MTEXT)
			cmd_line_err("msg-text too long (max: %d characters)\n", MAX_MTEXT);

		memcpy(msg.mtext, argv[optind + 2], msglen);
	} else
	{
		msglen = 0; 			/* No 'msg-text' ==> zero-length msg */
	}

	/* Send message */
	if (msgsnd(msqid, &msg, msglen, flags) == -1)
	{
		err_exit("msgsnd");
	}

	exit(EXIT_SUCCESS);

}

