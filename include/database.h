#include "utils.h"

multicast_group* find_multicast_group (multicast_group*, ipv4_address);
int remove_client_list (client_node*);
int remove_multicast_group (multicast_group*, ipv4_address);
int add_multicast_group (multicast_group*, ipv4_address);
int add_client_to_multicast_group (multicast_group*, ipv4_address, ipv4_address, int, char*);
int remove_client_from_multicast_group (multicast_group*, ipv4_address, int);