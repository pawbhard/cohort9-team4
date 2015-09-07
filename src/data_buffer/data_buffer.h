#ifndef data_buffer_h
#define data_buffer_h

//#include "mythread.h"
#include <string>

#define LIMIT 1024
#define HARD_LIMIT 4000
#define MAX_DATA_BUFSZ 4096
#define MAX_SWITCHES 10

typedef struct buf_data_ {
    int cpu_usage;
    int temperature;
    int pkt_rate;
} buf_data;

class Thread {
};

class SwitchDataBuffer:public Thread {
    private:
        int switch_id;
        std::string switch_name;
        int switch_ip;
        buf_data buffer[MAX_DATA_BUFSZ];
        int cur_buffer_size;
        static int switch_count;

    public:
        SwitchDataBuffer(std::string, int);
        void add_to_buffer();
        void start_data_collection( void *);
};




#endif
