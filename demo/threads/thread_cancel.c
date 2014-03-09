#include <pthread.h>
#include "light.h"

/*
 * 	usage:
 *		$ ./t_pthread_cancel
 *		Loop 1
 *		Loop 2
 *		Loop 3
 *		Thread was canceled
 */
static void *thread_func(void *arg)
{
	int j;
	printf("New thread started\n");
	for(j = 1; ; j++)
	{
		printf("Loop %d\n", j);
		sleep(1); /* A cancellation point */
	}

	/* NOTREACHED */
	return NULL;
}

int main(int argc, char *argv)
{
	pthread_t thr;
	int s;
	void *res;
	s = pthread_create(&thr, NULL, thread_func, NULL);
	if (s != 0)
		err_exit_en(s, "pthread_create");

	sleep(3);

	s = pthread_cancel(thr);
	if (s != 0)
		err_exit_en(s, "pthread_cancel");
	s = pthread_join(thr, &res);
	if (s != 0)
		err_exit_en(s, "pthread_join");

	if (res == PTHREAD_CANCELED)
		printf("Thread was canceled\n");
	else
		printf("Thread was not canceled (should not happend!)\n");

	exit(EXIT_SUCCESS);
}