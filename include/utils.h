#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 4096

static inline void logger (const char* info, ...) {
	va_list args;
	va_start(args, info);
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	printf("%s:",asctime(tm));
	printf (info, args);
}

extern int create_udp_socket (	int* , const char*, const int*, 
								struct sockaddr_in*);
