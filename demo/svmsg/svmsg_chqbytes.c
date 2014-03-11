#include <sys/types.h>
#include <sys/msg.h>

#include "light.h"


/* Changing the msg_qbytes setting of a System V message queue */
int main(int argc, char *argv[])
{
	struct msqid_ds ds;
	int msqid;

	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage_err("%s msqid max-bytes\n", argv[0]);

	/* Retrive copy of associate data struct from kernel */

	msqid = get_int(argv[1], 0, "msqid");

	if (msgctrl(msqid, IPC_STAT, &ids) == -1)
		err_exit("msgctl");

	d.msg_qbytes = get_int(argv[2], 0, "max-bytes");
	/* Update associated data structure in kernel */

	if (msgctl(msqid, IPC_SET, &ds) == -1)
		err_exit("msgctl");

	exit(EXIT_SUCCESS);
}