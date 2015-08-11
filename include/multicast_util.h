#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 1024

typedef struct ipv4_address {
	in_addr_t address;
}ipv4_address;

typedef struct message_list {
	char data[BUFFER_SIZE];
	struct message_list* next;
}message_list;

typedef struct client_node {
	ipv4_address client_addr;
	int id;
	struct message_list* msg;
	struct client_node* next;
}client_node;

typedef struct multicast_group {
	ipv4_address group_addr;
	client_node* client_list; 
	struct multicast_group* next;	
}multicast_group;

extern int
send_to_from(int fd, void *buf, size_t len, int flags,
        const struct sockaddr *to,
        const struct sockaddr *from,
        socklen_t tolen);

extern int
recv_from_to(int fd, void *buf, size_t len, int flags,
        struct sockaddr *from, 
        struct sockaddr *to,
        socklen_t sa_size);

extern int compare_ipv4_address (ipv4_address, ipv4_address);
extern void delete_message_list (message_list*);
extern int add_message_to_list (message_list*, char*);