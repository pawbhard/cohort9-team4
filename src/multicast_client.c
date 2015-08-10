#include "../include/multicast_client.h"

void send_message(int socketfd, in_addr_t group_address,char *buffer)
{
    //group address
    struct sockaddr_in group;
    memset(&group, 0, sizeof(group));
    group.sin_family       = AF_INET;
    group.sin_addr.s_addr  = group_address;
    group.sin_port         = htons(SERVER_PORT);

    struct sockaddr_in client;
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_addr.s_addr = INADDR_ANY;
    //send  message at regular interval
    while(1)
    {
        /*
        if(sendto(socketfd, buffer , strlen(buffer), 0, (struct sockaddr *) &group , sizeof(group)) < 0)
        {
            perror("Sending failed");
            return;
        }*/
        send_to_from(socketfd, buffer, strlen(buffer), 0, 
                (struct sockaddr *) &group,
                (struct sockaddr *) &client,
                sizeof(group));
        sleep(5);
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage ./client group_ip message\n");
        return 1;
    }

    int socketfd;
    char message[1024];
    memset(message,'\0',sizeof(message));
    strcpy(message,argv[2]);
    in_addr_t group_address  = inet_addr(argv[1]);  //TODO
    in_addr_t server_address = inet_addr("127.0.0.1");  //TODO

    //create datagram socket
    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) <0)
    {
        perror("Failed to create socket");
        exit(1);
    }
    
/*    struct sockaddr_in localSock;
    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(4321);
    localSock.sin_addr.s_addr = htons(INADDR_ANY);
    if(bind(socketfd, (struct sockaddr*)&localSock, sizeof(localSock)))
    {
        perror("Binding datagram socket error");
        close(socketfd);
        exit(1);
    }
  */   
    //multicast address 
    struct ip_mreq group;
    group.imr_multiaddr.s_addr = group_address;
    group.imr_interface.s_addr = server_address;

    if(setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
    {
        perror("Not able to join group");
        close(socketfd);
        exit(1);
    }
    
    //Set Packet INFO
    int setinfo = 1;
    if(setsockopt(socketfd, IPPROTO_IP, IP_PKTINFO, &setinfo, sizeof(setinfo)) < 0)
    {
        perror("Not able to set PKTINFO option");
        close(socketfd);
        exit(1);
    }

    //sending message
    send_message(socketfd,group_address,message);

    //Function of clients
}
