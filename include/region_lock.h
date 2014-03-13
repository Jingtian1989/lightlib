#ifndef _REGION_LOCK_H_
#define _REGION_LOCK_H_

/* Lock a file region using nonblocking F_SETLK */
int lock_region(int fd, int type, int whence, int start, int len);	

/* Lock a file region using blocking F_SETLKW */
int lock_region_wait(int fd, int type, int whence, int start, int len);


/* Test if a file region is lockable. Return 0 if lockable, or 
 * PID of process holding incompatible lock, or -1 on error.
 */
pid_t region_is_locked(int fd, int type, int whence, int start, int len)

#endif 

