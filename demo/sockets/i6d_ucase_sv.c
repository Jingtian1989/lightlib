#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#include "i6d_ucase.h"
#include "light.h"


/*
 * 	usage:
 *		$ ./i6d_ucase_sv &
 *		[1] 31047
 *		$ ./i6d_ucase_cl ::1 ciao                     Send to server on local host
 *		Server received 4 bytes from (::1, 32770)
 *		Response 1: CIAO
 */
int main(int argc, char *argv[])
{
	struct sockaddr_in6 svaddr, claddr;
	int sfd, j;
	ssize_t num_bytes;
	socklen_t len;
	char buf[BUF_SIZE];
	char claddr_str[INET6_ADDRSTRLEN];

	sfd = socket(AF_INET6, SOCK_DGRAM, 0);
	if (sfd == -1)
		err_exit("socket");

	memset(&svaddr, 0, sizeof(struct sockaddr_in6));
	svaddr.sin6_family = AF_INET6;
	svaddr.sin6_addr   = in6addr_any;
	svaddr.sin6_addr   = htons(PORT_NUM);

	if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) == -1)
		err_exit("bind");

	/* Receive messages, convert to uppercase, and return to client */
	for(;;)
	{
		len = sizeof(struct sockaddr_in6);
		num_bytes = recvfrom(sfd, buf, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);
		if (num_bytes == -1)
			err_exit("recvfrom");

		if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddr_str, INET6_ADDRSTRLEN) == NULL)
			printf("Couldn't convert client address to string\n");
		else
			printf("Server received %ld bytes from (%s, %u)\n", (long)num_bytes, claddr_str, ntohs(claddr.sin6_port));

		for (j = 0; j < num_bytes; j++)
			buf[j] = toupper((unsigned char) buf[j]);

		if (sendto(sfd, buf, num_bytes, 0, (struct sockaddr *) &claddr, len) != num_bytes)
			fatal("sendto");
	}

}

