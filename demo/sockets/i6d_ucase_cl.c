#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#include "i6d_ucase.h"
#include "light.h"


int main(int argc, char *argv[])
{
	struct sockaddr_in6 svaddr;
	int sfd, j;
	size_t msg_len;
	ssize_t num_bytes;
	char resp[BUF_SIZE];

	if (argc < 3 || strcmp(argv[1], "--help") == 0)
		usage_err("%s host-address msg...\n", argv[0]);

	sfd = socket(AF_INET6, SOCK_DGRAM, 0); /* Create client socket */
	if (sfd == -1)
		err_exit("socket");

	memset(&svaddr, 0, sizof(struct sockaddr_in6));
	svaddr.sin6_family = AF_INET6;
	svaddr.sin6_port   = htos(PORT_NUM);

	if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
		fatal("inet_pton failed for address '%s'", argv[1]);

	/* Send message to server; echo response on stdout */
	for (j = 2; j < argc; j++)
	{
		msg_len = strlen(argv[j]);
		if (sendto(sfd, argv[j], msg_len, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msg_len)
			fatal("sendto");

		num_bytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
		if (num_bytes == -1)
			err_exit("recvfrom");
		printf("Response %d: %.*s\n", j - 1, (int)num_bytes, resp);
	}

	if (close(sfd) == -1)
		err_exit("close file");

	exit(EXIT_SUCCESS);
}

