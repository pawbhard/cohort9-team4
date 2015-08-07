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
								const char* socket_ipv4_address,
								const int* port_number,
								struct sockaddr_in* socket_address,
								int bind_socket) {
	
	*udp_socket = socket (AF_INET, SOCK_DGRAM, 0);
	char check_ipv4_address[INET_ADDRSTRLEN];

	if (*udp_socket == -1) {
		LOGGER (get_date_time(), "Failed to create UDP socket\n");
		return FALSE;
	}

	if (!bind_socket) {
		LOGGER (get_date_time(),"Created UDP socket without bind\n");
		return TRUE;
	}

	bzero((char *)socket_address, sizeof(*socket_address));
	socket_address->sin_family = AF_INET;
	socket_address->sin_port = htons (*port_number);
	socket_address->sin_addr.s_addr = socket_ipv4_address?
							inet_addr (socket_ipv4_address):htonl (INADDR_ANY);

	if (bind(*udp_socket, (struct sockaddr *) socket_address, 
		sizeof (*socket_address))) {
		LOGGER (get_date_time(), "Failed to bind %d\n", 
				socket_address->sin_port);
		return FALSE;
	}
	inet_ntop( 	AF_INET, &(socket_address->sin_addr.s_addr), 
				check_ipv4_address, INET_ADDRSTRLEN );
	LOGGER (get_date_time(),"Created UDP socket binded to %s:%u\n", 
			check_ipv4_address, ntohs(socket_address->sin_port));
	return TRUE;
}