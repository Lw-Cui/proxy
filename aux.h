#ifndef _AUX_H_
#define _AUX_H_

int open_listenfd(unsigned short port);
int open_clientfd(char *hostname, unsigned short port);

#endif