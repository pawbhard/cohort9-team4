#include "server_distribution.h"

unsigned int task_id = 1;

int get_data (void)
{

    int grp_id;
    int data_size = 0;
    int ret_val = SUCCESS;
    set <int> free_client_list;
    DB db = DB::get_db_instance();

    for (int i = 0; i <= NUM_OF_TASKS; i++)
    {
        switch (type)
        {
           case TEMP_DATA :
           grp_id =  get_group_id (type, i);
           free_client_list = db.get_free_client_list(grp_id);    
           break;

           case CPU_DATA :
           grp_id =  get_group_id (type, i);
           free_client_list = db.get_free_client_list(grp_id);
           break;

           case PKT_RATE_DATA : 
           grp_id =  get_group_id (type, i);
           free_client_list = db.get_free_client_list(grp_id);
           break;

           default :
           cout << "Invalid type\n";
           return FAILURE;           
        }
       
        data_size = end - start;

        // Calling server_data_process to process data and sent to client
        ret_val = server_data_process (data, free_client_list, start, end, grp_id, i);      
        if (ret_val != SUCCESS)
        {
            cout << "Failed to process server data\n";
        }                
    }
    // TODO : Call timer function here
 
    return SUCCESS;
}

int get_group_id (int type, int task)
{
   
}

int server_data_process (char *buffer, set <int> free_client_list , int start, int end, int grp_id, int task)
{
    int free_clients = 0, rem = 0, client_data_size = 0, data_size = 0;
    int i = 0;
    int bk_start = 0; bk_end = 0;
    int ret_val = SUCCESS;
    int opcode;

    DB db == DB::get_db_instance();

    data_size = end - start;
    free_clients = free_client_list.size();
    rem = data_size % free_clients;
    client_data_size = (data_size - rem) / free_clients;

    opcode = (task_id * 10) + task;

    for (auto client_id : free_client_list)
    {
  
      if (i == free_clients)
       {
           client_data_size = client_data_size + rmd; 
       }

       bk_start = start;
       bk_end = start + client_data_size - 1;
       start = bk_end + 1;
       
       //Calling backup_info which will keep track of data sent to each client
       ret_val = backup_info (client_id, bk_start, bk_end);
       
       if (ret_val != SUCCESS)
       {
           cout << "Failed to backup the data\n";
           return FAILURE;
       }
    
       ret_val = send_packet_payload(fd, opcode, buffer+i*client_data_size, client_data_size, client_id);

       if (ret_val != SUCCESS)
       {
           cout << "send_packet_payload unsuccessful\n";
           return FAILURE;
       }

       db.set_client_busy_free(client_id, FALSE);
       i++;
       task_id++;
    }  
    return SUCCESS;
}

int backup_info (int client_id, int start, int end)
{
   int ret_val;
   position *p;

   p->start = start;
   p->end = end;

   track_data td = track_data::track_data_get_db_instance(); 
   ret_val = td.set_track(client_id, p);
   
}
