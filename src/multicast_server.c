#include "../include/multicast_server.h"

//TODO : initialization of server
void init(void)
{
    //TO be implemented
    return;
}

void run_group_server(multicast_group* database)
{
    
    int socketfd ;
    
    //create socket for server
    if((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) <  0)
    {
        perror("Socket creation failed for group");
        exit(1);
    }
   
    //Enable reuse 
    int set_option_on = 1;
    int res = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (char*) &set_option_on, 
                sizeof(set_option_on));
    if(res < 0)
    {
        perror("Reuse option failed");
        close(socketfd);
        exit(1);
    }

    //Bind to local address
    struct sockaddr_in local_addr_in;
    local_addr_in.sin_family        = AF_INET;
    local_addr_in.sin_port          = htons(SERVER_PORT);
    local_addr_in.sin_addr.s_addr   = htons(INADDR_ANY);
    if(bind(socketfd,(struct sockaddr *) &local_addr_in, sizeof(local_addr_in)))
    {
        perror("Binding failed");
        close(socketfd);
        exit(1);
    }

    //Disable all multicast receiveing 
    int mc_all = 0;
    if ((setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_ALL, 
            (void*) &mc_all, sizeof(mc_all))) < 0) 
    {
        perror("setsockopt() failed");
        close(socketfd);
        exit(1);
    }

    struct in_addr local_intf;
    local_intf.s_addr = INADDR_ANY; //ANY interface
    if(setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_IF, (char *) &local_intf,
            sizeof(local_intf)) < 0)
    {
        perror("Setting of local interface failed");
        close(socketfd);
        exit(1);
    }

    //disable loop due to multicast
    char loop = 0;
    if(setsockopt(socketfd, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&loop, sizeof(loop)) < 0)
    {
        perror("Failed to disable multicast loop ");
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

    int l = NUM_OF_GROUP;
    while(l--) {
        //subscribe to multicast groups 
        struct ip_mreq group;
        group.imr_multiaddr.s_addr = inet_addr(group_address[l]);
        group.imr_interface.s_addr = INADDR_ANY;
        if(setsockopt(socketfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0)
        {
            perror("Not able to add membership");
            close(socketfd);
            exit(1);
        }
    }

    
    pthread_t tid; //thread to display messages every 20 sec
    unsigned int delay = 20; //TODO get from config file

    int ret =  pthread_create(&tid,  NULL ,(void *) *display_thread ,&delay);
    if(ret != 0 )
    {
        perror("thread creation failed");
        exit(1);
    }

    //receive all cient messages and display at regular interval
    receive_and_store_messages_server(socketfd, database);
    
    //TODO: other work related to server 

    close(socketfd);
}

void receive_and_store_messages_server(int socketfd, multicast_group* database)
{
    char buffer[1024]; //MAX length of message TODO : create macro
    int retval; 
    struct sockaddr_in client_ad, server_ad;
    ipv4_address group, client;

    while(1)
    {
        memset(buffer,0,sizeof(buffer)); //clear buffer
        retval = recv_from_to(socketfd, buffer, sizeof(buffer), 0,
                                (struct sockaddr *) &client_ad,
                                (struct sockaddr *) &server_ad,
                                sizeof(server_ad));
        (void)retval;
            
        printf("\nServer received message from client : %s",inet_ntoa(client_ad.sin_addr));
        printf("\nServer received message for group : %s",inet_ntoa(server_ad.sin_addr));
        printf("\nMessage : %s\n\n",buffer);
        

        // Adding message to storage queue
        //TODO : implement and remove below comment
        //add_message(client_ad.sin_addr , group_ad.sin_addr , buffer)


        group.address = inet_addr(inet_ntoa(server_ad.sin_addr));
        client.address = inet_addr(inet_ntoa(client_ad.sin_addr));
        /*
         * If the client is added for the second time, only the message gets appended
         */
        add_client_to_multicast_group (database, group, client, 0, buffer);
    }
}

void display_thread(void * arg)
{
    int delay  = * (int *) arg;
    while(1)
    {
        sleep(delay);
        //TODO Implement below function and remove comment
        //display_per_group();
        /*
         * Use delete_message_list() function to clear the messages
         * after display on each group
         */
    }
}

int  main(int argc, char * argv[])
{
    multicast_group* database = NULL;
    ipv4_address group;
    for (int i=0;i<NUM_OF_GROUP;i++) {
        group.address = inet_addr (group_address[i]);
        add_multicast_group (database, group);
    }
    init(); //TODO 
    run_group_server(database);
    return 0;    
    //Other Tasks
}
