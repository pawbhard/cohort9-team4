#include <cstdio>
#include <cmath>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/param.h>
//#include "../database/database_def.h"
#include "../database/database_track_client.h"
#define CLIENT_PORT 4567

#define SERVER_ERROR(format,args...) printf("SERVER ERROR %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
#define SERVER_DEBUG(format,args...) printf("SERVER DEBUG %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
int convert_int_hex_buffer(int *buffer_int, const int size_int, char *buffer_char,int size_char);
int send_packet_payload(int socketfd, int opcode,char *buffer,int buffer_len,int client_id);


int get_result(in_addr_t, int, int, int, double);

//#define TEMP_DATA 0
#define CPU_MEM_USAGE 1
#define CPU_RATE 0

 enum {
    MEAN = 0,
    DEVIATION = 1,
    RANGE = 2,

    NUM_OF_TASKS = 3,};

int get_data (int *, int, int );
int get_group_id (int , int );
//int server_data_process (char *, set <int> , int , int , int , int );
int back_up_add_delete(int , int *, int );
void handle_timer(sigval);
//#define MEAN 0

class Result
{
   int mean_final;
   int mean_elements;
   int min_final;
   int max_final;

   Result() { mean_final = 0; mean_elements = 0; min_final = 0; max_final = 0;}
   static Result *result;
   Result ( Result const& ) {};
   Result& operator = ( Result const& ) {};

   public :
   static Result* get_instance ()
   { 
       if (!result) result = new Result;
       return result;
   }

   void update_result (int task_id, int num1, int num2)
   {
       DB *db = DB::get_instance();
       int task = db->get_opcode(task_id);
       switch (task)
       {
          case MEAN :
             update_mean (num1, num2);
             break;

          case RANGE :
             update_range (num1, num2);
             break;

          default : ; // some error handling
       }
   }

   void update_mean (int num1, int num2);
   void update_range (int num1, int num2);
   int get_mean () {return mean_final;}
   int get_mean_elements () {return mean_elements;}
};
