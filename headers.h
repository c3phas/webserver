/*header files and other definations for the server program:*/
#ifndef HEADERS_H
#define HEADERS_H
/*common header files*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

/*socket header files*/
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080  
#define BACKLOG 10

#endif
