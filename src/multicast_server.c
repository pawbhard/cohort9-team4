#include "../include/multicast_server.h"

static multicast_group* find_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = head;
    while (cur) {
        if (compare_ipv4_address(cur->group_addr, group)) {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}

static int add_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = head;
    multicast_group* node = (multicast_group*)malloc(sizeof(multicast_group));
    if (!node) {
        return FALSE;
    }
    node->next = NULL;
    node->client_list = NULL;
    node->group_addr = group;
    if (!cur) {
        cur = node;
        return TRUE;
    }
    while (cur->next) {
        cur = cur->next;
    }
    cur->next = node;
    return TRUE;
}
/*
static int remove_client_list (client_node* head) {
    client_node* cur;
    client_node* node;
    if (!head) {
        return FALSE;
    }
    cur = head;
    while (cur) {
        node = cur;
        cur = cur->next;
        node->next = NULL;
        free (node);
    }
    return TRUE;
}

static int remove_multicast_group (multicast_group* head, ipv4_address group) {
    multicast_group* cur = find_multicast_group (head, group);
    multicast_group* prev = head;
    multicast_group* node;
    
    if (!cur) {
        return FALSE;
    }
    if (cur == head) {
        head = head->next;
        remove_client_list (cur->client_list);
        free (cur);
        return TRUE;        
    }
    while (prev->next != cur) {
        prev = prev->next;
    }
    node = cur;
    prev->next = cur->next;
    node->next = NULL;
    remove_client_list (node->client_list);
    free (node);
    return TRUE;
}
*/
static int add_client_to_multicast_group (multicast_group* head, ipv4_address group, ipv4_address client, int id, char* message) {
    multicast_group* cur_group = find_multicast_group (head, group);
    client_node* cur_client;
    client_node* node;
    client_node* check;
    if (!cur_group) {
        return FALSE;
    }
    cur_client = cur_group->client_list; 
    check = cur_client;
    while (check) {
        if (compare_ipv4_address(check->client_addr, client)) {
            return add_message_to_list (check->msg, message);
        }
        check = check->next;
    }
    
    node = (client_node*)malloc(sizeof(client_node));
    if (!node) {
        return FALSE;
    }
    node->client_addr = client;
    node->id = id;
    add_message_to_list (node->msg, message);
    node->next = NULL;

    if (!cur_client) {
        cur_client = node;
        return TRUE;
    }
    while (cur_client->next) {
        cur_client = cur_client->next;
    }
    cur_client->next = node;
    return TRUE;
}
/*
static int remove_client_from_multicast_group (multicast_group* head, ipv4_address group, int id) {
    multicast_group* cur_group = find_multicast_group (head, group);
    client_node* cur_client;
    client_node* prev_client;
    client_node* node;
    if (!cur_group) {
        return FALSE;
    }
    prev_client = cur_group->client_list;
    if (!prev_client) {
        return FALSE;
    }
    if (prev_client->id == id) {
        node = prev_client;
        prev_client = prev_client->next;
        node->next = NULL;
        free (node);
        return TRUE;
    }
    cur_client = prev_client->next;
    while (cur_client) {
        if (cur_client->id == id) {
            node = cur_client;
            prev_client->next = cur_client->next;
            node->next = NULL;
            free (node);
            return TRUE;
        }
        prev_client = cur_client;
        cur_client = cur_client->next;
    }
    return FALSE;
}
*/
//TODO : initialization of server
void init(void)
{
    //TO be implemented
    return;
}

void run_group_server(multicast_group* head)
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
    receive_and_store_messages_server(socketfd, head);
    
    //TODO: other work related to server 

    close(socketfd);
}

void receive_and_store_messages_server(int socketfd, multicast_group* head)
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

        /* 
         * group_ad is not declared anywhere. Using server_ad instead.
         */
        sscanf(inet_ntoa(server_ad.sin_addr), "%d.%d.%d.%d", 
                &group.a, &group.b, &group.c, &group.d);
        sscanf(inet_ntoa(client_ad.sin_addr), "%d.%d.%d.%d", 
                &client.a, &client.b, &client.c, &client.d);
        /*
         * If the client is added for the second time, only the message gets appended
         */
        add_client_to_multicast_group (head, group, client, 0, buffer);
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
    multicast_group* head = NULL;
    ipv4_address addr;
    for (int i=0;i<NUM_OF_GROUP;i++) {
        sscanf(group_address[i], "%d.%d.%d.%d", 
            &addr.a, &addr.b, &addr.c, &addr.d);
        add_multicast_group (head, addr);
    }
    init(); //TODO 
    run_group_server(head);
    return 0;    
    //Other Tasks
}
