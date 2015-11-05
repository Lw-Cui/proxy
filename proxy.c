#include <stdio.h>
#include <netinet/in.h>
#include "rio.h"
#include "aux.h"
#define PORT 8080
#define MAX 100

int main(int args, char **argv) {

	int listenfd, serverfd;
	unsigned int port = PORT, clientlen;
	char recv[MAX];
	struct sockaddr_in clientaddr;

	if (args > 1)
		sscanf(argv[1], "%d", &port);
	listenfd = open_listenfd(port);
	while (1) {
		clientlen = sizeof(clientaddr);
		serverfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);

		while (rio_readline(serverfd, &recv, MAX) != -1) {
			printf("%s", recv);
		}
	}
	return 0;
}