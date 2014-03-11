#include <sys/types.h>
#include <sys/sem.h>
#include <time.h>
#include "semnum.h"
#include "light.h"

int main(int argc, char *argv[])
{
	struct semid_ds ds;
	union arg, dummy; 	/* Forth argument for semctl() */
	int semid, j;

	if (argc != 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s semid\n", argv[0]);
	semid = get_int(argv[1], 0, "semid");

	arg.buf = &ds;
	if (semctl(semid, 0, IPC_STAT, arg) == -1)
		err_exit("semctl");

	printf("Semaphore changed: %s", ctime(&ds.sem_ctime));
	printf("Last semop(): 	   %s", ctime(&ds.sem_otime));

	/* Display per-semaphore information */
	arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
	if (arg.array == NULL)
		err_exit("calloc");
	if (semctl(semid, 0, GETALL, arg) == -1)
		err_exit("semctl-GETALL");

	printf("Sem # Value SEMPID SEMNCNT SEMZCNT\n");

	for (j = 0; j < ds.sem_nsems; j++)
	{
		printf("%3d %5d %5d %5d\n", j, arg.array[j], 
			semctl(semid, j, GETTPID, dummy),
			semctl(semid, j, GETNCNT, dummy),
			semctl(semid, j, GETZCNT, dummy)
			);
	}

	exit(EXIT_SUCCESS);
}