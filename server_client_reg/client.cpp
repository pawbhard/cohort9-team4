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

#include "common_utils.h"
#include "client.h"



using namespace std;
uint32_t
get_local_sock_fd ()
{
  return(local_con_hndlr.local_fd);
}

syserr_t
set_server_addr(char *server_ip)
{
   local_con_hndlr.server_addr.sin_addr.s_addr = inet_addr(server_ip);
   local_con_hndlr.server_addr.sin_family      = AF_INET;
   local_con_hndlr.server_addr.sin_port        = htons(SERVER_PORT);
}


syserr_t
create_client_socket(char *server_ip)
{
   local_con_hndlr.local_fd=socket(AF_INET,SOCK_STREAM,0);
   cout << "Socket:" << get_local_sock_fd ();
   if(local_con_hndlr.local_fd<0) {
         cout << "create socket failed";
         return SYSERR;
   }
   return SUCCESS;
}



syserr_t
server_handshake()
{
  time_t server_hndshk_time;
  server_hndshk_time = time(NULL);
  client_server_msg_t cs_hello_msg;
  char stdout_msg[MAX_Tx_Rx_BUFFER];
  
  memset(&cs_hello_msg,0,sizeof(cs_hello_msg));
  read(get_local_sock_fd(),&cs_hello_msg,sizeof(client_server_msg_t));
  if(cs_hello_msg.cs_msg_type!=MSG_TYPE_HANDSHAKE) {
     sprintf(stdout_msg,"\n%s:Server Handshake Failed\n",ctime(&server_hndshk_time));
     puts(stdout_msg);
     return SYSERR;
  }
  memset(stdout_msg,0,sizeof(stdout_msg));
  sprintf(stdout_msg,"\n%s:Received Pkt:%s\n",ctime(&server_hndshk_time),cs_hello_msg.cs_payload);
  puts(stdout_msg);
  
   
  memset(stdout_msg,0,sizeof(stdout_msg));
  sprintf(stdout_msg,"\n%s:Sending Hello Message\n",ctime(&server_hndshk_time));
  puts(stdout_msg);

  memset(&cs_hello_msg,0,sizeof(cs_hello_msg));
  cs_hello_msg.cs_msg_type=MSG_TYPE_HANDSHAKE;
  strcpy(cs_hello_msg.cs_payload,"HELLO");
  write(get_local_sock_fd(),&cs_hello_msg,sizeof(cs_hello_msg));

  return SUCCESS;
} 

void 
client_join_leave_mgroup_hndlr() 
{
   time_t join_leave_time;
   join_leave_time=time(NULL);
   uint32_t admin_choice;
   client_server_msg_t client_req_msg; 
   char stdout_msg[MAX_Tx_Rx_BUFFER];
   while(1) {
     std::cout << "1:Server Mgroups Advert Request\n2:Join Mgroup\n3:Leave Mgroup";
     cin >> admin_choice;
     switch(admin_choice) 
     {
     case ADVERT_REQ:
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Sending Server Groups Advert Request\n",ctime(&join_leave_time));
          puts(stdout_msg);

          memset(&client_req_msg,0,sizeof(client_req_msg)); 
          client_req_msg.cs_msg_type=MSG_TYPE_MGROUP_ADVT_REQ;
          strcpy(client_req_msg.cs_payload,"Advertise the Mgroups");
          write(get_local_sock_fd(),&client_req_msg,sizeof(client_req_msg));
           
          memset(&client_req_msg,0,sizeof(client_req_msg)); 
          read(get_local_sock_fd(),&client_req_msg,sizeof(client_server_msg_t));
          if(client_req_msg.cs_msg_type!=MSG_TYPE_MGROUP_ADVT_RESP) {
          cout<<ctime(&join_leave_time)<<":Failed to get Server Group Info"; 
          /*
            memset(stdout_msg,0,sizeof(stdout_msg));
            sprintf(stdout_msg,"\n%s:Failed To Received Mgroup Info\n",ctime(&join_leave_time));
            puts(stdout_msg);
          */
            goto done; 
          }
          /* 
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%sReceived Server Mgroup Info\n%s \n",ctime(&join_leave_time),client_req_msg.cs_payload);
          puts(stdout_msg);
          */
          cout<<client_req_msg.cs_payload<<"Success";
          break;
      case MGROUP_JOIN_REQ:
          char join_group_list[10];
          cout << "Enter the List of Groups The Client has to join."; 
          cin >> join_group_list; 
          
          /* 
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Sending Mgroup Join Request for %s \n",ctime(&join_leave_time),join_group_list);
          puts(stdout_msg);
          */
          cout<<ctime(&join_leave_time)<<":Sending Mgroup Join Request for"<<join_group_list;
 
          memset(&client_req_msg,0,sizeof(client_req_msg));
          client_req_msg.cs_msg_type=MSG_TYPE_MGROUP_JOIN_REQ;
          strcpy(client_req_msg.cs_payload,join_group_list);
          write(get_local_sock_fd(),&client_req_msg,sizeof(client_req_msg));
 
          memset(&client_req_msg,0,sizeof(client_req_msg));
          read(get_local_sock_fd(),&client_req_msg,sizeof(client_server_msg_t));

          if(client_req_msg.cs_msg_type!=MSG_TYPE_MGROUP_JOIN_RESP) {
          cout<<ctime(&join_leave_time)<<":Registration Failed. for"<<join_group_list;
          /*
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Failed To Join Mgroup \n%s \n",ctime(&join_leave_time),client_req_msg.cs_payload);
          puts(stdout_msg); 
          */
          goto done;
          }
          /*
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Successfull Registered to the server mgroup \n%s \n",ctime(&join_leave_time),client_req_msg.cs_payload);
          puts(stdout_msg);
          */
          cout<<ctime(&join_leave_time)<<":SuccessFully Registerd to the server mgroup"<<client_req_msg.cs_payload;
          break;
      case MGROUP_LEAVE_REQ:
          char leave_group_list[10];
          cout << "Enter the List of Groups The Client has to leave.";
          cin >> leave_group_list;
          /*
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Sending Mgroup Leave Request for %s \n",ctime(&join_leave_time),leave_group_list);
          puts(stdout_msg);
          */
          cout<<ctime(&join_leave_time)<<":Sending Mgroup Join Request for"<<leave_group_list;
 
          memset(&client_req_msg,0,sizeof(client_req_msg));
          client_req_msg.cs_msg_type=MSG_TYPE_MGROUP_LEAVE_REQ;
          strcpy(client_req_msg.cs_payload,leave_group_list);
          write(get_local_sock_fd(),&client_req_msg,sizeof(client_req_msg));

          memset(&client_req_msg,0,sizeof(client_req_msg));
          read(get_local_sock_fd(),&client_req_msg,sizeof(client_server_msg_t));
          if(client_req_msg.cs_msg_type!=MSG_TYPE_MGROUP_LEAVE_RESP) {
             /*
             memset(stdout_msg,0,sizeof(stdout_msg));
             sprintf(stdout_msg,"\n%s:Failed To Unregister to Mgroup \n%s \n",ctime(&join_leave_time),client_req_msg.cs_payload);
             puts(stdout_msg); 
            */
            cout<<ctime(&join_leave_time)<<": Failed.to unregister from "<<leave_group_list;
             
             goto done;
          }
          /*
          memset(stdout_msg,0,sizeof(stdout_msg));
          sprintf(stdout_msg,"\n%s:Successfull unregistered from the server mgroup \n%s \n",ctime(&join_leave_time),client_req_msg.cs_payload);
          puts(stdout_msg);
          */ 
           cout<<ctime(&join_leave_time)<<":SuccessFully unregisterd from the server mgroup"<<client_req_msg
.cs_payload;
          break;
          

      done:
        puts("Retry Again");
    } 
       }
}
     
          
          









        
         
      
 
  

syserr_t 
server_connection_hndlr(char *server_ip) 
{
   char group_id[10];   
   char message[MAX_Tx_Rx_BUFFER];
   uint32_t sock;

   if(create_client_socket(server_ip)<0) {
       cout << "Failed to Create client Socket\n";
       return SYSERR;
    }
    printf("Client_Socket:%d",get_local_sock_fd());

   if(!set_server_addr(server_ip)){
         cout <<"Failed to Set the Local address";
         return SYSERR;
   }
   
   sock=get_local_sock_fd();

   if(connect(sock,
        (struct sockaddr *)&local_con_hndlr.server_addr,
            sizeof(struct sockaddr_in))<0) {
     cout <<"Failed to create connection";
     return SYSERR;
   }
   
   if(!server_handshake()) {
      cout<<"Handshake failed with the server";
      exit(1);
   }
   
   client_join_leave_mgroup_hndlr();
       
}
   

   
 
int main(int argc , char *argv[])
{
   /*Register to the Client and Join the Multicast Group*/
   if(argc<2) {
       cout<<"\nPlease enter the ServerIp\n";
       return SYSERR;
   }
   server_connection_hndlr(argv[1]);
}




   


