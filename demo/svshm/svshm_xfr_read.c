#include "semnum.h"
#include "svshm_xfr.h"

/*  Transfer blocks of data from stdin  to a System V shared memory segment */
int main(int argc, char *argv)
{
	int semid, shmid, xfrs, bytes;
	struct shmseg *shmp;

	/* Get IDs for semaphore set and shared memory created by writer */
	semid = semget(SEM_KEY, 0, 0);
	if (semid == -1)
		err_exit("semget");

	shmid = shmget(SHM_KEY, 0, 0);
	if (shmid == -1)
		err_exit("shmget");

	/* Transfer blocks of data from shared memory to stdout */
	for (xfrs = 0, bytes = 0; ; xfrs++)
	{
		if (reserve_sem(semid, READ_SEM) == -1)
			err_exit("reserve_sem");

		if (shmp->cnt == 0) 		/* Writer encountered EOF */
			break;
		bytes += shmp->cnt;

		if (write(STDOUT_FILENO, shmp->buf, shmp->cnt) != shmp->cnt)
			fatal("partial/failed write");

		if (release_sem(semid, WRITE_SEM) == -1) 	/* Give writer a turn */
			err_exit("release_sem");

	}

	if (shmdt(shmp) == -1)
		err_exit("shmdt");

	/* Give writer one more turn, so it can clean up */
	if (release_sem(semid, WRITE_SEM) == -1)
		err_exit("release_sem");

	fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
	exit(EXIT_SUCCESS);
}