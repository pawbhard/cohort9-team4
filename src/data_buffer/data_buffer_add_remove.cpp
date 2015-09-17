#include <iostream>
#include <list>

#include <unistd.h>
#include <stdlib.h>

#include "data_buffer.h"

using namespace std;

void SwitchDataBuffer::stopDataCollection(void) {
    collect = false;
}


void SwitchDataBuffer::startDataCollection(void) {

    buf_data switch_data;
    collect = true;

    //connect to switch
    while(collect == true) {
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
    
    pthread_mutex_lock(&lock);

    if( cur_buffer_size >= MAX_DATA_BUFSZ ) {
        cout << "ALERT: Max Buffer Limit reached for switch "<<switch_name<<endl;
        return FAILURE;
    }
    
    buf_data_node *buf = new buf_data_node;

    buf->data.cpu_usage = data.cpu_usage;
    buf->data.temperature = data.temperature;
    buf->data.pkt_rate = data.pkt_rate;

    if( cur_buffer_size == 0 || buf_hdr == NULL ) {
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

    pthread_mutex_unlock(&lock);
    return SUCCESS;
}

list<buf_data> SwitchDataBuffer::getListOfDataAndFlush() {
    
    int count=0;

    list<buf_data> buf_list;
    buf_data_node *cur=NULL, *tmp = buf_hdr;

    pthread_mutex_lock(&lock);
    if(storeLimitReached()) {
    
        do { 
            cur = tmp;
            buf_list.push_back(tmp->data);
            tmp = tmp->next;
            delete cur;
            count++;        
        } while( tmp != buf_tail && count < STORE_LIMIT);
        
        buf_hdr = tmp;
        buf_tail->next = buf_hdr;

    }
    pthread_mutex_unlock(&lock);
    return buf_list;
}

void SwitchDataBuffer::printBufferData() {
    
    if( buf_hdr == NULL || cur_buffer_size == 0 )
        cout << "list empty"<<endl;
    else {
        buf_data_node *tmp = buf_hdr;
        cout << endl<<switch_name<<":"<<switch_id<<":cur buffer size :"<<cur_buffer_size<<endl;
        do {
            cout << "cpu usage :"<<tmp->data.cpu_usage<<" temp:"<<tmp->data.temperature<<" pkt rate:"<<tmp->data.pkt_rate<<endl;
            tmp = tmp->next;
        } while( tmp != buf_tail );
    }
}

bool SwitchDataBuffer::storeLimitReached() {

    if(cur_buffer_size >= STORE_LIMIT+1)
        return true;
    return false;
}

bool SwitchDataBuffer::hardLimitReached() {
    if(cur_buffer_size >= HARD_LIMIT+1)
        return true;
    return false;
}


void SwitchDataBuffer::removeAllFromBuffer() {
    
    pthread_mutex_lock(&lock);
    if( buf_hdr == NULL )
        cout << "list empty"<<endl;
    else {
        buf_data_node *cur, *tmp = buf_hdr;

        do {
            cur = tmp;
            tmp = tmp->next;
            delete cur;
            cur_buffer_size--;
        } while(tmp != buf_tail);

        delete tmp;
        cur_buffer_size--;
    }
    pthread_mutex_unlock(&lock);
}
