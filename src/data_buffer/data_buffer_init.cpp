#include <iostream>
#include <string>
#include "data_buffer.h"


using namespace std;

int SwitchDataBuffer::switch_count = 0;

SwitchDataBuffer::SwitchDataBuffer(string sname, int ip)
{
    if( switch_count < MAX_SWITCHES ){
        ++switch_count;
        switch_id = switch_count;
        switch_name = sname;
        switch_ip = ip;
        buf_hdr = buf_tail = NULL;
        cur_buffer_size = 0; 
    }
    else {
        //DEBUG Infra
        cout << "ALERT: Limit reached: Cannot add more switches"<<endl;
    }
}

void SwitchDataBuffer::run() {
    startDataCollection();
}



int init_data_buffers() {
    //read from switch db or config file

    string sw_name("s1");
    SwitchDataBuffer sdbuf(sw_name, 0x1010101);
    
    string sw_name2("s2");
    SwitchDataBuffer sdbuf1(sw_name2, 0x2020202);
    //Thread has to be started
    sdbuf.start();
    sdbuf1.start();

    sdbuf.join();
    sdbuf1.join();
    return 0;
}

int main()
{
    return init_data_buffers();
}


