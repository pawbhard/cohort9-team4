//File to define all the functions

/*
   24               //APIs (All are made int to return error )
   25               int add_group(int group_id);
   26               int add_client_group(int client_id , int group_id);
   27               set<int> get_client_for_group(int group_id);
   28               set<int> get_groups_for_client(int client_id);
   29               int remove_client_group(int client_id , int group_id);
   30               int remove_client(int client_id);
   31               int get_client_id_from_ip(int client_id);
   32               int set_client_id_from_ip(struct inet_addr);

*/
#include"database_def.h"

using namespace std;
int DB::add_group(int group_id)
{
    DATABASE_DEBUG("Adding group %d",group_id);
    if(group_set.find(group_id) != group_set.end())
    {
        DATABASE_ERROR("Group Already exists ");
        return FAILURE;
    }

    group_set.insert(group_id);

    return SUCCESS;
}

int DB::add_client_group(int client_id, int group_id)
{
    DATABASE_DEBUG("Adding client(%d) and group(%d) pair",client_id,group_id);
    if(group_set.find(group_id) == group_set.end())
    {
        DATABASE_ERROR("Group not present ");
        return FAILURE;
    }
    
    unordered_map<int, set<int>>::iterator it = group_to_client.find(group_id);
    if(it == group_to_client.end())
    {
        DATABASE_DEBUG("No previous mapping found . creating new");
        set<int> client;
        client.insert(client_id);
        group_to_client.insert( {group_id, client});
    }
    else    // some mappings are present already 
    {
        DATABASE_DEBUG("Previous mapping found Appending value");
        it->second.insert(client_id); // as it is set duplicates will be removed accordingly
    }

    it = client_to_group.find(client_id);
    if(it == client_to_group.end())
    {
        DATABASE_DEBUG("No previous mapping found . creating new");
        set<int> group;
        group.insert(group_id);
        client_to_group.insert( {client_id, group});
    }
    else    // some mappings are present already 
    {
        DATABASE_DEBUG("Previous mapping found Appending value");
        it->second.insert(group_id); // as it is set duplicates will be removed accordingly
    }
    
    DATABASE_DEBUG("Creating new Client state mapping");
    add_new_client_state(client_id);

    return SUCCESS;
}


int DB::remove_client_group(int client_id , int group_id)
{
    DATABASE_DEBUG("Removing client(%d) and group(%d) pair",client_id,group_id);
    if(group_set.find(group_id) == group_set.end())
    {
        DATABASE_ERROR("Group not present ");
        return FAILURE;
    }
    
    //Delete from group_to_client    
    unordered_map<int, set<int>>::iterator it = group_to_client.find(group_id);
    if(it == group_to_client.end())
    {
        //no mapping present for this group
        DATABASE_ERROR("Client(%d) is not part of group(%d)",client_id, group_id);
        return FAILURE;
    }

    //delete client id from set associated
    it->second.erase(client_id);
    
    //Delete from client_to_group    
    it = client_to_group.find(client_id);
    if(it == client_to_group.end())
    {
        //no mapping present for this group
        DATABASE_ERROR("Client(%d) is not part of group(%d)",client_id, group_id);
        return FAILURE;
    }

    //delete client id from set associated
    it->second.erase(group_id);
    
    return SUCCESS;
}

int DB::remove_client(int client_id)
{

    DATABASE_DEBUG("Removing client(%d)",client_id);
    
    //Delete from group_to_client  
    //traverse through map and delete client

    for (auto& x: group_to_client)
    {
        x.second.erase(client_id);
    }
    
    //Delete from client_to_group    
    client_to_group.erase(client_id);

    //DELETING FROM CLIENT_STATE database 
    client_state.erase(client_id);

    return SUCCESS;
}

set<int> DB::get_groups_for_client(int client_id)
{
    DATABASE_DEBUG("Getting all groups for client_id (%d)", client_id);
    unordered_map<int, set<int>>::iterator it = client_to_group.find(client_id);
    if(it == client_to_group.end())
    {
        DATABASE_ERROR("CAnnot find client id");
        set<int> s; //empty set 
        return s;
    }
    
    return it->second;
}

set<int> DB::get_clients_for_group(int group_id)
{
    DATABASE_DEBUG("Getting all clients for group_id (%d)", group_id);
    unordered_map<int, set<int>>::iterator it = group_to_client.find(group_id);
    if(it == group_to_client.end())
    {
        DATABASE_ERROR("CAnnot find client id");
        set<int> s;
        return s; ///empty set is returned for failure
    }
    return it->second;
}


