#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "rio.h"

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