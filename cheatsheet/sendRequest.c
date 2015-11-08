#include <sys/types.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>
#define MAX 10
#define LISTENQ 1024


ssize_t rio_readline(int fd, void *usrbuf, size_t maxlen) {
	int n, rc;
	char c, *buf = usrbuf;
	for (n = 1; n < maxlen; n++)
		if ((rc = read(fd, &c, 1)) == 1) {
			*buf++ = c;
			if (c == '\n')
				break;
		} else if (rc == 0) {
			if (n == 1)
				return 0;
			else
				break;
		} else {
			return -1;
		}

	*buf = 0;
	return n;
}

ssize_t rio_writeline(int fd, void *usrbuf, size_t n) {
	size_t nleft = n;
	ssize_t nwritten;
	char *bufp = usrbuf;

	while (nleft > 0) {
		if ((nwritten = write(fd, bufp, nleft)) <= 0)
			if (errno == EINTR)
				nwritten = 0;
			else
				return -1;

		nleft -= nwritten;
		bufp += nwritten;
	}

	return n;
}

int open_clientfd(char *hostname, unsigned short port) {
	int clientfd;
	struct in_addr *addr;
	struct hostent *hp;
	struct sockaddr_in serveraddr;
	char **p;
	int stat;

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;

	memset((char*)&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	memcpy((char *)&serveraddr.sin_addr.s_addr, (char *)hp->h_addr_list[0], hp->h_length);
	serveraddr.sin_port = htons(port);

	if ((stat = connect(clientfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr))) < 0) {
		return -3;
	}

	return clientfd;
}

int main(int args, char **agrv) {
	int port = 80, clientfd = -1, requestfd;
	int stat;
	char *host = "localhost";
	char recv[MAX], send[MAX];
	int filefd = open("test.html", O_WRONLY, 0);

	switch (args) {
	case 3:
		sscanf(agrv[2], "%d", &port);
	case 2:
		clientfd = open_clientfd(agrv[1], port);
		break;
	default:
		clientfd = open_clientfd(host, port);
		break;
	}

	printf("clientfd %d\n", clientfd);
	if (clientfd < 0)
		return 0;

	requestfd = open("simpleRequest", O_RDONLY, 0);
	//requestfd = open("exampleRequest", O_RDONLY, 0);

	while (rio_readline(requestfd, send, MAX)) {
		printf("%s", send);
		stat = rio_writeline(clientfd, send, strlen(send));
	}

	printf("\n===RECV===\n");

	while (rio_readline(clientfd, recv, MAX)) {
		printf("%s", recv);
		write(filefd, recv, strlen(recv));
	}

	close(clientfd);
	return 0;
}