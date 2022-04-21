#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int sockfd, pton_check, msg_size;
	char buf[sizeof(struct in_addr)];
	struct sockaddr_in ip4_address;
	socklen_t addr_size;
	struct sockaddr *s;
	char *msg;

	if (argc < 3)
	{
		fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char *ip = argv[1];
	int port = atoi(argv[2]);

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	ip4_address.sin_family = AF_INET;
	ip4_address.sin_port = htons(port);

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

	ip4_address.sin_addr = *((struct in_addr *) &buf);
	
	s = (struct sockaddr *) &ip4_address;
	addr_size = sizeof(ip4_address);

	msg = "Hello!";
	msg_size = sizeof(msg);

	sendto(sockfd, msg, msg_size, 0, s, addr_size);

	return 0;
}
