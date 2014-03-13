#include <fcntl.h>
#include "region_locking.h"


static int lock_reg(int fd, int cmd, int type, int whence, int start, off_t len)
{
	struct flock fl;

	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;

	return fcntl(fd, cmd, &fl);
}

int lock_region(int fd, int type, int whence, int start, int len)
{
	return lock_reg(fd, F_SETLK, type, whence, start, len);
}


int lock_region_wait(int fd, int type, int whence, int start, int len)
{
	return lock_reg(fd, F_SETLK, type, whence, start, len);
}


pid_t region_is_locked(int fd, int type, int whence, int start, int len)
{
	struct flock fl;
	fl.l_type = type;
	fl.l_whence = whence;
	fl.l_start = start;
	fl.l_len = len;

	if (fcntl(fd, F_GETLK, &fl) == -1)
		return -1;

	return (fl.l_type == F_UNLCK) ? 0 : fl.l_pid;
}
