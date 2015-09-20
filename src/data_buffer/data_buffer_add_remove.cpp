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
    int retval = FAILURE;
    
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

bool SwitchDataBuffer::bufLimitReached(buf_data_t *buf, int limit){
    if(buf->size >= limit)
        return true;
    return false;
}

void SwitchDataBuffer::bufferAdd(buf_data_t *buf, int data) {
    pthread_mutex_lock(&buf->lock);
    
    if( !bufLimitReached(buf, HARD_LIMIT)) {
        buf->data[buf->tail] = data;
        buf->tail = (buf->tail+1)%MAX_DATA_BUFSZ;
        cout << "Inserted data "<<data<<" Head: "<<buf->head<<" Tail: "<<buf->tail<<endl;
        buf->size++;
    } else {
        cout<<"Buffer Add failed due to hardlimit reached"<<endl;
    }

    pthread_mutex_unlock(&buf->lock);

    if( bufLimitReached(buf, STORE_LIMIT)) {
        int *tmp = getData(buf);
        free(tmp);
    }
}

int * SwitchDataBuffer::getData(buf_data_t *buf) {
    
    int i = 0;
    
    int *raw_buffer = NULL;

    pthread_mutex_lock(&buf->lock);
    if(buf->head == buf->tail) {
        pthread_mutex_unlock(&buf->lock);
        return NULL;
    }
    else {
        if( bufLimitReached(buf, STORE_LIMIT)) {
            raw_buffer = (int *)malloc(sizeof(int)*(STORE_LIMIT));
            while(i < STORE_LIMIT) {
                cout<<"Get Buf Data "<<buf->data[buf->head]<<" Head :"<<buf->head<<" Tail: "<<buf->tail<<endl;
                raw_buffer[i] = buf->data[buf->head++];
                buf->head = buf->head%MAX_DATA_BUFSZ;
                buf->size--;
                i++;           
            }
        }
    }

    pthread_mutex_unlock(&buf->lock);
    return raw_buffer;
}

void SwitchDataBuffer::startDataCollection(void) {

    bool wait = false;
    collect = true;
    
    int data;

    //connect to switch
    while(collect == true) {
        //Do SNMP get 
        if ( doSnmpGet("CISCO-PROCESS-MIB::cpmCPUTotal1minRev.1", switch_ip.c_str(), &data) == SUCCESS) {
            bufferAdd(&buf[CPU_RATE], data);
        } else {
            wait = true;
            cout << "SNMP Get failed CPU rate"<<endl;
        }

        if(doSnmpGet("CISCO-PROCESS-MIB::cpmCPUMemoryUsed.1", switch_ip.c_str(), &data) == SUCCESS) {
            bufferAdd(&buf[CPU_MEM_USAGE], data);
        }
        else {
            wait = true;
            cout << "SNMP Get failed CPU mem"<<endl;
        }

        if( wait == true ) {
            sleep(1);
            wait = false;
        }
    }
}

/*
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

*/
