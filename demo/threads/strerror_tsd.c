#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <pthread.h>

#include "light.h"
static pthread_once_t once = PTHREAD_ONCE_INIT;
static pthread_key_t str_error_key;

#define MAX_ERROR_LEN 256

static void destructor(void *buf)
{
	free(buf);
}


static void create_key(void)
{
	int s;
	/* Allocate a unique thread-specific data key and save the addresss 
	 * of the destructor for thread-specific data buffers
	 */

	 s = pthread_key_create(&str_error_key, destructor);
	 if (s != 0)
	 	err_exit_en(s, "pthread_key_create");
}
/* A thread-safe implemention of strerror() using thread-specific data */
char *strerror(int err)
{
	int s;
	char *buf;

	/* Make first caller allocate key for thread-specific data */

	s = pthread_once(&once, create_key);
	if (s != 0)
		err_exit_en(s, "pthread_once");

	buf = pthread_getspecific(str_error_key);
	if (buf == NULL)
	{
		buf = malloc(MAX_ERROR_LEN);
		if (buf == NULL)
			err_exit("malloc");
		s = pthread_setspecific(str_error_key, buf);
		if (s != 0)
			err_exit_en(s, "pthread_setspecific");
	}
	if (err < 0 || err >= _sys_err || _sys_errlist[err] == NULL)
		snprintf(buf, MAX_ERROR_LEN, "Unkown error %d", err);
	else
	{
		strcpy(buf, _sys_errlist[err], MAX_ERROR_LEN-1);
		buf[MAX_ERROR_LEN-1] = '\0';
	}
	return buf;

}