#ifndef data_buffer_h
#define data_buffer_h

#include <string>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "thread_common.h"

#define SUCCESS 0
#define FAILURE -1

#define STORE_LIMIT 128
#define HARD_LIMIT 256
#define MAX_DATA_BUFSZ 4096
#define SNMP_COMMUNITY "demopublic"
#define MAX_SWITCHES 10

using namespace std;

typedef struct buf_data_ {
    int cpu_rate;
    int cpu_mem_usage;
} buf_data;

typedef struct buf_data_node_ {
    buf_data data;
    buf_data_node_ *next;
} buf_data_node;


class SwitchDataBuffer:public Thread {
    private:
        int switch_id;
        string switch_name;
        string switch_ip;
        buf_data_node *buf_hdr;
        buf_data_node *buf_tail;
        int cur_buffer_size;
        static int switch_count;
        pthread_mutex_t lock;
        bool collect;
        
        struct snmp_session session;
        
        void startDataCollection(void);
        int addToBuffer(buf_data &);
        void removeAllFromBuffer();
        void snmpSessionInit();

    public:
        SwitchDataBuffer(string, string);
        void stopDataCollection(void);
        bool storeLimitReached();
        bool hardLimitReached();
        list<buf_data> getListOfDataAndFlush();
        void printBufferData();

        virtual void run();
};




#endif
