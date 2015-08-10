#include "utils.h"

static int run_multicast_udp_server (int*, int*, struct sockaddr_in *, 
	const char*, const int*, const int*);

static multicast_group* find_multicast_group (multicast_group*, ipv4_address);
static int remove_client_list (client_node*);
static int remove_multicast_group (multicast_group*, ipv4_address);
static int add_multicast_group (multicast_group*, ipv4_address);
static int add_client_to_multicast_group (multicast_group*, ipv4_address, ipv4_address, int, char*);
static int remove_client_from_multicast_group (multicast_group*, ipv4_address, int);
extern int read_server_config ();