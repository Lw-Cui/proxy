#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define MAX 100

ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
	size_t nleft = n;
	ssize_t nread;
	char *bufp = usrbuf;

	while (nleft > 0) {
		if ((nread = read(fd, bufp, nleft)) < 0)
			if (errno == EINTR)
				nread = 0;
			else 
				return -1;
		else if (nread == 0)
			break;
		nleft -= nread;
		bufp += nread;
	}

	return (n - nleft);
}

ssize_t rio_readline(int fd, void *usrbuf, size_t maxlen) {
	int n, rc;
	char c, *buf = usrbuf;
	for (n = 1; n < maxlen; n++)
		if ((rc = rio_readn(fd, &c, 1)) == 1) {
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

int main(int argc, char **argv) {
	int fd;
	char buf[MAX];

	if (argc > 1) {
		fd = open("foo.txt", O_RDONLY, 0);
		while (rio_readline(fd, buf, MAX))
			printf("%s", buf);
	}
    return 0;
}
