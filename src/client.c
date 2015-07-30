#include <stdio.h>
#include "../include/client.h"

static int send_data_to_server (int* udp_socket,
								const char* server_ipv4_address,
								const int* client_port_number,
								const int* server_port_number) {

	socklen_t server_address_size = sizeof(struct sockaddr_storage);
	struct sockaddr_in server_address;
	char data_buffer[BUFFER_SIZE] = "temp";
	int sent;
	int received;

   	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (*server_port_number);
	server_address.sin_addr.s_addr = inet_addr (server_ipv4_address);
	memset (server_address.sin_zero, '\0', sizeof (server_address.sin_zero));  

	while (TRUE) {
		LOGGER (get_date_time(), "Enter message: ");
		fgets (data_buffer, BUFFER_SIZE, stdin);
		data_buffer[strcspn(data_buffer, "\n")] = 0;

		LOGGER (get_date_time(), "Sending \" %s \" to the server %u\n", 
				data_buffer, server_address.sin_port);

		sent = sendto (*udp_socket, data_buffer, sizeof(data_buffer), 0,
				(struct sockaddr *)&server_address, server_address_size);
		if (sent == -1) {
			LOGGER (get_date_time(), "Error sending data to server %u\n", 
					server_address.sin_port);
			continue;	
		}
		LOGGER (get_date_time(), "Sent \" %s \" to server %u\n", data_buffer, 
				server_address.sin_port);
		LOGGER (get_date_time(), "Waiting to receive data from server %u\n", 
				server_address.sin_port);

    	received = recvfrom (*udp_socket, data_buffer, BUFFER_SIZE, 0, 
					(struct sockaddr *)&server_address, &server_address_size);

    	if (received == -1) {
			LOGGER (get_date_time(), "Error receiving data from server %u\n", 
					server_address.sin_port);
			continue;	
		}
    	LOGGER (get_date_time(), "Received message from server: %s\n",data_buffer);
  	}

  	return TRUE;
}

int main(int argc, char const *argv[]) {
	int udp_socket;
	const char* multicast_ipv4_address;
	int client_port_number;
	int server_port_number;
	struct sockaddr_in client_address;

	if (argc != 4) {
		LOGGER (get_date_time(), "Insufficient number of arguments\n");
		return 1;
	}

	multicast_ipv4_address = argv[1];
	client_port_number = atoi (argv[2]);
	server_port_number = atoi (argv[3]);
	
	if (!create_udp_socket (	&udp_socket, multicast_ipv4_address, 
								&client_port_number, &client_address)) {
		LOGGER (get_date_time(), "Program abborting\n");
		return 1;	
	}

	if (!send_data_to_server(	&udp_socket, multicast_ipv4_address, 
								&client_port_number, &server_port_number)) {
		LOGGER (get_date_time(), "Program abborting\n");
		return 1;	
	}
	close(udp_socket);
	return 0;
}