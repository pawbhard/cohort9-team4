#include "../../include/client.h"

void execute_task (connection_handler *connection) {
	bool flag = TRUE;
	char data[BUFFER_SIZE];

	do {
		std::cout << get_date_time() << "Waiting for data from server\n";
		read (connection->local_fd, data, BUFFER_SIZE);

	} while (flag);
}