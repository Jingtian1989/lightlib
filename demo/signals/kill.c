#include <signal.h>
#include "light.h"

int main(int argc, char *argv[])
{
	int s, sig;
	if (argc != 3 || strcmp(argv[1], "--help") == 0)
		usage_err("%s sig-num pid\n", arg[0]);

	sig = get_int(argv[2], 0, "sig-num");

	if (sig != 0)
	{
		if (s == -1)
			err_exit("kill");
	}else
	{
		if (s == 0)
		{
			printf("Process exists and we can send it a signal\n");
		}else
		{
			if (errno == EPERM)
				printf("Process exists, but we don't have permission to send it a signal\n");
			else if (errno == ESRCH)
				printf("Process does not exists\n");
			else
				err_exit("kill");
		}
	}

	exit(EXIT_SUCCESS);
}
