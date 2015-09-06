#include <iostream.h>
#include<string.h>  //strlen
#include<sys/socket.h>  //socket
#include<arpa/inet.h>   //inet_addr
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
  
   /*Get Server Group Info*/
   bzero(message,MAX_Tx_Rx_BUFFER);
   cout<<"\nServer Multicast Group Info :\n";
   read(sock,message,sizeof(message));
   puts(message);
  
   /*Join one of the Groups in server*/
   bzero(message,MAX_Tx_Rx_BUFFER);
   cout << "\nEnter the Group for which the Client Has to Join\n";
   cin >> group_id;
   strncpy(message,group_id,sizeof(group_id));
   write(sock,message,sizeof(message));
   
   close(get_local_sock_fd());
   return 0;
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




   


