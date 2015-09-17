#include <iostream>
#include <list>

#include <unistd.h>
#include <stdlib.h>

#include "data_buffer.h"

using namespace std;

pthread_mutex_t snmp_lock;

int doSnmpGet(const char *cur_oid, const char *ip, int *data_ptr) {
    
    struct snmp_pdu *pdu;                   
    struct snmp_pdu *response;
    struct snmp_session session, *sess_handle;

    struct variable_list *vars;            

    oid id_oid[MAX_OID_LEN];
    oid serial_oid[MAX_OID_LEN];

    size_t id_len = MAX_OID_LEN;
    size_t serial_len = MAX_OID_LEN;

    int status;                             
    int retval;
    
    pthread_mutex_lock(&snmp_lock);

    sess_handle = snmp_open(&session);
    init_snmp("MibCheck");

    snmp_sess_init( &session );
    session.version = SNMP_VERSION_2c;
    session.community = (u_char *)SNMP_COMMUNITY;
    session.community_len = strlen(SNMP_COMMUNITY);
    session.peername = (char *)ip;

    add_mibdir(".");

    pdu = snmp_pdu_create(SNMP_MSG_GET);

    if( !snmp_parse_oid(cur_oid, id_oid, &id_len)) {
        cout << "snmp oid parsing failed"<<endl;
        return -1;
    }
    
    snmp_add_null_var(pdu, id_oid, id_len);

    status = snmp_synch_response(sess_handle, pdu, &response);
    if( status == 0) {
        for(vars = response->variables; vars; vars = vars->next_variable) {
                *data_ptr = (int) *(vars->val.integer);
                retval = SUCCESS;
                break;
        }
    } else {
        *data_ptr = -1;
        retval = FAILURE;
    }
    
    snmp_free_pdu(response);
    snmp_close(sess_handle);

    pthread_mutex_unlock(&snmp_lock);

    return retval;
}


void SwitchDataBuffer::stopDataCollection(void) {
    collect = false;
}


void SwitchDataBuffer::startDataCollection(void) {

    buf_data switch_data;
    bool wait = false;
    collect = true;

    //connect to switch
    while(collect == true) {
        //Do SNMP get 
        if ( doSnmpGet("CISCO-PROCESS-MIB::cpmCPUTotal1minRev.1", switch_ip.c_str(), &switch_data.cpu_rate) == FAILURE
            || doSnmpGet("CISCO-PROCESS-MIB::cpmCPUMemoryUsed.1", switch_ip.c_str(), &switch_data.cpu_mem_usage) == FAILURE) {
            wait = true;
        }

        if( wait == false) {
            if(addToBuffer(switch_data) != SUCCESS)
            {
                cout << "ALERT: Adding data to buffer failed"<<endl;
            }
        }

        if( hardLimitReached() || wait == true ) {
            sleep(1);
            wait = false;
        }
    }
}

int SwitchDataBuffer::addToBuffer(buf_data &data) {
    
    pthread_mutex_lock(&lock);

    if( cur_buffer_size >= MAX_DATA_BUFSZ ) {
        cout << "ALERT: Max Buffer Limit reached for switch "<<switch_name<<endl;
        return FAILURE;
    }
    
    buf_data_node *buf = new buf_data_node;
    
    buf->data = data;
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
            cur_buffer_size--;
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
            cout << "cpu usage :"<<tmp->data.cpu_rate<<" cpu mem usage:"<<tmp->data.cpu_mem_usage<<endl;
            tmp = tmp->next;
        } while( tmp != buf_tail );
    }
}

bool SwitchDataBuffer::storeLimitReached() {

    if(cur_buffer_size >= STORE_LIMIT-1)
        return true;
    return false;
}

bool SwitchDataBuffer::hardLimitReached() {
    if(cur_buffer_size >= HARD_LIMIT-1)
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


