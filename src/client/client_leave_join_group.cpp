#include "../../include/client.h"

extern void *client_group_join_or_leave (void *v_connection) {
	char choice = '\0';
	char group_list[BUFFER_SIZE] = "";
	bool flag = TRUE;
	connection_handler *connection = (connection_handler*) v_connection;
	std::cout << get_date_time() << " Server Ready\n";
	do {
		std::cout << get_date_time() <<"\n1. Join Group\n2. Leave Group\n3. Exit program\nWhat would you like to do? ";
		cin >> choice;
		switch(choice) {
			case '1': 	std::cout << get_date_time() << " Retreiving the list of available groups from the server...\n";
						strcpy(group_list,"");
						display_group_list_from_server(connection, AVAILABLE);
 						std::cout << get_date_time() << " Enter the list of groups you want to join (Eg: 1,2,3,4) : ";
 						cin >> group_list;
 						send_join_or_leave_messsage(connection, group_list, JOIN);
						break;
			case '2':	std::cout << get_date_time() << " List of registered groups\n";
						strcpy(group_list,"");
						display_group_list_from_server(connection, REGISTERED);
						std::cout << get_date_time() << " Enter the list of groups you want to leave (Eg: 1,2,3,4) : ";
						cin >> group_list;
 						send_join_or_leave_messsage(connection, group_list, LEAVE);
						break;
			case '3':	std::cout << get_date_time() << " Leaving all the groups\n";
						strcpy(group_list, "all");
						send_join_or_leave_messsage(connection, group_list, LEAVE);
						flag = FALSE;
						close (connection->local_fd);
						break;
			default:	continue; 
		}
	} while(flag);
	return NULL;
}

void display_group_list_from_server(connection_handler *connection, bool available_register) {
	char groups[BUFFER_SIZE];
	char message[100];
	strcpy(message,available_register?"Requesting list of available groups":"Requesting list of registered groups");
	write (connection->local_fd, message, sizeof(message));
	read (connection->local_fd, groups, sizeof(groups));
	cout << get_date_time() << groups << endl;
}

void send_join_or_leave_messsage(connection_handler *connection, char* group_list, char join_leave) {
	char groups[BUFFER_SIZE+10] = {'r','e','g',' ',join_leave,' ','\0'};
	strcat(groups, group_list);
	write (connection->local_fd, groups, sizeof(groups));
	std::cout << "\n" << get_date_time() << " Sending registration messages to these groups : " << groups << "\n";
}