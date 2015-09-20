#include "../../include/client.h"

static void calculate_mean(uint32_t data[], uint32_t size, uint32_t result[]) {
	double mean = data[0], diff;
	uint32_t i;
	for (i=1;i<size;i++) {
		diff = data[i] - mean;
		mean += diff/(i+1);
	}
	result[1] = (uint32_t)mean;
	result[2] = size;
}

static void calculate_standard_deviation(uint32_t data[], uint32_t size, uint32_t result[]) {

}

static void calculate_range(uint32_t data[], uint32_t size, uint32_t result[]) {
	uint32_t min = 0, max = 0;
	uint32_t i;
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

extern void *client_receive_and_execute_task (void* temp_con){
	struct g_conn_hndl_t *local_con_hndlr = (struct g_conn_hndl_t*)temp_con;
	bool flag = TRUE;
	client_server_msg_t data;
	task_type task;
	uint32_t input_size = 0;
	uint32_t *input;
	
	do {
		std::cout << get_date_time() << " Waiting for data from server\n";
		read (local_con_hndlr->local_fd, &data, MESSAGE_SIZE);
		data.cs_comput_output[0] = data.c_payload[0];	
		task = (task_type)data.c_payload[1];
		input_size = (uint32_t)data.c_payload[2];
		input = data.c_payload + 3;
    	switch (task) {
    		case mean: 		std::cout << get_date_time() << " Calculating mean of %d elements\n";
    						calculate_mean(input, input_size, data.cs_comput_output);
    						break;
    		case standard_deviation:
    						std::cout << get_date_time() << " Calculating standard_deviation for %d elements\n";
    						calculate_standard_deviation(input, input_size, data.cs_comput_output);
    						break;  
    		case range:		std::cout << get_date_time() << " Calculating range for %d elements\n";
    						calculate_range(input, input_size, data.cs_comput_output);
    						break;
    		default:		continue;
    	}
    	data.cs_msg_type = MSG_COMPUTE_TASK_OUTPUT;
    	write (local_con_hndlr->local_fd, &data, MESSAGE_SIZE);
	} while (flag);
	return NULL;
}