#include "database.h"

static int run_multicast_udp_server (int*, int*, struct sockaddr_in *, 
	const char*, const int*, const int*);
extern int read_server_config ();