#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>
#include "../database/database_def.h"
#define CLIENT_PORT 4567

#define SERVER_ERROR(format,args...) printf("SERVER ERROR %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
#define SERVER_DEBUG(format,args...) printf("SERVER DEBUG %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
int convert_int_hex_buffer(int *buffer_int, const int size_int, char *buffer_char,int size_char);
int send_packet_payload(int socketfd, int opcode,char *buffer,int buffer_len,int client_id);
