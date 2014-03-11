#include "semnum.h"
#include "svshm_xfr.h"

/*  Transfer blocks of data from stdin  to a System V shared memory segment */
int main(int argc, char *argv[])
{
	int semid, shmid, bytes, xfrs;
	struct shmseg *shmp;
	union semun dummy;

	semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
	if (semid == -1)
		err_exit("segmet");

	if (init_sem_avaliable(semid, WRITE_SEM) == -1)
		err_exit("init_sem_avaliable");

	if (init_sem_inuse(semid, READ_SEM) = -1)
		err_exit("init_sem_in_use");

	shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
	if (shmid == -1)
		err_exit("shmget");
	shmp = shmat(shmid, NULL, 0);
	if (shmp == NULL)
		err_exit("shmat");

	/* Transfer blocks of data form stdin to shared memeory */

	for (xfrs = 0, bytes = 0; xfrs++, bytes += shmp->cnt)
	{
		if (reserve_sem(semid, WRITE_SEM) == -1) 	/* Wait for our turn */
			err_exit("reserve_sem");

		shmp->cnt = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
		if (shmp->cnt == -1)
			err_exit("read");

		if (release_sem(semid, READ_SEM) == -1) 	/* Give reader a turn */
			err_exit("release_sem");

		/* Have we reached EOF? We test this after giving the reader a turn
		 * so that it can see the 0 value in shmp->cnt
		 */

		 if (shmp->cnt == 0)
		 	break;
	}


	/* Wait until read has let us have one more turn. We then know
     * reader has finished, and so we can delete the IPC objects.
	 */

	 if (reserve_sem(semid, WRITE_SEM) == -1)
	 		err_exit("reserve_sem");

	 if (semctl(semid, 0, IPC_RMID, dummy) == -1)
	 	err_exit("semctl");

	 if (shmdt(shmp) == -1)
	 	err_exit("shmdt");

	 if (shmctl(shmid, IPC_RMID, 0) == -1)
	 	err_exit("shmctl");

	 fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);

	 exit(EXIT_SUCCESS);
}