#include <sys/file.h>
#include <fcntl.h>
#include "curr_time.h"
#include "light.h" 		


/*
 *	Using flock()
 */

 int main(int argc, char *argv[])
 {
 	int fd, lock;
 	const char *lname;

 	if (argc < 3 || strcmp(argv[1], "--help") == 0 || 
 		strchr("sx", argv[2][0]) == NULL)
 		usage_err("%s file lock [sleep-time]\n"
 			" 'lock' is 's' (shared) or 'x' (exclusive)\n"
 			"	optionally followed 'n' (nonblocking)\n"
 			"	'secs' specifies time to hold lock\n"
 			, argv[0]
 			);

 	lock = (argv[2] == 's') ? LOCK_SH : LOCK_EX;
 	if (argv[2][1] == 'n')
 		lock |= LOCK_NB;

 	fd = open(argv[1], O_RDONLY); /* Open file to be locked */
 	if (fd == -1)
 		err_exit("open");

 	lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

 	printf("PID %ld: requesting %s at %s\n", (long) getpid(), lname, curr_time("%T"));

 	if (flock(fd, lock) == -1)
 	{
 		if (errno == EWOULDBLOCK)
 			fatal("PID %ld: already locked - bye!", (long) getpid());
 		else
 			err_exit("flock (PID=%ld)", (long) getpid());
 	}

 	printf("PID %ld: granted 	%s at %s\n", (long)getpid(), lname, curr_time("%T"));

 	sleep((argc > 3) ? get_int(argv[3], GN_NONNEG, "sleep-time") : 10);

 	printf("PID %ld: releasing %s at %s\n", (long)getpid(), lname, curr_time("%T"));
 	if (flock(fd, LOCK_UN) == -1)
 		err_exit("flock");

 	if (close(fd) == -1)
 		err_exit("close file");

 	exit(EXIT_SUCCESS);

 }


