#include <iostream>
#include <string>
#include <list>

#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "data_buffer.h"


using namespace std;

int SwitchDataBuffer::switch_count = 0;

SwitchDataBuffer::SwitchDataBuffer(string sname, in_addr_t ip)
{
    if( switch_count < MAX_SWITCHES ){
        ++switch_count;
        switch_id = switch_count;
        switch_name = sname;
        switch_ip = ip;
        buf_hdr = buf_tail = NULL;
        cur_buffer_size = 0; 
        if (pthread_mutex_init(&lock, NULL) != 0) {
            cout<<"\nlock init failed\n";
        }
    }
    else {
        //DEBUG Infra
        cout << "ALERT: Limit reached: Cannot add more switches"<<endl;
    }
}

void SwitchDataBuffer::run() {
    startDataCollection();
    
    removeAllFromBuffer();
    pthread_mutex_destroy(&lock);
}



list<SwitchDataBuffer *> init_data_buffers() {
    //read from switch db or config file
    
    int num_switches = 3;
    string conf_arr[3][3] = { {"s1","10.10.10.10"}, {"s2","20.20.20.20"}, {"s3","30.30.30.30"}};

    list <SwitchDataBuffer *> switch_list; 

    for ( int i=0; i<num_switches; i++ )
    {
        SwitchDataBuffer *sdbuf = new SwitchDataBuffer(conf_arr[i][0], inet_addr(conf_arr[i][1].c_str()));
        switch_list.push_back(sdbuf);
        sdbuf->start();
    }

    return switch_list;
}

int main()
{
    list<SwitchDataBuffer *> slist_data = init_data_buffers();

    while(1) {}
    return 0;
}


