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
#define LOGGER(time, info, ...) printf("%s " info, time, ##__VA_ARGS__)

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
}client_node;

typedef struct multicast_group {
	ipv4_address group_addr;
	client_node* client_list;
}multicast_group;

static inline char* get_date_time () {
	char* time_data;
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	time_data = asctime(tm);
	time_data[strcspn(time_data, "\n")] = 0;
	return time_data;
}

extern int create_udp_socket (	int*, const char*, const int*, 
								struct sockaddr_in*, int);

extern void reverse_string (char*, int);
extern int compare_ipv4_address (ipv4_address, ipv4_address);
extern void delete_message_list (message_list*);
extern int add_message_to_list (message_list*, char*);