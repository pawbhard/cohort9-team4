#include "../include/server.h"

static int run_udp_server (	int* udp_socket, 
							const char* multicast_ipv4_address,
							const int* server_port_number) {
	
	struct sockaddr_in server_storage;
	struct ip_mreq multicast_ipv4_request;
	char data_buffer[BUFFER_SIZE];
	int data_size;
	int sent;
	socklen_t server_address_size = sizeof(server_storage);

	memset(&server_storage, 0, sizeof(struct sockaddr_in));	
	memset(&multicast_ipv4_request, 0, sizeof(struct ip_mreq));
	
	server_storage.sin_family = AF_INET;
	server_storage.sin_port = htons (*server_port_number);
	server_storage.sin_addr.s_addr = inet_addr (multicast_ipv4_address);
	memset (server_storage.sin_zero, '\0', sizeof (server_storage.sin_zero));

	multicast_ipv4_request.imr_multiaddr.s_addr = 
						inet_addr(multicast_ipv4_address);
	multicast_ipv4_request.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt (*udp_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
              (const void *)&multicast_ipv4_request, sizeof(struct ip_mreq))) {
		LOGGER (get_date_time(), "Failed to join multicast group\n");
		return FALSE;
	}
	
	LOGGER (get_date_time(), "Multicast UDP server has started\n");

	while (TRUE) {
		data_size = recvfrom (*udp_socket, data_buffer, BUFFER_SIZE, 0, 
					(struct sockaddr *)&server_storage, &server_address_size);

		if (data_size == -1) {
			LOGGER (get_date_time(), "Error receiving data from client %u\n", 
					server_storage.sin_port);
			continue;
		}

		LOGGER (get_date_time(), "Received \"%s\" from client %u\n", data_buffer, 
				server_storage.sin_port);

		reverse_string (data_buffer, strlen(data_buffer));
		
		LOGGER (get_date_time(), "Sending data to client\n");
		sent = sendto (*udp_socket, data_buffer, data_size, 0, 
			(struct sockaddr *)&server_storage, server_address_size);
		if (sent == -1) {
			LOGGER (get_date_time(), "Error sending data to client %u\n", 
					server_storage.sin_port);
			continue;	
		}
		LOGGER (get_date_time(), "Sent \"%s\" to client %u\n", data_buffer, 
				server_storage.sin_port);
	}

	return TRUE;
}

int main (int argc, char const *argv[]) {
	int udp_socket;
	const char* multicast_ipv4_address;
	int port_number;
	struct sockaddr_in server_address;

	if (argc != 3) {
		LOGGER (get_date_time(), "Insufficient number of arguments\n");
		return 1;
	}

	multicast_ipv4_address = argv[1];
	port_number = atoi(argv[2]);
	if (!create_udp_socket(&udp_socket, multicast_ipv4_address, &port_number, 
							&server_address)) {
		LOGGER (get_date_time(), "Program abborting\n");
		return 1;	
	}

	if (!run_udp_server(&udp_socket, multicast_ipv4_address, &port_number)) {
		LOGGER (get_date_time(), "Program abborting\n");
		return 1;
	}
	close(udp_socket);
	
	return 0;
}