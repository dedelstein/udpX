#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int sockfd, pton_check, bind_check;
	socklen_t addr_size;
	struct sockaddr *s;
	struct sockaddr in_addr;
	struct sockaddr_in serv_addr;
	char buf[BUFSIZ];

	if (argc != 3)
	{
		fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *ip = argv[1];
	int port = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	pton_check = inet_pton(AF_INET, ip, &(buf));
	if (!pton_check)
	{
		fprintf(stderr, "Could not convert %s to valid IPv4 binary.\n", ip);
		exit(EXIT_FAILURE);
	}
	else if (pton_check == -1)
	{
		fprintf(stderr, "Invalid Address Family\n");
		exit(EXIT_FAILURE);
	}

	serv_addr.sin_addr = *((struct in_addr *) &buf);

	s = (struct sockaddr *) &serv_addr;
	addr_size = sizeof(serv_addr);

	bind_check = bind(sockfd, s, addr_size);
	if (bind_check < 0)
	{
		fprintf(stderr, "Bind error");
		exit(EXIT_FAILURE);
	}

	socklen_t in_addr_size = sizeof(in_addr);

	recvfrom(sockfd, buf, BUFSIZ, 0, &in_addr, &in_addr_size);

	sendto(sockfd, buf, strlen(buf), 0, &in_addr, in_addr_size);

	return 0;
}
