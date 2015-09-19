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
    int i;
    if( switch_count < MAX_SWITCHES ){
        ++switch_count;
        switch_id = switch_count;
        switch_name = sname;
        switch_ip = ip;
        
        for( i=0; i<MAX_PARAMS; i++) {
            bzero(&buf[i], sizeof(buf_data_t));

            if (pthread_mutex_init(&buf[i].lock, NULL) != 0) {
                cout<<"\nlock init failed\n";
            }
        }
    }
    else {
        //DEBUG Infra
        cout << "ALERT: Limit reached: Cannot add more switches"<<endl;
    }
}

void SwitchDataBuffer::run() {
    startDataCollection();
    
    for(int i=0; i<MAX_PARAMS; i++) 
        pthread_mutex_destroy(&buf[i].lock);
}



list<SwitchDataBuffer *> init_data_buffers() {
    //read from switch db or config file
    
    int num_switches = 2;
    string conf_arr[2][2] = { {"s1","10.193.52.162"}, {"s2","10.193.52.163"}};

    list <SwitchDataBuffer *> switch_list; 

    for ( int i=0; i<num_switches; i++ )
    {
        SwitchDataBuffer *sdbuf = new SwitchDataBuffer(conf_arr[i][0], conf_arr[i][1]);
        sdbuf->start();
        switch_list.push_back(sdbuf);
    }

    return switch_list;
}

/*
int main()
{
    init_data_buffers();
    while(1) { }
    return 0;
}
*/
