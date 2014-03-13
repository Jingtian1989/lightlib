#include <sys/un.h>
#include <sys/socket.h>
#include <ctype.h>

#include "ud_ucase.h"
#include "light.h"

int main(int argc, char *argv[])
{
	struct sockaddr_un svaddr, claddr;
	int sfd, j;
	ssize_t num_bytes;
	ssize_t len;
	char buf[BUF_SIZE];

	sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (sfd == -1)
		err_exit("remove-%s", SV_SOCK_PATH);

	memset(&svaddr, 0, sizeof(struct sockaddr_un));
	svaddr.sun_family = AF_UNIX;
	strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

	if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
		err_exit("bind");

	/* Receive messages, convert to uppercase, and return to client */
	for (;;)
	{
		len = sizeof(struct sockaddr_un);
		num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

		if (num_bytes == -1)
			err_exit("recvfrom");

		for (j = 0; j < num_bytes; j++)
			buf[j] = toupper((unsigned char) buf[j]);

		if (sendto(sfd, buf, num_bytes, 0, (struct sockaddr *) &claddr, len) != num_bytes)
			fatal("sendto");
	}
}