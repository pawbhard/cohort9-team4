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
        cur_buffer_size = 0; 
    }
    else {
        //DEBUG Infra
        cout << "ALERT: Limit reached: Cannot add more switches"<<endl;
    }
}



void SwitchDataBuffer::start_data_collection(void *arg) {
   //spawn threads for each switch to fill individual buffers


}


int init_data_buffers() {
    //read from switch db or config file

    string sw_name("s1");
    SwitchDataBuffer sdbuf(sw_name, 0x1010101);
   
    //Thread has to be started
    //sdbuf.startThread();
    //sdbuf.joinThread();
    return 0;
}

int main()
{
    return init_data_buffers();
}


