#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

int host_entry(char *hostname) {
	int clientfd;
	struct hostent *hp;
	struct in_addr *addr;
	char **p;

	if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return -1;

	if ((hp = gethostbyname(hostname)) == NULL)
		return -2;

	printf("domain_name:\n  %s\n", hp->h_name);

	printf("Aliases:\n");
	for (p = hp->h_aliases; *p != NULL; p++)
		printf("  %s\n", *p);
	if (hp->h_aliases[0] == NULL)
		printf("  (NONE)\n");

	printf("Address:\n");
	for (p = hp->h_addr_list; *p != NULL; p++) {
		addr = (struct in_addr *)*p;
		printf("  %x -ntohl-> %x -ntoa-> %s\n", addr->s_addr,
			 ntohl(addr->s_addr), inet_ntoa(*addr));
	}

	printf("Length:\n  %d\n", hp->h_length);
	return 0;
}

int main(int args, char **agrv) {
	if (args > 1)
		host_entry(agrv[1]);
	return 0;
}