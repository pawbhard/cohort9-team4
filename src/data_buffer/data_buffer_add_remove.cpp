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
        printBufferData();
        sleep(1);

    }
}

int SwitchDataBuffer::addToBuffer(buf_data &data) {
    
    if( cur_buffer_size >= MAX_DATA_BUFSZ ) {
        cout << "ALERT: Max Buffer Limit reached for switch "<<switch_name<<endl;
        return FAILURE;
    }
    
    buf_data *buf = new buf_data;

    buf->cpu_usage = data.cpu_usage;
    buf->temperature = data.temperature;
    buf->pkt_rate = data.pkt_rate;

    if( buf_hdr == NULL ) {
        buf_hdr = buf;
        buf_tail = buf;
    }
    else {
        buf_tail->next = buf;
        buf_tail = buf;
    }
    buf->next = buf_hdr;

    cur_buffer_size++;

    if(cur_buffer_size >= STORE_LIMIT)
        cout << "Buffer store limit reached/exceeded"<<endl;
    return SUCCESS;
}

void SwitchDataBuffer::printBufferData() {
    
    if( buf_hdr == NULL )
        cout << "list empty"<<endl;
    else {
        buf_data *tmp = buf_hdr;
        cout << endl<<switch_name<<":"<<switch_id<<":cur buffer size :"<<cur_buffer_size<<endl;
        do {
            cout << "cpu usage :"<<tmp->cpu_usage<<" temp:"<<tmp->temperature<<" pkt rate:"<<tmp->pkt_rate<<endl;
            tmp = tmp->next;
        } while( tmp != buf_tail );
    }
}


int SwitchDataBuffer::removeFromBuffer() {
    
}
