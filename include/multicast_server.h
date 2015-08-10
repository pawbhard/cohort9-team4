#include "../include/multicast_util.h"
#define NUM_OF_GROUP 5
#define SERVER_PORT 4567

char *group_address[NUM_OF_GROUP] = 
{   "226.1.1.1",
    "226.1.1.2",
    "226.1.1.3",
    "226.1.1.4",
    "226.1.1.5",
    //ADD other if required
    //TODO : should get from config file 
};

//start group_server
// Void *arg so that can be used in pthread 
//   if multiple servers is required
//   takes server_info_t as argument
void run_group_server(multicast_group*);

//Used to receive messages from clients and store it
void receive_and_store_messages_server(int socketfd, multicast_group* head);

//Used to display messages at regular interval
// arg is delay time unsigned integer 
void display_thread( void * arg);

//TODO : Used to initialize groups and name from config file
void init(void);

static multicast_group* find_multicast_group (multicast_group*, ipv4_address);
//static int remove_client_list (client_node*);
//static int remove_multicast_group (multicast_group*, ipv4_address);
static int add_multicast_group (multicast_group*, ipv4_address);
static int add_client_to_multicast_group (multicast_group*, ipv4_address, ipv4_address, int, char*);
//static int remove_client_from_multicast_group (multicast_group*, ipv4_address, int);