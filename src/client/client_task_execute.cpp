#include "../../include/client.h"

static void calculate_mean(double data[], uint size, char* result) {
	double mean = data[0], diff;
	uint i;
	for (i=1;i<size;i++) {
		diff = data[i] - mean;
		mean += diff/(i+1);
	}
	snprintf (result, RESULT_SIZE, "%lf", mean);
}
static void calculate_median(double data[], uint size, char* result) {
}
static void calculate_standard_deviation(double data[], uint size, char* result) {
}
static void calculate_range(double data[], uint size, char* result) {
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
	snprintf (result, RESULT_SIZE, "%lf %lf", min, max);
}

void *client_receive_and_execute_task (void *v_connection) {
	bool flag = TRUE;
	connection_handler* connection = (connection_handler*) v_connection; 
	char data[BUFFER_SIZE];
	char *result;
	task_type task_id;
	uint input_size = 0, i, j, k;
	double *input;

	
	do {
		std::cout << get_date_time() << "Waiting for data from server\n";
		read (connection->local_fd, data, BUFFER_SIZE);
		task_id = (task_type)atoi(data);
		i = 2;
		j = 2;
		while (data[i]!=' ') {
        	i++;
    	}
	   	input_size = atoi(data+j);
	   	input = (double*)malloc(sizeof(double)*input_size);
	   	result = (char*)malloc(sizeof(char*)*RESULT_SIZE);
	   	k = 0;
    	while (data[i]) {
        	if (data[i]==' ') {
            	input[k++] = strtod(data+i-j, NULL);
            	j = 0;
        	} else {
            	j++;
        	}
        	i++;
    	}
    	input[k] = strtod(data+i-j, NULL);
    	switch (task_id) {
    		case mean: 		std::cout << get_date_time() << "Calculating mean of %d elements\n";
    						calculate_mean(input, input_size, result);
    						break;
    		case median:	std::cout << get_date_time() << "Calculating median of %d elements\n";
    						calculate_median(input, input_size, result);
    						break; 
    		case standard_deviation:
    						std::cout << get_date_time() << "Calculating standard_deviation for %d elements\n";
    						calculate_standard_deviation(input, input_size, result);
    						break;  
    		case range:		std::cout << get_date_time() << "Calculating range for %d elements\n";
    						calculate_range(input, input_size, result);
    						break;
    		default:		free (input);
    						continue;
    	}
    	write (connection->local_fd, result, RESULT_SIZE);
    	free (input);
    	free (result);
	} while (flag);
	return NULL;
}