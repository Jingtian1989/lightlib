#include <sys/time.h>
#include <sys/resource.h>

#include "light.h"
/* Modifying and retrieving a process's nice value */
int main(int argc, char *argv[])
{
	int which, prio;
	id_t who;

	if (argc != 4 || strchr("pgu", argv[1][0]) == NULL)
		usage_err("%s {p|g|u} who priority\n"
			"	set priority of: p=process; g=process group;"
			"u=process for user\n", argv[0]);


	/* Set nice value according to command-line arguments */
	which = (argv[1][0] == 'p') ? PRIO_PROCESS : 
		(argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
		who = get_long(argv[2], 0, "who");
		prio = get_int(argv[3], 0, "prio");

	if (setpriority(which, who, prio) == -1)
		err_exit("getpriority");

	/* Retrive nice value to check the change */
	errno = 0;	/* Because successful call may return -1 */
	prio = getpriority(which, who);
	if (prio == -1 && errno != 0)
	{
		err_exit("getpriority");
	}
	printf("Nice value=%d\n", prio);

	exit(EXIT_SUCCESS);
}