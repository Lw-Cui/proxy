#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#define MAX 50

void read() {
	int fd;
	char c;

	fd = open("foo.txt", O_RDONLY, 0);
	printf("fd = %d\n", fd);
	while (read(fd, &c, 1) != 0)
		printf("%c", c);
	printf("\nEOF\n");
	close(fd);
}

void stat() {
	struct stat foo;
	char *type = "unknown";

	stat("foo", &foo);
	if (S_ISDIR(foo.st_mode))
		type = "directory";
	printf("%s\n", type);
}

int main(int argc, char **argv) {
    return 0;
}
