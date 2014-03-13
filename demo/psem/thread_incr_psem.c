#include <semaphore.h>
#include <pthread.h>
#include "light.h"

static int glob = 0;
static sem_t sem;

static void *thread_func(void *arg)
{
	int loops = (int)arg;

	int loc, j;

	for (j = 0; j < loops; j++)
	{
		if (sem_wait(&sem) == -1)
			err_exit("sem_wait");

		loc = glob;
		loc++;
		glob = loc;

		if (sem_post(&sem) == -1)
			err_exit("sem_post");
	}

	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_t t1, t2;
	int loops, s;

	loops = (argc > 1) ? get_int(argv[1], GN_GT_0, "num-loops") : 1000000;


	/* Initialize a thread-shared semaphore with the value 1 */

	if (sem_init(&sem, 0, 1) == -1)
		err_exit("sem_init");

	/* Create tow threads that increment 'glob' */
	s = pthread_create(&t1, NULL, thread_func, &loops);
	if (s != 0)
		err_exit_en(s, "pthread_create");
	s = pthread_create(&t2, NULL, thread_func, &loops);
	if (s != 0)
		err_exit_en(s, "pthread_create");


	/* Wait for threads to terminate */
	s = pthread_join(t1, NULL);
	if (s != 0)
		err_exit_en(s, "pthread_join");
	s = pthread_join(t2, NULL);
	if (s != 0)
		err_exit_en(s, "pthread_join");

	if (sem_destroy(&sem) == -1)
		err_exit("sem_destroy");

	printf("glob - %d\n", glob);
	exit(EXIT_SUCEESS);
}