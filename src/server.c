#include "../include/server.h"

static int run_udp_server (int* udp_socket) {
	
	struct sockaddr_in* client_address;
	struct sockaddr_storage server_storage;
	char data_buffer[BUFFER_SIZE];
	int data_size;
	int sent;
	socklen_t server_address_size = sizeof(server_storage);

	logger ("UDP server has started\n");

	while (TRUE) {
		data_size = recvfrom (*udp_socket, data_buffer, BUFFER_SIZE, 0, 
					(struct sockaddr *)&server_storage, &server_address_size);

		client_address = (struct sockaddr_in*)&server_storage;
		
		if (data_size == -1) {
			logger ("Error receiving data from client %u\n", 
					client_address->sin_port);	
			continue;
		}

		logger ("Received %s from client %u\n", data_buffer, 
				client_address->sin_port);

		for (int i=0; i<(data_size-1) ; i++) { 
			data_buffer[i] = toupper (data_buffer[i]);
		}
		
		sent = sendto (*udp_socket, data_buffer, data_size, 0, 
			(struct sockaddr *)&server_storage, server_address_size);
		if (sent == -1) {
			logger ("Error sending data to client %u\n", 
					client_address->sin_port);
			continue;	
		}
		logger ("Sent %s to client %u\n", data_buffer, 
				client_address->sin_port);
	}

	return TRUE;
}

int main (int argc, char const *argv[]) {
	int udp_socket;
	const char* ipv4_address;
	int port_number;
	struct sockaddr_in server_address;

	if (argc != 3) {
		logger ("Insufficient number of arguments\n");
		return 1;
	}

	ipv4_address = argv[1];
	port_number = atoi(argv[2]);
	if (!create_udp_socket(&udp_socket, ipv4_address, &port_number, 
							&server_address)) {
		logger ("Program abborting\n");
		return 1;	
	}

	if (!run_udp_server(&udp_socket)) {
		logger ("Program abborting\n");
		return 1;
	}

	return 0;
}