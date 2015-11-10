#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include "rio.h"
#include "aux.h"
#define PORT 8080
#define SERVERPORT 8000
#define MAX 100
#define HOST_BEG 11

int main(int args, char **argv) {
	int firstline;
	int len;
	int listenfd, serverfd, clientfd;
	fd_set read_set;
	unsigned int port = PORT, clientlen;
	char recv[MAX], host[MAX];
	struct sockaddr_in clientaddr;

	if (args > 1)
		sscanf(argv[1], "%d", &port);
	listenfd = open_listenfd(port);
	memset(host, 0, sizeof(host));

	while (1) {
		firstline = 1;
		clientlen = sizeof(clientaddr);
		serverfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		while (1) {
			FD_ZERO(&read_set);
			if (!firstline)
				FD_SET(clientfd, &read_set);
			FD_SET(serverfd, &read_set);
			if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
				return 0;

			if (FD_ISSET(serverfd, &read_set)) {
				//rio_readline(serverfd, recv, MAX);
				if ((len = read(serverfd, recv, MAX)) <= 0)
					break;
				if (firstline) {
					strncpy(host, recv + HOST_BEG, strchr(recv + HOST_BEG, '/') - recv - HOST_BEG);
					clientfd = open_clientfd(host, 80);
					firstline = 0;
				}
				//rio_write(clientfd, recv, MAX);
				write(clientfd, recv, len);
				printf("%s", recv);
			}

			if (FD_ISSET(clientfd, &read_set)) {
				//rio_readline(clientfd, recv, MAX);
				if ((len = read(clientfd, recv, MAX)) <= 0)
					break;
				//rio_write(serverfd, recv, MAX);
				write(serverfd, recv, len);
			}
		}
	}

	close(serverfd);
	close(clientfd);
	return 0;
}