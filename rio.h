#ifndef _RIO_H_
#define _RIO_H_ 
#include <stdlib.h>

ssize_t rio_readline(int fd, void *usrbuf, size_t maxlen);
ssize_t rio_write(int fd, void *usrbuf, size_t n);
#endif