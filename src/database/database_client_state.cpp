/*
         set<int> get_free_client_list(int group_id);
         int set_client_busy(int client_id);
*/

#include "database_def.h"

set<int> DB::get_free_client_list(int group_id)
{
    DATABASE_DEBUG("Getting list of free client for group_id");
    
    set<int> free_client;
    set <int>::iterator it;
    set<int> client_group = get_clients_for_group(group_id);
    
    for(auto x : client_group)
    {   
        it = client_state.find(x);
        if(it == client_state.end())
        {
            DATABASE_ERROR("Client not found in client state database");
        }

        if(it->second == true) //true means free
            free_client.inset(it->first);
    }
    
    return free_client;
}

int DB::set_client_busy_free(int client_id,bool state)
{
    set<int> ::iterator it;
    it = client_state.find(client_id);
    if(it == client_state.end())
    {
        DATABASE_ERROR("Client not found in client state database");
        return FAILURE;
    }
    it->second = state;
    return SUCCESS;
}

int add_new_client_state(int client_id) //by default state is free
{
    client_state.insert( pair<int, bool> (client_id, true);
    return SUCCESS;
}
