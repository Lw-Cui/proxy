#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#include "aux.h"
#define LISTENQ 1024

int open_listenfd(unsigned short port) {
	int listenfd, optval = 1;
	struct sockaddr_in serveraddr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
		(const void *)&optval, sizeof(int)) < 0)
		return -1;

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	serveraddr.sin_port = htons(port);
	if (bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return -1;

	if (listen(listenfd, LISTENQ) < 0)
		return -1;

	return listenfd;
}

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
	memcpy((char *)&serveraddr.sin_addr.s_addr, (char *)hp->h_addr_list[0], hp->h_length);
	serveraddr.sin_port = htons(port);

	if (connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
		return -3;

	return clientfd;
}