/*
         set<int> get_free_client_list(int group_id);
         int set_client_busy(int client_id);
*/

#include "database_def.h"

set<int> DB::get_free_client_list(int group_id)
{
    DATABASE_DEBUG("Getting list of free client for group_id");
    
    set<int> free_client;
    unordered_map <int, bool>::iterator it;
    set<int> client_group = get_clients_for_group(group_id);
    
    for(auto x : client_group)
    {    
        it = client_state.find(x);
        if(it == client_state.end())
        {
            DATABASE_ERROR("Client not found in client state database");
        }

        if(it->second == true) //true means free
        { 
           free_client.insert(it->first);
           set_client_busy_free(it->first, false);
        }
    }
    
    return free_client;
}

int DB::set_client_busy_free(int client_id,bool state)
{
    unordered_map<int, bool> ::iterator it;
    it = client_state.find(client_id);
    if(it == client_state.end())
    {
        DATABASE_ERROR("Client not found in client state database");
        return FAILURE;
    }
    it->second = state;
    DATABASE_DEBUG("Client(%d) state is changes as %s",client_id, state == true ? "true" : "false");
    return SUCCESS;
}

int DB::add_new_client_state(int client_id) //by default state is free
{
    DATABASE_DEBUG("Adding Client(%d) state as free",client_id); 
    client_state.insert( pair<int, bool> (client_id, true));
    return SUCCESS;
}

//int set_opcode_task(int task_id, int opcode);
//              int get_opcode(int task_id);

int DB::set_opcode_task(int task_id, int opcode)
{
   task_opcode.insert({task_id, opcode});
}

int DB::get_opcode(int task_id)
{
   unordered_map<int, int> ::iterator it = task_opcode.find(task_id);
   return it->second;
}

