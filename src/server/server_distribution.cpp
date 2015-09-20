#include "server_header.h"
#include "../common_util/timer.h"

unsigned int task_id = 1;
using namespace std;

int get_data (int *data, size_t size, int type)
{

    int grp_id;
    int ret_val = SUCCESS;
    set <int> free_client_list;
    DB *db = DB::get_instance();
    track_data *td = track_data::get_instance();

    for (int i = 0; i <= NUM_OF_TASKS; i++)
    {
        switch (type)
        {
/*           case TEMP_DATA :
           grp_id =  get_group_id (type, i);
           free_client_list = db->get_free_client_list(grp_id);    
           break;
*/
           case CPU_MEM_USAGE :
           grp_id =  get_group_id (type, i);
           free_client_list = db->get_free_client_list(grp_id);
           break;

           case CPU_RATE : 
           grp_id =  get_group_id (type, i);
           free_client_list = db->get_free_client_list(grp_id);
           break;

           default :
           cout << "Invalid type\n";
           return FAILURE;           
        }
    
	if (free_client_list.size() == 0)
        {
           SERVER_DEBUG ("No free clients, ignoring data");
           free (data);
           return FAILURE;
        }   
 
//	static int task
        int size_data, put;
        int *arr;
        int cap = size;
        int per_client = ceil((float)cap/(float)free_client_list.size());
        set<int>::iterator it;
        for(it = free_client_list.begin(); it != free_client_list.end(); ++it)
        {
            if (cap <= 0)
                break;

            size_data = cap > per_client ? per_client : cap;
            arr = new int[size_data + 3];
            arr[2] = size_data;
            arr[0] = task_id;
            arr[1] = i;
            memcpy(arr+3, data+(size - cap), sizeof(int) * size_data);
            
            // calling back_up function
            back_up_add_delete(task_id, arr, 1);
        
            put = write(*it, (void*)arr, sizeof(arr));
            SERVER_DEBUG("Sending %d to client %d", put, *it );

            td->set_track(task_id, *it, (size - cap), ((size - cap) + size_data));          
            timer *t1;
            t1 = new timer(task_id, 100, handle_timer);
            t1->start();
        }
    }
    // TODO : Call timer function here
 
    return SUCCESS;
}

int get_group_id (int type, int task)
{
    if (type == CPU_MEM_USAGE)
    {
        if (task == MEAN)
           return 1;
        else if (task == DEVIATION)
           return 2;
        else if (task == RANGE)
           return 3;
    }
    else if (type == CPU_RATE)
    {
        if (task == MEAN)
           return 4;
        else if (task == DEVIATION)
           return 5;
        else if (task == RANGE)
           return 6;
    }
}

#if 0
int server_data_process (char *buffer, set <int> free_client_list , int start, int end, int grp_id, int task)
{
    int free_clients = 0, rem = 0, client_data_size = 0, data_size = 0;
    int i = 0;
    int bk_start = 0; 
    int bk_end = 0;
    int ret_val = SUCCESS;
    int opcode;
    int rmd;

    DB *db = DB::get_instance();

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

       db->set_client_busy_free(client_id, false);
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

   track_data *td = track_data::get_instance(); 
   ret_val = td->set_track(client_id, *p);
   
}
#endif

struct back_up {
      int id;
      int *arr;
 };

int back_up_add_delete (int task_id, int *arr, int flag) //flag 0 del 1 add
{
	static vector <back_up> bk;
        if (flag)
        {
		back_up b;
		b.id = task_id; b.arr = arr;
      		bk.push_back(b);
        } 
	else
	{
                
		for (int it = 0; it <= bk.size(); it++)
		{
			if (bk[it].id == task_id) {
                                                      
                            arr = bk[it].arr;
                            bk.erase(it + bk.begin());
		         }  
		}
	}

}

void handle_timer(sigval s)
{
   track_data *td = track_data::get_instance();
   int client_id;
   int start;
   int end;
   int ret_val;
   int *arr;
   int put;

   SERVER_DEBUG ("Timer fired for task id : %d", s.sival_int);
   ret_val = td->get_track(int s.sival_int, int client_id, int start, int end);

   if (ret_val != SUCCESS)
   {
      back_up_add_delete(s.sival_int, arr, 0);   
   }
   else
   {
      back_up_add_delete(s.sival_int, arr, 0);
      put = write(client_id, (void*)arr, sizeof(arr));
      SERVER_DEBUG("Sending %d to client %d", put, client_id );
   }
   free (arr);
} 
