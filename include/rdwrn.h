#ifndef _RDWRN_H_
#define _RDWRN_H_

/* Return number of bytes read, 0 on EOF, or -1 on error */
ssize_t readn(int fd, void *buffer, size_t count);

/* Return number of bytes written, or -1 on error */
ssize_t writen(int fd, void *buffer, size_t count);

#endif