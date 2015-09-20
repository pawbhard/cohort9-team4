#ifndef data_buffer_h
#define data_buffer_h

#include <string>
#include "../common_util/threadpool.h"

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>

#include "thread_common.h"

#define SUC 0
#define FAIL -1

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
    int *data;
    int head;
    int tail;
    int size;
    int type;
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
        void startDataCollection(void);
        void snmpSessionInit();
        int bufferAdd(buf_data_t *buf, int data);

    public:
        SwitchDataBuffer(string, string);
        void stopDataCollection(void);
        void printBufferData();
        
        buf_data_t * getBuffer(int type);

        int *getData(buf_data_t *buf);
        bool bufLimitReached(buf_data_t *buf, int limit);
        void bufferFree(int *);
        virtual void run();
};


void dispatch_routine_cpu_usage( void *arg);
    void dispatch_routine_cpu_rate(void *arg); 
#endif
