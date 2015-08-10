#include "../include/server.h"

static multicast_group* find_multicast_group (multicast_group* head, ipv4_address group) {
	multicast_group* cur = head;
	while (cur) {
		if (compare_ipv4_address(cur->group_addr, group)) {
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

static int add_multicast_group (multicast_group* head, ipv4_address group) {
	multicast_group* cur = head;
	multicast_group* node = (multicast_group*)malloc(sizeof(multicast_group));
	if (!node) {
		return FALSE;
	}
	node->next = NULL;
	node->client_list = NULL;
	node->group_addr = group;
	if (!cur) {
		cur = node;
		return TRUE;
	}
	while (cur->next) {
		cur = cur->next;
	}
	cur->next = node;
	return TRUE;
}

static int remove_client_list (client_node* head) {
	client_node* cur;
	client_node* node;
	if (!head) {
		return FALSE;
	}
	cur = head;
	while (cur) {
		node = cur;
		cur = cur->next;
		node->next = NULL;
		free (node);
	}
	return TRUE;
}

static int remove_multicast_group (multicast_group* head, ipv4_address group) {
	multicast_group* cur = find_multicast_group (head, group);
	multicast_group* prev = head;
	multicast_group* node;
	
	if (!cur) {
		return FALSE;
	}
	if (cur == head) {
		head = head->next;
		remove_client_list (cur->client_list);
		free (cur);
		return TRUE;		
	}
	while (prev->next != cur) {
		prev = prev->next;
	}
	node = cur;
	prev->next = cur->next;
	node->next = NULL;
	remove_client_list (node->client_list);
	free (node);
	return TRUE;
}

static int add_client_to_multicast_group (multicast_group* head, ipv4_address group, ipv4_address client, int id, char* message) {
	multicast_group* cur_group = find_multicast_group (head, group);
	client_node* cur_client;
	client_node* node;
	client_node* check;
	if (!cur_group) {
		return FALSE;
	}
	cur_client = cur_group->client_list; 
	check = cur_client;
	while (check) {
		if (compare_ipv4_address(check->client_addr, client)) {
			return add_message_to_list (check->msg, message);
		}
		check = check->next;
	}
	
	node = (client_node*)malloc(sizeof(client_node));
	if (!node) {
		return FALSE;
	}
	node->client_addr = client;
	node->id = id;
	add_message_to_list (node->msg, message);
	node->next = NULL;

	if (!cur_client) {
		cur_client = node;
		return TRUE;
	}
	while (cur_client->next) {
		cur_client = cur_client->next;
	}
	cur_client->next = node;
	return TRUE;
}

static int remove_client_from_multicast_group (multicast_group* head, ipv4_address group, int id) {
	multicast_group* cur_group = find_multicast_group (head, group);
	client_node* cur_client;
	client_node* prev_client;
	client_node* node;
	if (!cur_group) {
		return FALSE;
	}
	prev_client = cur_group->client_list;
	if (!prev_client) {
		return FALSE;
	}
	if (prev_client->id == id) {
		node = prev_client;
		prev_client = prev_client->next;
		node->next = NULL;
		free (node);
		return TRUE;
	}
	cur_client = prev_client->next;
	while (cur_client) {
		if (cur_client->id == id) {
			node = cur_client;
			prev_client->next = cur_client->next;
			node->next = NULL;
			free (node);
			return TRUE;
		}
		prev_client = cur_client;
		cur_client = cur_client->next;
	}
	return FALSE;
}


static int run_multicast_udp_server (int* send_udp_socket,
							int* receive_udp_socket,
							struct sockaddr_in *socket_address,
							const char* multicast_group_ipv4_address,
							const int* multicast_group_port_number,
							const int* multicast_server_port_number) {
	
	struct sockaddr_in multicast_group_storage;
	char data_buffer[BUFFER_SIZE];
	int data_size;
	int sent;
	socklen_t multicast_server_address_size = sizeof(struct sockaddr_storage);
	socklen_t socket_address_size = sizeof(struct sockaddr_storage);
		
	memset(&multicast_group_storage, 0, sizeof(struct sockaddr_in));	
	
	multicast_group_storage.sin_family = AF_INET;
	multicast_group_storage.sin_port = htons (*multicast_group_port_number);
	multicast_group_storage.sin_addr.s_addr = 
						inet_addr (multicast_group_ipv4_address);
	
	LOGGER (get_date_time(), "Multicast UDP server has started\n");
	while (TRUE) {

		LOGGER (get_date_time(), "Waiting for clients to join"
				" the multicast group\n");
		data_size = recvfrom (*receive_udp_socket, data_buffer, BUFFER_SIZE, 0, 
				(struct sockaddr *)socket_address, &socket_address_size);

		if (data_size == -1) {
			LOGGER (get_date_time(), "Error receiving data from client %u\n", 
					ntohs(socket_address->sin_port));
			continue;
		}

		LOGGER (get_date_time(), "Received \" %s \" from client %s \n", 
				data_buffer, inet_ntoa (socket_address->sin_addr));
		snprintf (data_buffer, BUFFER_SIZE, "Client has joined the "
			"multicast group %s ", multicast_group_ipv4_address);
		
		LOGGER (get_date_time(), "Sending data \" %s \" to multicast group\n", 
				data_buffer);
		sent = sendto (*send_udp_socket, data_buffer, sizeof(data_buffer), 0, 
					(struct sockaddr *)&multicast_group_storage, 
					multicast_server_address_size);

		if (sent == -1) {
			perror("Send to Error");
			LOGGER (get_date_time(), "Error sending data to multicast group %u\n", 
					multicast_group_storage.sin_port);
			LOGGER (get_date_time(), "Continue waiting for clients\n");
			continue;	
		}
		LOGGER (get_date_time(), "Sent \" %s \" to multicast group %s:%u\n", 
			data_buffer, multicast_group_ipv4_address, 
			ntohs(multicast_group_storage.sin_port));
	}
	close (*send_udp_socket);
	close (*receive_udp_socket);
	return TRUE;
}

int main (int argc, char const *argv[]) {
	int send_udp_socket;
	int receive_udp_socket;
	const char* multicast_group_ipv4_address;
	int multicast_group_port_number;
	int server_port_number;
	struct sockaddr_in socket_address;

	if (argc != 4) {
		LOGGER (get_date_time(), "Insufficient number of arguments\n");
		LOGGER (get_date_time(), "Usage: ./server "
				"<multicast_group_ipv4_address> <multicast_group_port_number>"
				" <server_port_number>\n");
		return 1;
	}

	multicast_group_ipv4_address = argv[1];
	multicast_group_port_number = atoi(argv[2]);
	server_port_number = atoi(argv[3]);
	
	if (!create_udp_socket(&send_udp_socket, NULL, NULL, 
		NULL, FALSE)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		return 1;	
	}
	LOGGER (get_date_time(), "Created send UDP socket\n");

	if (!create_udp_socket(&receive_udp_socket, NULL, 
		&server_port_number, &socket_address, TRUE)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		close(receive_udp_socket);
		return 1;	
	}
	LOGGER (get_date_time(), "Created receive UDP socket\n");

	if (!run_multicast_udp_server(&send_udp_socket, &receive_udp_socket, 
		&socket_address, multicast_group_ipv4_address, 
		&multicast_group_port_number, &server_port_number)) {
		LOGGER (get_date_time(), "Program abborting\n");
		close(send_udp_socket);
		close(receive_udp_socket);
		return 1;
	}
	close(send_udp_socket);
	close(receive_udp_socket);
		
	return 0;
}