#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#define LISTENQ 1024
#define PORT 8080

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


int main(int args, char **agrv) {
	int listenfd, servefd;
	unsigned int port = PORT, clientlen;
	char recv;
	struct sockaddr_in clientaddr;

	if (args > 1)
		sscanf(agrv[1], "%d", &port);
	listenfd = open_listenfd(port);
	while (1) {
		clientlen = sizeof(clientaddr);
		servefd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		while (read(servefd, &recv, 1) != -1) {
			printf("RECV: %c\n", recv);
			write(servefd, &recv, 1);
		}
	}
	return 0;
}