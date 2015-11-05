#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

int open_clientfd(char *hostname, unsigned short port) {
	int clientfd;
	struct in_addr *addr;
	struct hostent *hp;
	struct sockaddr_in serveraddr;
	char **p;

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;

	memset((char*)&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	memcpy((char *)hp->h_addr_list[0], 
		(char *)&serveraddr.sin_addr.s_addr, hp->h_length);
	serveraddr.sin_port = htons(port);

	if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	return clientfd;
}

int main(int args, char **agrv) {
	int port = 80, clientfd = -1;
	char send, recv;

	switch (args) {
	case 3:
		sscanf(agrv[2], "%d", &port);
	case 2:
		clientfd = open_clientfd(agrv[1], port);
		break;
	default:
		break;
	}

	if (clientfd == -1)
		return -1;

	while ((send = fgetc(stdin)) != EOF) {
		write(clientfd, &send, 1);
		read(clientfd, &recv, 1);
		fputc(recv, stdout);
	}

	close(clientfd);
	return 0;
}