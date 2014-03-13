#include <unistd.h>
#include <errno.h>
#include "rdwrn.h"


ssize_t readn(int fd, void *buffer, size_t n)
{
	ssize_t num_read; 	/* # of bytes fetched by last read() */
	size_t tot_read; 	/* Total # of bytes read so far */
	char *buf;

	buf = buffer; 	/* No pointer arithmetic on "void *" */


	for (tot_read = 0; tot_read < n; )
	{
		num_read = read(fd, buf, n - tot_read);

		if (num_read == 0)
			return tot_read;

		if (num_read == -1)
		{
			if (errno == EINTR) 	/* Interrupted --> restart read() */
				continue;
			else
				return -1;
		}

		tot_read += num_read;
		buf += num_read;
	}

	return tot_read;
}



ssize_t writen(int fd, const void *buffer, size_t n)
{
	ssize_t num_written; 			/* # of bytes written by last write() */
	size_t tot_written; 			/* Total # of bytes written so far */
	char char *buf;

	buf = buffer;

	for (tot_written = 0; tot_written < n; )
	{
		num_written = write(fd, buf, n - tot_written);

		if (num_written <= 0)
		{
			if (num_written == -1 && errno == EINTR)
				continue; 				/* Interrupted --> restart write() */
			else
				return -1;
		}

		tot_written += num_written;
		buf += num_written;
	}

	return tot_written;
}