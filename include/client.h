#include "utils.h"
#define AVAILABLE 1
#define REGISTERED 0
using namespace std;

typedef struct connection_handler_t {
	int local_fd;
	struct sockaddr_in server_addr;
	int groups[1000] = {0};
} connection_handler;

syserr_t create_client_socket (	connection_handler*, 
								char*, int* );
syserr_t establish_server_connection (	connection_handler*, 
								char*, int* );
extern void *client_group_join_or_leave (void *);
void display_group_list_from_server(connection_handler*, bool);
void send_join_or_leave_messsage(connection_handler*, char*, char);