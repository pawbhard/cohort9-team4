#define MAX_Tx_Rx_BUFFER 65534
#define SUCCESS 1
#define SYSERR  0
#define SERVER_PORT 8888

typedef uint32_t syserr_t;
struct g_conn_hndl_t
{
  uint32_t local_fd;
  struct sockaddr_in server_addr;
};
struct g_conn_hndl_t local_con_hndlr;


