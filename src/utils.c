#include "../include/utils.h"

extern void reverse_string (char* string, int length) {
	char temp;
	for (int i=0; i<(length/2); i++) {
		temp = string[i];
		string[i] = string[length-i-1];
		string[length-i-1] = temp;
	}
}

extern int create_udp_socket (	int* udp_socket,
								const char* ipv4_address,
								const int* port_number,
								struct sockaddr_in* server_address) {
	
	*udp_socket = socket (PF_INET, SOCK_DGRAM, 0);

	if (*udp_socket == -1) {
		LOGGER (get_date_time(), "Failed to create UDP socket\n");
		return FALSE;
	}

	server_address->sin_family = AF_INET;
	server_address->sin_port = htons (*port_number);
	server_address->sin_addr.s_addr = inet_addr (ipv4_address);
	memset (server_address->sin_zero, '\0', sizeof (server_address->sin_zero));  

	if (bind(*udp_socket, (struct sockaddr *) server_address, 
		sizeof (*server_address))) {
		LOGGER (get_date_time(), "Failed to bind %d\n", server_address->sin_port);
		return FALSE;
	}

	return TRUE;
}