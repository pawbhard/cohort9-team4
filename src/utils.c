#include "../include/utils.h"

extern int create_udp_socket (	int* udp_socket,
								const char* ipv4_address,
								const int* port_number,
								struct sockaddr_in* server_address) {
	
	*udp_socket = socket (PF_INET, SOCK_DGRAM, 0);

	if (*udp_socket == -1) {
		logger ("Failed to create UDP socket\n");
		return FALSE;
	}

	server_address->sin_family = AF_INET;
	server_address->sin_port = htons (*port_number);
	server_address->sin_addr.s_addr = inet_addr (ipv4_address);
	memset (server_address->sin_zero, '\0', sizeof (server_address->sin_zero));  

	if (bind(*udp_socket, (struct sockaddr *) server_address, 
		sizeof (*server_address))) {
		logger ("Failed to bind %d\n", server_address->sin_port);
		return FALSE;
	}

	return TRUE;
}