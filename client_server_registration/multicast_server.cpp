#include<iostream.h>
#include<string.h>    //strlen
#include<stdlib.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<pthread.h> //for threading , link with lpthread
#include<stdint.h>
#include<netinet/in.h>
#include<netdb.h>
#include<stdlib.h>

#include"multicast_server.h"
using namespace std;

uint32_t
get_server_sock_fd()
{ 
  return(server_sock_hndl.server_sock);
}
    

syserr_t 
set_server_addr()
{
    server_sock_hndl.server_addr.sin_family = AF_INET;
    server_sock_hndl.server_addr.sin_addr.s_addr = INADDR_ANY;
    server_sock_hndl.server_addr.sin_port = htons(SERVER_PORT);
    return SUCCESS;
}

syserr_t 
server_socket_create()
{
    server_sock_hndl.server_sock = socket(AF_INET,SOCK_STREAM,0);

    if(server_sock_hndl.server_sock==-1 ) {
        cout << "Failed to create socket";
        return SYSERR;
    }

    if(!set_server_addr()) {
        cout << "Failed to set server address";
        return SYSERR;
    }
    
    if((bind(get_server_sock_fd(),
          (struct sockaddr *)&server_sock_hndl.server_addr,
          sizeof(struct sockaddr_in)))<0) {
          cout <<"Failed to bind";
          return SYSERR;
     }
     return SUCCESS;
}

void *register_client(  void *client_desc)
{
    struct client_sock_fd_t *client_hndlr =(struct client_sock_fd_t *)client_desc;
    struct client_info_t client_info;

    int sock =client_hndlr->client_sock;
    int read_size;
    char message[MAX_Tx_Rx_BUFFER],client_message[MAX_Tx_Rx_BUFFER];
    uint32_t  client_group;

    client_info.client_id=client_id;
    strncpy(client_info.client_ip,client_hndlr->client_ip,sizeof(client_info.client_ip));
      //Start Communication
    /*Advertise the Groups to the Client*/
    bzero(message,sizeof(message));
    sprintf(message,"\nThe following are the components\n.MEAN(1):\nMedian(2):\nMode(3): \n");
    write(sock,message,sizeof(message));

    /*Register the Client for a Group based on the Client
    Input*/
    bzero(message,sizeof(message));
    read(sock,message,sizeof(message));
    client_group=atoi(message);
    client_info.multicast_group_id=client_group;
    cout <<"\n Adding new Client to the group \n"<< client_info.multicast_group_id;
    cout <<"\nClient Information:\nClient_id:"<<client_info.client_id<<
    "\nClientIP:"<<client_info.client_ip<<
    "\nMulticast Group:"<<client_info.multicast_group_id;

    /*Add the Client info to the server database*/
    /*Function to be Implemented*/
    //add_client_info(client_info);


    return NULL;

}


void * client_connection_hndl(void *)
{
    struct sockaddr_in  client_addr;
    int client_sock_hndl;
    int c,client_sock;
    c=sizeof(struct sockaddr_in);

    /*Create server socket*/
    if(!server_socket_create()) {
        cout <<"Failed to create socket";
        exit(1);
     }
    cout <<"Waiting for connections...";
    listen(get_server_sock_fd(),3);
     
    while(client_sock_hndl=
      accept(get_server_sock_fd(),
           (struct sockaddr *)&client_addr,(socklen_t*)&c
    )) {
         cout <<"Connection Accepted";
         if(client_sock_hndl<0) {
            cout <<"Accept failed";
            exit(1);
         }
        
    pthread_t register_thread;
    struct client_sock_fd_t  client_fd;
    char address[INET6_ADDRSTRLEN];
/*
    client_fd = (struct client_sock_fd_t*)
                 malloc(sizeof(struct client_sock_fd_t));
                 */
    client_id+=1;
    client_fd.client_sock = client_sock_hndl;

          
    const char *dst=inet_ntop(client_addr.sin_family,&(client_addr.sin_addr),
      address,sizeof(address));
    if(dst==NULL) {
        cout <<"Destination is NULL";
        exit(1);
      }
        
    strncpy(client_fd.client_ip,address,sizeof(address));
     
    if(pthread_create(&register_thread,NULL,register_client,(void *)&client_fd)<0) {
           cout <<"Failed to create pthread";
           exit(1);
     }
    pthread_join(register_thread,NULL);
       
    }

}
         
int main(int argc,char*argv[])
{
    /*thread to handle client connect and register process*/
    pthread_t con_hndl;
    if(pthread_create(&con_hndl,NULL,client_connection_hndl,NULL)<0) {
           cout <<"Failed to create pthread";
           return SYSERR;
       }
    pthread_join(con_hndl,NULL);

}
     
         



