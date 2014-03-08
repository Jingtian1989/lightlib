#include <pthread.h>
#include "light.h"

/* 
 *	demonstrate how main thread waits created threads terminate and cleans up shared resouces.
 *
 *	usage:
 *		$ ./thread_multijoin 1 1 2 3 3 						Create 5 threads
 *			Thread 0 terminating
 *			Thread 1 terminating
 *			Reaped thread 0 (numLive=4)
 *			Reaped thread 1 (numLive=3)
 *			Thread 2 terminating
 *			Reaped thread 2 (numLive=2)
 *			Thread 3 terminating
 *			Thread 4 terminating
 *			Reaped thread 3 (numLive=1)
 *			Reaped thread 4 (numLive=0)		
 */

static pthread_cond_t thread_died = PTHREAD_COND_INITIALIZER; 	
static pthread_mutex_t thread_mutex = PTHREAD_MUTEX_INITIALIZER; 	/* Used to protect thread array */

static int tot_threads = 0;
static int num_live = 0;
static int num_unjoined = 0;

enum tstate 
{
	TS_ALIVE,
	TS_TERMINATED,
	TS_JOINED
};

static struct 
{
	pthread_t tid;
	enum tstate state;
	int sleep_time;
} *thread;

static void *thread_func(void *arg)
{
	int idx = *((int *)arg);
	int s;
	sleep(thread[idx].sleep_time);
	printf("Thread %d terminating\n", idx);

	s = pthread_mutex_lock(&thread_mutex);
	if (s != 0)
		err_exit_en(s, "pthread_mutex_lock");
	num_unjoined++;
	thread[idx].state = TS_TERMINATED;

	s = pthread_mutex_unlock(&thread_mutex);
	if (s != 0)
		err_exit_en(s, "pthread_mutex_unlock");
	s = pthread_cond_signal(&thread_died);
	if (s != 0)
		err_exit_en(s, "pthread_cond_signal");

	return NULL;
}

int main(int argc, char *argv[])
{
	int s, idx;
	if (argc < 2 || strcmp(argv[1], "--help") == 0)
		usage_err("%s nsecs...\n", argv[0]);

	thread = calloc(argc-1, sizeof(*thread));
	if (thread == NULL)
		err_exit("calloc");

	for(idx = 0; idx < argc - 1; idx++)
	{
		thread[idx].sleep_time = get_int(argv[idx + 1], GN_NONNEG, NULL);
		thread[idx].state = TS_ALIVE;
		s = pthread_create(&thread[idx].tid, NULL, thread_func, &idx);
		if (s != 0)
			err_exit_en(s, "pthread_create");
	}
	tot_threads = argc - 1;
	num_live = tot_threads;

	/* Join with terminated threads */
	while(num_live > 0)
	{
		s = pthread_mutex_lock(&thread_mutex);
		if (s != 0)
			err_exit_en(s, "pthread_mutex_lock");
		while (num_unjoined == 0)
		{
			s = pthread_cond_wait(&thread_died, &thread_mutex);
			if (s != 0)
				err_exit_en(s, "pthread_cond_wait");
		}

		for (idx = 0; idx < tot_threads; idx++)
		{
			if (thread[idx].state == TS_TERMINATED)
			{
				s = pthread_join(thread[idx].tid, NULL);
				if (s != 0)
					err_exit_en(s, "pthread_join");
				thread[idx].state = TS_JOINED;
				num_live--;
				num_unjoined--;
				printf("Reaped thread %d (numLived = %d)\n", idx, num_live);
			}
		}
		s = pthread_mutex_unlock(&thread_mutex);
		if (s != 0)
			err_exit_en(s, "pthread_mutex_unlock");
	}
	exit(EXIT_SUCCESS);
}