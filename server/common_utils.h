#define SUCCESS 1
#define SYSERR 0
#define MAX_MGROUP_COUNT 3

#define MAX_Tx_Rx_BUFFER 65534

#define SERVER_PORT 8888

#define MSG_TYPE_HANDSHAKE 1
#define MSG_TYPE_JOIN_GROUP  2 
#define MSG_TYPE_LEAVE_GROUP 3 
#define MSG_TYPE_MGROUP_ADVT_REQ 4
#define MSG_TYPE_MGROUP_ADVT_RESP 5
#define MSG_TYPE_MGROUP_JOIN_REQ 6
#define MSG_TYPE_MGROUP_JOIN_RESP 7
#define MSG_TYPE_MGROUP_LEAVE_REQ 8 
#define MSG_TYPE_MGROUP_LEAVE_RESP 9


#define ADVERT_REQ 1
#define MGROUP_JOIN_REQ 2
#define MGROUP_LEAVE_REQ 3


typedef uint32_t syserr_t;



typedef struct client_server_msg_ { 
uint32_t cs_msg_type;
char    cs_payload[MAX_Tx_Rx_BUFFER];
uint32_t   cs_comput_output[3];
}client_server_msg_t;
