#include <iostream>
#include <string>
#include <list>

#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "data_buffer.h"


using namespace std;

int SwitchDataBuffer::switch_count = 0;

SwitchDataBuffer::SwitchDataBuffer(string sname, string ip)
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
    
    int num_switches = 2;
    string conf_arr[2][2] = { {"s1","10.193.52.162"}, {"s2","10.193.52.163"}};

    list <SwitchDataBuffer *> switch_list; 

    for ( int i=0; i<num_switches; i++ )
    {
        SwitchDataBuffer *sdbuf = new SwitchDataBuffer(conf_arr[i][0], conf_arr[i][1]);
        switch_list.push_back(sdbuf);
        sdbuf->start();
    }

    return switch_list;
}

int main()
{
    return 0;
}
