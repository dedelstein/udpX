#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Desc:	Template function for capturing user input
 *
 * Param:	prompt - const char *, contextual prompt
 *			output - char *, buffer for input
 *			outputLen - const int, length of input buffer
 */
void StrPrompt(const char *prompt, char *output, const int outputLen);

int main(int argc, char **argv)
{
	int sockfd, pton_check;
	char buf[sizeof(struct in_addr)];
	struct sockaddr_in ip4_address;
	socklen_t addr_size;
	struct sockaddr *s;
	char msg[BUFSIZ];

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

	StrPrompt("Please enter a message to send to server:", msg, BUFSIZ);

	sendto(sockfd, msg, strlen(msg), 0, s, addr_size);

	recvfrom(sockfd, msg, BUFSIZ, 0, s, &addr_size);
	printf("Recieved: %s\n", msg);

	return 0;
}

// Attempts to sscanf into given char* output, loops until success
void StrPrompt(const char *prompt, char *output, const int outputLen)
{
	int success = 0;
	int test;
	int bufLen;

	do
	{
		char buf[BUFSIZ];

		printf("%s\n", prompt);
		fgets(buf, BUFSIZ, stdin);
		bufLen = strlen(buf);

		if (bufLen > outputLen)
		{
			puts("Input too long!");
			continue;
		}
		else if (bufLen <= 1)
		{
			puts("Empty Input!");
			continue;
		}
		else if (buf[bufLen - 1] != '\n') // Checks if full line read
		{
			printf("Invalid Input. Line exceeds maximum length (%d chars)\n",
				   outputLen - 1);
			continue;
		}

		test = sscanf(buf, "%[^\t\n]", output);

		if (test != 1)
		{
			puts("Invalid Entry");
		}
		else
			success = 1;

	} while (!success);
}
