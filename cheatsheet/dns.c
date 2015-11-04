#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdio.h>

int main(int args, char **argv) {
	struct hostent *hostp;
	struct in_addr addr;
	char **p;

	struct sockaddr_in sa;
	char host[1024];

	if (args != 2)
		return 0;

	if (inet_aton(argv[1], &sa.sin_addr)) {
		sa.sin_family = AF_INET;
		sa.sin_port = 80;
		getnameinfo((struct sockaddr *)&sa, sizeof(sa), host, sizeof(host), NULL, 0, 0);
		printf("host: %s\n", host);
		hostp = gethostbyname(host);
	} else {
		hostp = gethostbyname(argv[1]);
	}

	printf("domain_name:\n  %s\n", hostp->h_name);

	printf("Aliases:\n");
	for (p = hostp->h_aliases; *p != NULL; p++)
		printf("  %s\n", *p);
	if (hostp->h_aliases[0] == NULL)
		printf("  (NONE)\n");

	printf("Address:\n");
	for (p = hostp->h_addr_list; *p != NULL; p++) {
		addr = *(struct in_addr *)*p;
		printf("  %x -ntohl-> %x -ntoa-> %s\n", addr.s_addr,
			ntohl(addr.s_addr), inet_ntoa(addr));
	}

	printf("Length:\n  %d\n", hostp->h_length);
	return 0;
}