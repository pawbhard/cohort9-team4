#include "../../include/client.h"

static void calculate_mean(double data[], uint size, double result[]) {
	double mean = data[0], diff;
	uint i;
	for (i=1;i<size;i++) {
		diff = data[i] - mean;
		mean += diff/(i+1);
	}
	result[1] = mean;
}

static void calculate_standard_deviation(double data[], uint size, double result[]) {

}

static void calculate_range(double data[], uint size, double result[]) {
	double min = 0.0f, max = 0.0f;
	uint i;
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
	double data[BUFFER_SIZE];
	double result[RESULT_SIZE];
	task_type task;
	uint input_size = 0;
	double *input;
	
	do {
		std::cout << get_date_time() << " Waiting for data from server\n";
		read (connection->local_fd, data, BUFFER_SIZE);
		result[0] = data[0];
		task = (task_type)data[1];
		input_size = (uint)data[2];
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