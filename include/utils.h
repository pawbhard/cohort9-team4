#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <pthread.h>

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 4096
#define RESULT_SIZE 3
#define MAX_Tx_Rx_BUFFER 65534
#define SUCCESS 0
#define FAILURE 1 
#define SYSERR  0
#define JOIN '1'
#define LEAVE '0'

typedef uint32_t syserr_t;

enum task_type {
	mean,
	standard_deviation,
	range
};

static inline char* get_date_time () {
	char* time_data;
	time_t t = time(NULL);
	struct tm *tm = localtime(&t);
	time_data = asctime(tm);
	time_data[strcspn(time_data, "\n")] = 0;
	return time_data;
}