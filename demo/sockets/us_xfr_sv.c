#include <sys/un.h>
#include <sys/socket.h>
#include "light.h"

#define BACKLOG 5
int main(int argc, char *argv[])
{
	struct sockaddr_un addr;
	int sfd, cfd;
	ssize_t num_read;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sfd == -1)
		err_exit("socket");

	/* Construct server socket address, bind socket to it, 
 	 * and make this a listening socket.
	 */

	if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
		err_exit("remove-%s", SV_SOCK_PATH);

	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
		err_exit("bind");

	if (listen(sfd, BACKLOG) == -1)
		err_exit("listen");

	for (;;) 	/* Handle client connections iteratively */
	{
		/* Accept a connection. The connection is returned on a new socket, 'cfd'; 
 		 * the listening socket('sfd') remains open and can be used to accept fur-
 		 * ther connections.
		 */

		cfd = accept(sfd, NULL, NULL);
		if (cfd == -1)
			err_exit("accept");

		/* Transfer data from connected socket to stdout until EOF */
		while ((num_read = read(cfd, buf, BUF_SIZE)) > 0)
			if (write(STDOUT_FILENO, buf, num_read) != num_read)
				fatal("partial/failed write");
		if (num_read == -1)
			err_exit("read");

		if (close(cf) == -1)
			err_msg("close");
	}
}