#include "../../include/client.h"

static void calculate_mean(int data[], int size, int result[]) {
	double mean = data[0], diff;
	int i;
	for (i=1;i<size;i++) {
		diff = data[i] - mean;
		mean += diff/(i+1);
	}
	result[1] = (int)mean;
	result[2] = size;
}

static void calculate_standard_deviation(int data[], int size, int result[]) {

}

static void calculate_range(int data[], int size, int result[]) {
	int min = 0, max = 0;
	int i;
	for (i=0;i<size;++i) {
		if (data[i]<min) {
			min = data[i];
		}
		if (data[i]>max) {
			max = data[i];
		}
	}
	result[1] = min;
	result[2] = max;
}

void *client_receive_and_execute_task (void *v_connection) {
	bool flag = TRUE;
	connection_handler* connection = (connection_handler*) v_connection; 
	int data[BUFFER_SIZE];
	int result[RESULT_SIZE];
	task_type task;
	int input_size = 0;
	int *input;
	
	do {
		std::cout << get_date_time() << " Waiting for data from server\n";
		read (connection->local_fd, data, BUFFER_SIZE);
		result[0] = data[1];
		task = (task_type)data[2];
		input_size = (int)data[0];
		input = data+3;
    	switch (task) {
    		case mean: 		std::cout << get_date_time() << " Calculating mean of %d elements\n";
    						calculate_mean(input, input_size, result);
    						break;
    		case standard_deviation:
    						std::cout << get_date_time() << " Calculating standard_deviation for %d elements\n";
    						calculate_standard_deviation(input, input_size, result);
    						break;  
    		case range:		std::cout << get_date_time() << " Calculating range for %d elements\n";
    						calculate_range(input, input_size, result);
    						break;
    		default:		continue;
    	}
    	write (connection->local_fd, result, RESULT_SIZE);
	} while (flag);
	return NULL;
}