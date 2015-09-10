#ifndef data_buffer_h
#define data_buffer_h

#include <string>
#include <netinet/in.h>

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
} buf_data;

typedef struct buf_data_node_ {
    buf_data data;
    buf_data_node_ *next;
} buf_data_node;


class SwitchDataBuffer:public Thread {
    private:
        int switch_id;
        string switch_name;
        in_addr_t switch_ip;
        buf_data_node *buf_hdr;
        buf_data_node *buf_tail;
        int cur_buffer_size;
        static int switch_count;
        pthread_mutex_t lock;
        bool collect;

    public:
        SwitchDataBuffer(string, in_addr_t);
        void startDataCollection(void);
        void stopDataCollection(void);
        int addToBuffer(buf_data &);
        bool storeLimitReached();
        bool hardLimitReached();
        void removeAllFromBuffer();
        list<buf_data> getListOfDataAndFlush();
        void printBufferData();

        virtual void run();
};




#endif
