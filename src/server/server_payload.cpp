#include "server_payload.h"


int send_packet_payload(int socketfd, int opcode,char *buffer,int buffer_len,int client_id)
{
    iovec iov[2];
    msghdr msg;
    union { 
        char cmsg[CMSG_SPACE(sizeof(struct in_pktinfo))];
    }u;

     cmsghdr *cmsgptr;
    //see man pages 
    char opcode_buf[4];
    snprintf(opcode_buf,4,"%x",opcode);
    iov[0].iov_base = opcode_buf;
    iov[0].iov_len  = sizeof(opcode_buf);
    iov[1].iov_base = buffer;
    iov[1].iov_len  = buffer_len;

    memset(&u, 0, sizeof(u));
    memset(&msg, 0, sizeof(msg));
    
    //get clent ip from database 
    DB *db = DB::get_instance();
//    in_addr_t client_ip = db.get_client_id_from_ip(client_id);
    in_addr_t client_ip; 
    sockaddr_in client_addr;
    memset(&client_addr,0,sizeof(client_addr));
    client_addr.sin_family      = AF_INET;
    client_addr.sin_addr.s_addr = client_ip;
    client_addr.sin_port        = htons(CLIENT_PORT); //should be fix for clients
    
    msg.msg_name = (void *)(sockaddr *)&client_addr;
    msg.msg_namelen = sizeof(client_addr);
    msg.msg_iov = iov;
    msg.msg_iovlen = 2;
    msg.msg_control = &u;
    msg.msg_controllen = sizeof(u);
    msg.msg_flags = 0 ; //flags for now is 0
    cmsgptr = CMSG_FIRSTHDR(&msg);
    msg.msg_controllen = cmsgptr->cmsg_len;

    int retval = sendmsg(socketfd,&msg,0); //flags are 0
    return retval;
}

int convert_int_hex_buffer(int *buffer_int, const int size_int, char *buffer_char,int size_char)
{
    if(size_char < size_int*8) {
        SERVER_ERROR("Insufficient character buffer length ");
        return FAILURE;
    }
    int i;
    char temp[9];
    for(i = 0; i < size_int; i++) {
        memset(temp,0,sizeof(temp));
        snprintf(temp,9,"%08X\n",buffer_int[i]);
        strncat(buffer_char,temp,size_char);
    }

    return SUCCESS;
}
