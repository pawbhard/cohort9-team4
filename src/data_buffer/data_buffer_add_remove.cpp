#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "data_buffer.h"

using namespace std;



void SwitchDataBuffer::startDataCollection(void) {

    buf_data switch_data;

    //connect to switch
    while(1) {
        //Do SNMP get 
        switch_data.cpu_usage = rand()%100;
        switch_data.temperature = rand()%200;
        switch_data.pkt_rate = rand()%10000;
        if(addToBuffer(switch_data) != SUCCESS)
        {
            cout << "ALERT: Adding data to buffer failed"<<endl;
        }
        sleep(1);

    }
}

int SwitchDataBuffer::addToBuffer(buf_data &data) {
    
    if( cur_buffer_size >= MAX_DATA_BUFSZ ) {
        cout << "ALERT Max Buffer Limit reached for switch "<<switch_name<<endl;
        return FAILURE;
    }

    switch_buf_data[cur_buffer_size].cpu_usage = data.cpu_usage;
    switch_buf_data[cur_buffer_size].temperature = data.temperature;
    switch_buf_data[cur_buffer_size].pkt_rate = data.pkt_rate;

    cur_buffer_size++;

    if(cur_buffer_size >= STORE_LIMIT)
        cout << "Buffer store limit reached/exceeded"<<endl;
}

int SwitchDataBuffer::removeFromBuffer() {
    
}
