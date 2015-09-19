#include<iostream>
#include<stdio.h>
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
#include<time.h>

#include"common_utils.h"
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
    int opt=1 ;
    if(setsockopt(get_server_sock_fd(),SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt))){
       cout << "Set socket failed";
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

void incr_client_cnt()
{
   client_id =client_id +1;
}



  
syserr_t
client_server_handshake_init(struct client_sock_fd_t * client_sock_hndlr) 
{
   time_t handshake_time;
   handshake_time=time(NULL);
   client_server_msg_t cs_handshake_msg; 
   memset(&cs_handshake_msg,0,sizeof(cs_handshake_msg));
   char stdout_msg[MAX_Tx_Rx_BUFFER]; 
   
   cs_handshake_msg.cs_msg_type=MSG_TYPE_HANDSHAKE;
   strcpy(cs_handshake_msg.cs_payload,"HELLO");
   
   write(client_sock_hndlr->client_sock,&cs_handshake_msg,sizeof(client_server_msg_t));
 
   memset(&cs_handshake_msg,0,sizeof(cs_handshake_msg));
   read(client_sock_hndlr->client_sock,&cs_handshake_msg,sizeof(client_server_msg_t));
 
   if(cs_handshake_msg.cs_msg_type==MSG_TYPE_HANDSHAKE){
   sprintf(stdout_msg,"\n%s:%s:Hanshake Success\n",ctime(&handshake_time),client_sock_hndlr->client_ip);
   puts(stdout_msg);
   
   return SUCCESS;
   }
   sprintf(stdout_msg,"\n%s:%s:Hanshake Failure\n",ctime(&handshake_time),client_sock_hndlr->client_ip);
   puts(stdout_msg);

   return SYSERR;
 
}


void *register_client(  void *client_desc)
{
    struct client_sock_fd_t *client_hndlr =(struct client_sock_fd_t *)client_desc;
    struct client_info_t client_info;
    time_t client_join_leave_time; 
    client_join_leave_time=time(NULL); 
    char stdout_msg[MAX_Tx_Rx_BUFFER];

    int sock =client_hndlr->client_sock;
    char message[MAX_Tx_Rx_BUFFER];
    uint32_t client_group;
    uint32_t client_choice;
    
    //Populate client details.// 
    client_info.client_id=client_id;
    strncpy(client_info.client_ip,client_hndlr->client_ip,sizeof(client_info.client_ip));
    
    while(1) {
    //Start Client Join Leave Process
    client_server_msg_t client_req_msg;
    
    memset(&client_req_msg,0,sizeof(client_req_msg));

    read(sock,&client_req_msg,sizeof(client_server_msg_t));
    client_choice=client_req_msg.cs_msg_type;
    switch(client_choice) 
    {
    case MSG_TYPE_MGROUP_ADVT_REQ:
         sprintf(stdout_msg,"\n%s:%s:Received Mgroup  Advert Request Msg\n",ctime(&client_join_leave_time),client_info.client_ip); 
         puts(stdout_msg);
  
         memset(&client_req_msg,0,sizeof(client_server_msg_t)); 
         strcpy(client_req_msg.cs_payload,"1:Mean\n2:Median\n3:Mode"); 
         client_req_msg.cs_msg_type =MSG_TYPE_MGROUP_ADVT_RESP;
         write(sock,&client_req_msg,sizeof(client_server_msg_t));
         
         memset(stdout_msg,0,sizeof(stdout_msg)); 
         sprintf(stdout_msg,"\n%s:%s:Sent Mgroup Info\n",ctime(&client_join_leave_time),client_info.client_ip);
         puts(stdout_msg);
         break;

    case MSG_TYPE_MGROUP_JOIN_REQ:
         memset(stdout_msg,0,sizeof(stdout_msg));
         sprintf(stdout_msg,"\n%s:%s:Received Group Join Reqest Msg\n",ctime(&client_join_leave_time),client_info.client_ip);
         puts(stdout_msg);
        
         client_info.multicast_group_id =atoi(client_req_msg.cs_payload);
         cout <<"\n Adding new Client to the group \n"<< client_info.multicast_group_id;
         cout <<"\nClient Information:\nClient_id:"<<client_info.client_id<<
         "\nClientIP:"<<client_info.client_ip<<
         "\nMulticast Group:"<<client_info.multicast_group_id;
  
         memset(&client_req_msg,0,sizeof(client_server_msg_t));
         sprintf(client_req_msg.cs_payload,"%d",client_info.multicast_group_id);
         client_req_msg.cs_msg_type=MSG_TYPE_MGROUP_JOIN_RESP;
         write(sock,&client_req_msg,sizeof(client_server_msg_t));

         memset(stdout_msg,0,sizeof(stdout_msg));
         sprintf(stdout_msg,"\n%s:%s:Successfully Registered to  Mgroup \n",ctime(&client_join_leave_time),client_info.client_ip);
         puts(stdout_msg);
         break;

     case MSG_TYPE_MGROUP_LEAVE_REQ:
         memset(stdout_msg,0,sizeof(stdout_msg));
         sprintf(stdout_msg,"\n%s:%s:Received Group Leave Reqest Msg\n",ctime(&client_join_leave_time),client_info.client_ip);
         puts(stdout_msg);
 

         client_info.multicast_group_id =atoi(client_req_msg.cs_payload);
         cout <<"\n Deleting   Client  from  group \n"<< client_info.multicast_group_id;
         cout <<"\nClient Information:\nClient_id:"<<client_info.client_id<<
         "\nClientIP:"<<client_info.client_ip<<
         "\nMulticast Group:"<<client_info.multicast_group_id;

         memset(&client_req_msg,0,sizeof(client_server_msg_t));
         sprintf(client_req_msg.cs_payload,"%d",client_info.multicast_group_id);
         client_req_msg.cs_msg_type=MSG_TYPE_MGROUP_LEAVE_RESP;
         write(sock,&client_req_msg,sizeof(client_server_msg_t));
   
         memset(stdout_msg,0,sizeof(stdout_msg));
         sprintf(stdout_msg,"\n%s:%s:Successfully unregistered from  Mgroup \n",ctime(&client_join_leave_time),client_info.client_ip);
         puts(stdout_msg);
         break;

        
    }
}
}
         
         
          
     
     
     
     
   


void * client_connection_hndl(void *)
{
    struct sockaddr_in  client_addr;
    int client_sock_hndl;
    int c,client_sock;
    c=sizeof(struct sockaddr_in);
    time_t cs_time;
    cs_time=time(NULL);

    /*Create server socket*/
    if(!server_socket_create()) {
	        cout <<"Failed to create socket";
        exit(1);
     }
    listen(get_server_sock_fd(),3);
    puts("Server is running");
     
    while(client_sock_hndl=
      accept(get_server_sock_fd(),
           (struct sockaddr *)&client_addr,(socklen_t*)&c
    )) {
         if(client_sock_hndl<0) {
            cout <<"Accept failed";
            exit(1);
         }
    printf("\n|%s:|Received a New Connection \n",ctime(&cs_time));
    /*Server Client Handshake*/

    struct client_sock_fd_t *client_fd=NULL;
    client_fd=(struct client_sock_fd_t *)malloc(sizeof(client_sock_fd_t));
    char address[INET6_ADDRSTRLEN];
 
    client_fd->client_sock = client_sock_hndl;
    const char *dst = inet_ntop(client_addr.sin_family,&(client_addr.sin_addr),
                    address,sizeof(address));
    if(dst==NULL) {
      cout << "Failed to get Client Ip";
      exit(1);
    }
    strncpy(client_fd->client_ip,address,sizeof(address));
    if(!client_server_handshake_init(client_fd)){
         std::cout <<ctime(&cs_time) << ":ClientIp:"<<address<<"-Handshake Failed";
         exit(SYSERR);
    }
    //Handshake Successful
    incr_client_cnt();
    pthread_t register_thread;
    
    if(pthread_create(&register_thread,NULL,register_client,(void *)client_fd)<0) {    
        cout <<"Failed to create pthread";
        exit(1);
    }
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
     
         



