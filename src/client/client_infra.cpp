#include "../../include/client.h"

void show_usage () {
	std::cout << "\nUsage: ./client server_ip_address server_port_number\n";
	std::cout << "Example: ./client 192.168.0.10 7500\n";
}

int main (int argc , const char *argv[]) {
	connection_handler client_server_connection;
	char server_ip[16] = "0.0.0.0";
	int server_port = 0;
	pthread_t registeration_thread;
	//pthread_t task_thread;

	if (argc < 3) {
		show_usage();	
		return FAILURE;
	}

	strcpy(server_ip, argv[1]);
	server_port = atoi(argv[2]);

	if (!create_client_socket(&client_server_connection, server_ip, &server_port) < 0) {
		std::cout << get_date_time() << "Failed to Create Client Socket\n";
		return FAILURE;
	}
	
	if (!establish_server_connection(&client_server_connection, server_ip, &server_port)) {
		std::cout << get_date_time() << "Failed to connect to server\n";
		return FAILURE;	
	}

	if (!pthread_create(&registeration_thread, NULL, client_group_join_or_leave, (void*) &client_server_connection)) {
		std::cout << get_date_time() << "Unable to create registration thread\n";
		return FAILURE;
	}
	/*
	if (!pthread_create(&task_thread, NULL, &client_receive_and_execute_task, &client_server_connection)) {
		std::cout << get_date_time() << "Unable to create task thread\n";
		return FAILURE;	
	}
	*/
	return SUCCESS;
}