typedef enum multicast_group_ {
    MEAN,
    MEDIAN,
    MODE
} multicast_group_t;

struct g_server_sock_t 
{
    uint32_t server_sock;
    struct sockaddr_in server_addr;
};
    
struct client_info_t 
{
    char client_ip[INET6_ADDRSTRLEN];
    uint32_t client_id;
    uint32_t multicast_group_id;
};

struct client_sock_fd_t
{
    uint32_t client_sock;
    char     client_ip[INET6_ADDRSTRLEN];
};

struct client_sock_fd_t g_client_sock_info[1000];
uint32_t client_id;


struct g_server_sock_t server_sock_hndl;
void *register_client(struct client_sock_fd_t *);





