#ifndef data_buffer_h
#define data_buffer_h

#include <string>
#include "thread_common.h"

#define SUCCESS 0
#define FAILURE -1

#define STORE_LIMIT 1024
#define HARD_LIMIT 4000
#define MAX_DATA_BUFSZ 4096
#define MAX_SWITCHES 10

using namespace std;

typedef struct buf_data_ {
    int cpu_usage;
    int temperature;
    int pkt_rate;
    buf_data_ *next;
} buf_data;


class SwitchDataBuffer:public Thread {
    private:
        int switch_id;
        string switch_name;
        int switch_ip;
        buf_data *buf_hdr;
        buf_data *buf_tail;
        int cur_buffer_size;
        static int switch_count;

    public:
        SwitchDataBuffer(string, int);
        void startDataCollection(void);
        int addToBuffer(buf_data &);
        int removeFromBuffer();
        void printBufferData();

        virtual void run();
};




#endif
