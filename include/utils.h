#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 4096
#define LOGGER(info, ...) printf( "%s %s: " info, __DATE__, __TIME__, ##__VA_ARGS__)

extern int create_udp_socket (int* , const char*, const int*);
