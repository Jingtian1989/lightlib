#include <sys/stat.h>
#include <fcntl.h>
#include "region_lock.h"
#include "create_pid_file.h"

#include "light.h"

#define BUF_SIZE 100

/* Used to create a pid lock file to ensure just one instance of a program is started */
int create_pid_file(const char *prog_name, const char *pid_file, int flags)
{
	int fd;
	char buf[BUF_SIZE];
	fd = open(pid_file, O_RDWR | O_CREAT, S_IRUSR|S_IWUSR);
	if (fd == -1)
		err_exit("Could not open PID file %s", pid_file);

	if (flags & CPF_CLOEXEC)
	{
		/* Set the close-on-exec file descriptor flag */
		flags = fcntl(fd, F_GETFD); 	/* Fetch flags */
		if (flags == -1)
			err_exit("Could not get flags for PID file %s", pid_file);

		flags |= FD_CLOEXEC; 	/* Turn on FD_CLOEXEC */
		if (fcntl(fd, F_SETFD, flags) == -1) 	/* Update flags */
			err_exit("Could not set flags for PID file %s", pid_file);
	}

	if (lock_region(fd, F_WRLCK, SEEK_SET, 0, 0) == -1)
	{
		if (errno == EAGAIN || errno == EACCESS)
			fatal("PID file '%s' is locked; probably "
				"'%s' is already running", pid_file, prog_name);
		else
			err_exit("Unable to lock PID file '%s'", pid_file);
	}

	if (ftruncate(fd, 0) == -1)
		err_exit("Could not truncate PID file '%s'", pid_file);

	snprintf(buf, BUF_SIZE, "%ld\n", (long) getpid());
	if (write(fd, buf, strlen(buf)) != strlen(buf))
		fatal("Writing to PID file '%s'", pid_file);

	return fd;
}