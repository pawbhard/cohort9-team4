#define MAX_Tx_Rx_BUFFER 65534 
#define SERVER_PORT 8888
#define SUCCESS 1
#define SYSERR 0

typedef uint32_t syserr_t;

typedef enum multicast_group_ {
    MEAN,
    DEVIATION,
    RANGE
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


struct g_server_sock_t server_sock_hndl;
void *register_client(struct client_sock_fd_t *);
uint32_t client_id;





