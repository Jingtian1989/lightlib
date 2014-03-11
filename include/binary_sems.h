#ifndef _BINARY_SEMS_H_
#define _BINARY_SEMS_H_

#include "light.h"

extern boolean bs_use_sem_undo;		/* Use SEM_UNDO during semop()? */
extern boolean bs_retry_on_eintr; 	/* Retry if semop() interrupted by signal handler */

int init_sem_avaliable(int semid, int semnum);

int init_sem_inuse(int semid, int semnum);

int reserve_sem(int semid, int semnum);

int release_sem(int semid, int semnum);

#endif 