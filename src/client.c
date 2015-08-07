#include <stdio.h>
#include "../include/client.h"

static int listen_to_multicast_server (int* send_udp_socket,
								int* receive_udp_socket,
								struct sockaddr_in *socket_address,	
								const char* multicast_ipv4_address,
								const int* client_port_number,
								const char* server_ipv4_address,
								const int* server_port_number) {

	socklen_t server_address_size = sizeof(struct sockaddr_storage);
	socklen_t socket_address_size = sizeof(struct sockaddr_storage);
	struct sockaddr_in server_address;
	struct ip_mreq multicast_ipv4_request;
	char data_buffer[BUFFER_SIZE];
	int sent;
	int received;
	LOGGER (get_date_time(), "Multicast group %s\n", multicast_ipv4_address);
   	/* Join the multicast group */
   	multicast_ipv4_request.imr_multiaddr.s_addr = 
						inet_addr(multicast_ipv4_address);
	multicast_ipv4_request.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt (*receive_udp_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, 
              &multicast_ipv4_request, sizeof(multicast_ipv4_request))) {
		LOGGER (get_date_time(), "Failed to join multicast group\n");
		perror ("Falied multicast join");
		close (*send_udp_socket);
		close (*receive_udp_socket);
		return FALSE;
	}
	LOGGER (get_date_time(), "Joined multicast group %s\n",
			multicast_ipv4_address);

   	/* Send a message to the server regarding the membership*/
   	server_address.sin_family = AF_INET;
	server_address.sin_port = htons (*server_port_number);
	server_address.sin_addr.s_addr = inet_addr (server_ipv4_address);
	memset (server_address.sin_zero, '\0', sizeof (server_address.sin_zero));
   	
   	snprintf (data_buffer, sizeof(data_buffer), "%s %s", "Join ", 
   			multicast_ipv4_address);
	LOGGER (get_date_time(), "Sending \" %s \" to the multicast server %u\n", 
				data_buffer, ntohs(server_address.sin_port));
	sent = sendto (*send_udp_socket, data_buffer, sizeof(data_buffer), 0,
			(struct sockaddr *)&server_address, server_address_size);
	if (sent == -1) {
		LOGGER (get_date_time(), "Error sending data to multicast server %u\n", 
				ntohs(server_address.sin_port));
		close (*send_udp_socket);
		close (*receive_udp_socket);
		return FALSE;	
	}
	LOGGER (get_date_time(), "Sent \" %s \" to server %u\n", data_buffer, 
			ntohs(server_address.sin_port));
		
   	/* Start listening to multicast messages from the server*/
	while (TRUE) {
		LOGGER (get_date_time(), "Waiting to receive data from multicast "
			"server \n");
    	received = recvfrom (*receive_udp_socket, data_buffer, BUFFER_SIZE, 0, 
					(struct sockaddr *)socket_address, &socket_address_size);
    	if (received == -1) {
			LOGGER (get_date_time(), "Error receiving data from server %u\n", 
					ntohs(socket_address->sin_port));
			continue;	
		}
    	LOGGER (get_date_time(), "Received message from multicast server:"
    		" %s\n", data_buffer);
  	}

  	return TRUE;
}

int main(int argc, char const *argv[]) {
	int send_udp_socket;
	int receive_udp_socket;
	const char* multicast_group_ipv4_address;
	const char* server_ipv4_address;
	int multicast_group_port_number;
	int client_port_number;
	int server_port_number;
	struct sockaddr_in socket_address;

	if (argc != 6) {
		LOGGER (get_date_time(), "Insufficient number of arguments\n");
		LOGGER (get_date_time(), "Usage: ./client "
				"<multicast_group_ipv4_address> <multicast_group_port_number>"
				" <server_ipv4_address> <server_port_number> "
				"<client_port_number>\n");
		return 1;
	}

	multicast_group_ipv4_address = argv[1];
	multicast_group_port_number = atoi(argv[2]);
	server_ipv4_address = argv[3];
	server_port_number = atoi (argv[4]);
	client_port_number = atoi (argv[5]);
	
	if (!create_udp_socket ( &send_udp_socket, NULL, NULL,
		NULL, FALSE)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		return 1;	
	}
	LOGGER (get_date_time(), "Created send UDP socket\n");

	if (!create_udp_socket ( &receive_udp_socket, NULL, 
		&multicast_group_port_number, &socket_address, TRUE)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		close(receive_udp_socket);
		return 1;	
	}
	LOGGER (get_date_time(), "Created receive UDP socket\n");	

	if (!listen_to_multicast_server( &send_udp_socket, &receive_udp_socket,
	 			&socket_address, multicast_group_ipv4_address, 
	 			&client_port_number, server_ipv4_address, 
	 			&server_port_number)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		close(receive_udp_socket);
		return 1;	
	}
	close(send_udp_socket);
	close(receive_udp_socket);	
	return 0;
}