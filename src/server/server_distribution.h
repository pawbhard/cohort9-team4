

#define TEMP_DATA 0
#define CPU_DATA  1
#define PKT_RATE_DATA 2

typedef enum {
    AVERAGE = 0,
    DEVIATION = 1,

    NUM_OF_TASKS = 2,};

int get_data (char *, int , int , int );
int get_group_id (int , int );
int server_data_process (char *, set <int> , int , int , int , int );
int backup_info (int , int , int );
