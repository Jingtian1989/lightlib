#include <light.h>

/*
 * 	usage:
 *		$ ./vfork
 *		Child executing 		
 *		Parent executing
 *		istack = 666
 *
 *	Semantic features that distinguish it from fork(): the child shares the parent's memory,
 *	and the parent is suspended until child terminates or calls exec().
 */

int main(int argc, char *argv[])
{
	int istack = 222;
	switch(vfork())
	{
		case -1:
			err_exit("vfork");

		case 0:
			sleep(3);	/* Child executes first, in parent's memory space */
						/* Even if we sleep for a while parent still is not scheduled */
			write(STDOUT_FILENO, "Child executing\n", 16);
			istack *= 3;	/* This change will be seen by parent */
			_exit(EXIT_SUCCESS);

		default: 		/* Parent is blocked until child exits */
			write(STDOUT_FILENO, "Parent executing\n", 17);
			printf("istack = %d\n", istack);
			exit(EXIT_SUCCESS);


	}
}