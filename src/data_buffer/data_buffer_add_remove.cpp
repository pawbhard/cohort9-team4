#include <iostream>
#include <list>

#include <unistd.h>
#include <stdlib.h>

#include "data_buffer.h"
#include "../server/server_header.h"

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

    add_mibdir("../data_buffer");

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

buf_data_t* SwitchDataBuffer::getBuffer(int type) {
    return &buf[type];
}

bool SwitchDataBuffer::bufLimitReached(buf_data_t *buf, int limit){
    if(buf->size >= limit)
        return true;
    return false;
}

void SwitchDataBuffer::bufferFree(int *buf) {
    if(buf) {
        free(buf);
    }
}

int SwitchDataBuffer::bufferAdd(buf_data_t *buf, int data) {
    pthread_mutex_lock(&buf->lock);
    
    if( !bufLimitReached(buf, MAX_DATA_BUFSZ-1)) {
        buf->data[buf->tail] = data;
        buf->tail = (buf->tail+1)%MAX_DATA_BUFSZ;
        buf->size++;
    }  else {
        return -1;    
    }

    pthread_mutex_unlock(&buf->lock);
    return 0;
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

    collect = true;
    
    int data,retval;
    
    //Threadpool for data 
    threadpool t;
    
    //connect to switch
    while(collect == true) {
        //Do SNMP get 
        if ( doSnmpGet("CISCO-PROCESS-MIB::cpmCPUTotal1minRev.1", switch_ip.c_str(), &data) == SUCCESS) {
            retval = bufferAdd(&buf[CPU_RATE], data);
        } else {
            retval = bufferAdd(&buf[CPU_RATE], rand()%100);
        }
        
        if(retval == -1) {
            cout<<"Buffer MAXLIMIT reached Dispatching to clints "<<endl;
            int *tmp_buf = buf[CPU_RATE]->data;

            buf[CPU_RATE]->data = (int *)malloc(sizeof(int)*MAX_DATA_BUFSZ);
            ///Defination in server_dispatch 
            t.submit(dispatch_routine_cpu_rate, (void *) tmp_buf);
        }


        if(doSnmpGet("CISCO-PROCESS-MIB::cpmCPUMemoryUsed.1", switch_ip.c_str(), &data) == SUCCESS) {
            retval = bufferAdd(&buf[CPU_MEM_USAGE], data);
        }
        else {
            retval = bufferAdd(&buf[CPU_MEM_USAGE], rand()%100000);
        }

        if(retval == -1) {
            cout<<"Buffer MAXLIMIT reached Dispatching to clints "<<endl;
            int *tmp_buf = buf[CPU_MEM_USAGE]->data;

            buf[CPU_MEM_USAGE]->data = (int *)malloc(sizeof(int)*MAX_DATA_BUFSZ);
            ///Defination in server_dispatch 
            t.submit(dispatch_routine_cpu_usage,(void *)tmp_buf);
        }
    }
}


void dispatch_routine_cpu_usage( void *arg) {
{
    get_data((int *) arg, MAX_DATA_BUFSZ,CPU_MEM_USAGE);
}
void dispatch_routine_cpu_rate(void *arg) {
    get_data((int *) arg, MAX_DATA_BUFSZ,CPU_RATE);
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
