#include <sys/types.h>
#include <sys/msg.h>
#include "light.h"

int main(int argc, char *argv[])
{
	int j;
	if (argc > 1 && strcmp(argv[1], "--help") == 0)
		usage_err("%s [msqid...]\n", argv[0]);

	for (j = 1; j < argc; j++)
		if (msgctl(get_int(argv[j], 0, "msqid"), IPC_RMID, NULL) == -1)
			err_exit("msgctl %s", argv[j]);

	exit(EXIT_SUCCESS);
}