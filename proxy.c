#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include "rio.h"
#include "aux.h"
#define PORT 8080
#define MAX 100
#define HOST_BEG 11

int main(int args, char **argv) {
	int firstline = 1;
	int listenfd, serverfd, clientfd;
	unsigned int port = PORT, clientlen;
	char recv[MAX], host[MAX];
	struct sockaddr_in clientaddr;

	if (args > 1)
		sscanf(argv[1], "%d", &port);
	listenfd = open_listenfd(port);
	memset(host, 0, sizeof(host));
	while (1) {
		clientlen = sizeof(clientaddr);
		serverfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		while (rio_readline(serverfd, recv, MAX)) {
			if (firstline) {
				strncpy(host, recv + HOST_BEG, strchr(recv + HOST_BEG, '/') - recv - HOST_BEG);
				clientfd = open_clientfd(host, 80);
				firstline = 0;
			}
			rio_write(clientfd, recv, MAX);
			printf("%s", recv);
			if (!strcmp(recv, "\n\r"))
				break;
		}

		printf("\nRECV:\n");
		while (rio_readline(clientfd, recv, MAX)) {
			printf("%s", recv);
			fflush(stdout);
			rio_write(serverfd, recv, MAX);
			if (!strcmp(recv, "\n"))
				break;
		}

		close(serverfd);
	}
	return 0;
}