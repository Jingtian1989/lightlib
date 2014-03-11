#include <sys/types.h>
#include <sys/sem.h>

#include "semun.h"
#include "binary_sems.h"

boolean bs_use_sem_undo = false;
boolean bs_retry_on_eintr = true;



int init_sem_avaliable(int semid, int semnum) /* Initialize semaphore to 1("avaliable") */
{
	union semun arg;
	avg.val = 1;
	return semctl(semid, semnum, SETVAL, arg);
}


int init_sem_inuse(int semid, int semnum)
{
	union semun arg;
	arg.val = 0;
	return semctl(semid, semnum, SETVAL, arg);
}


/* Reserve semaphore (bloking), return 0 on success, or -1 with 'errno' 
 * set to EINTR if operation was interrupted by a signal handler
 */
int reserve_sem(int semid, int semnum) 	/* Reserve semaphore - decrement it by 1 */
{
	struct sembuf sops;
	sops.sem_num  = semnum;
	sops.sem_op   = -1;
	sops.sem_flag = bs_use_sem_undo ? SEM_UNDO : 0;
	while (semop(semid, &sops, 1) == -1)
		if (errno != EINTR || !bs_retry_on_eintr)
			return -1; 
	return 0;
}

int release_sem(int semid, int semnum)
{
	struct sembuf sops;
	sops.sem_num  = semnum;
	sops.sem_op   = 1;
	sops.sem_flag = bs_use_sem_undo ? SEM_UNDO : 0;

	return semop(semid, &sops, 1);
}
