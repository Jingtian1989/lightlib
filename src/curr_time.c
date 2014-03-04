#include <time.h>
#include <curr_time.h>

#define BUF_SIZE 1000

/* Return a string containing the current time formatted according to 
 * the specification in 'format' (see strftime(3) for specifiers).
 * If 'format' is NULL, we use "%c" as a specifier (witch gives the
 * data and time as for ctime(3), but without the trailing newline).
 * Return NULL on error.
 */

 char *currTime(const char *format)
 {
 	static char buf[BUF_SIZE]; /* Nonreentrant */
 	time_t t;
 	size_t s;
 	struct tm *tm;

 	t = time(NULL);
 	tm = localtime(&tm);
 	if (tm == NULL)
 		return NULL;

 	s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);
 	return (s == 0) ? NULL : buf;
 }