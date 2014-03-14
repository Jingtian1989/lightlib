static int pfd[2]; 	/* File descriptors for pipe */

static void handler(int sig)
{
	int saved_errno; 		/* In case we change 'errno' */

	saved_errno = errno;
	if (write(pfd[1], "x", 1) == -1 && errno != EAGAIN)
		err_exit("write");
	errno = saved_errno;
}


int main(int argc, char *argv[])
{
	fd_set read_fds;
	int ready, nfds, flags;

	struct timeval timeout;
	struct timeval *pro;
	struct sigaction sa;
	char ch;

	/* ... Initialize 'timeout', 'read_fds', and 'nfds' for select() */

	if (pipe(pfd) == -1)
		err_exit("pipe");

	FD_SET(pfd[0], &read_fds); 	/* Add read end of pipe to 'readfds '*/
	nfds = max(nfds, pfd[0] + 1);
	flags = fcntl(pfd[0], F_GETFL);
	if (flags == -1)
		err_exit("fcntl-F_GETFL");

	flags |= O_NONBLOCK; 	/* Make sure end nonblocking */

	if (fcntl(pfd[0], F_SETFL, flags) == -1)
		err_exit("fcntl-F_SETFL");

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		err_exit("sigaction");

	while((ready = select(nfds, &read_fds, NULL, NULL, pto)) == -1 && errno == EINTR)
		continue;	/* Restart if interrupted by signal */

	if (ready == -1) 	
		err_exit("select");

	if (FD_ISSET(pfd[0], &read_fds))
	{
		printf("A signal was caught\n");

		for (;;)
		{
			if (read(pfd[0], &ch, 1) == -1)
			{
				if (errno == EAGAIN)
					break;
				else
					err_exit("read");
			}

			/* Perform any actions that should be taken in response to signal */
		}
	}

	/* Examine file descriptors sets returned by select() to see which other file descriptors are ready */

}


