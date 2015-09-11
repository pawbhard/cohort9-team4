#include "../../include/client.h"

syserr_t create_client_socket (	connection_handler *connection, 
								char *server_ip,
								int *server_port) {	
	connection->local_fd = socket(AF_INET,SOCK_STREAM,0);
	if (connection->local_fd < 0) {
		cout << get_date_time() << " Socket creation failed\n";
		return FAILURE;
	}
	std::cout << get_date_time() << " TCP Socket created with fd " << connection->local_fd << "\n";
	connection->server_addr.sin_addr.s_addr = inet_addr(server_ip);
	connection->server_addr.sin_family      = AF_INET;
	connection->server_addr.sin_port        = htons(*server_port);
	return SUCCESS;
}

syserr_t establish_server_connection ( 	connection_handler *connection, 
										char *server_ip,
										int *server_port ) {
	
	if (connect(connection->local_fd,
        (struct sockaddr *)&connection->server_addr,
            sizeof(struct sockaddr_in))<0) {
		std::cout << get_date_time() << " Failed to create connection\n";
		return FAILURE;
	}
  	
  	std::cout << get_date_time() <<" Client connected to Server " << server_ip << ":" << server_port << "\n";
  	return SUCCESS;
}
