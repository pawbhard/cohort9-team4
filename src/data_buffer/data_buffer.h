#ifndef data_buffer_h
#define data_buffer_h

#include <string>

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "thread_common.h"

#define SUCCESS 0
#define FAILURE -1

#define STORE_LIMIT 10
#define HARD_LIMIT 20
#define MAX_DATA_BUFSZ 40
#define SNMP_COMMUNITY "demopublic"
#define MAX_SWITCHES 10

#define CPU_RATE 0
#define CPU_MEM_USAGE 1
#define MAX_PARAMS 2

using namespace std;

typedef struct buf_data_t_ {
    int data[MAX_DATA_BUFSZ];
    int head;
    int tail;
    int size;
    pthread_mutex_t lock;
} buf_data_t;


class SwitchDataBuffer:public Thread {
    private:
        int switch_id;
        string switch_name;
        string switch_ip;
        
        buf_data_t buf[MAX_PARAMS];

        static int switch_count;
        bool collect;
        
        struct snmp_session session;
        
        void startDataCollection(void);
        void snmpSessionInit();
        void bufferAdd(buf_data_t *buf, int data);

    public:
        SwitchDataBuffer(string, string);
        void stopDataCollection(void);
        void printBufferData();
        
        buf_data_t * getBuffer(int type);

        int *getData(buf_data_t *buf);
        bool bufLimitReached(buf_data_t *buf, int limit);

        virtual void run();
};




#endif
