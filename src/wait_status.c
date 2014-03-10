#define _GNU_SOURCE
#include <string.h>
#include <sys/wait.h>
#include "wait_status.h"
#include "light.h"

/* NOTE: The following function employs printf(), which is not 
 * async-signal-safe. As such, this function is also not async-signal-safe.
 */

 void print_wait_status(const char *msg, int status)
 {

 	if (msg != NULL)
 		printf("%s", msg);

 	if (WIFEXITED(status))
 	{
 		printf("child exited, status=%d\n", WEXITSTATUS(status));
 	}else if (WIFSIGNALED(status))
 	{
 		printf("child killed by signal %d (%s)\n", WSTOPSIG(status), strsignal(WSTOPSIG(status)));

 #ifdef WIFCONTINUED 
 	}else if (WIFCONTINUED(status))
 	{
 		printf("child continued\n");
 #endif 
 	} else
 	{		/* Should never happen */
 		printf("what happened to this child? (status=%x)\n", (unsigned int) status);
 	}
 	
 } 