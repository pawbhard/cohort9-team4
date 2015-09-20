#include<vector>
#include<unordered_map>
#include<set>
#include<cstdio>
#include<iostream>
#include <netinet/in.h>

#define DATABASE_ERROR(format,args...) printf("DATABASE ERROR %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)
#define DATABASE_DEBUG(format,args...) printf("DATABASE DEBUG %s(%d): " format "\n" ,__FUNCTION__,__LINE__,##args)

#define FAILURE 0
#define SUCCESS 1

using namespace std;

//Database class to provide only one instace of the object 
class DB 
{
    private : unordered_map <int, set<int>>  group_to_client;
              unordered_map <int, set<int>>  client_to_group;
              unordered_map <int, in_addr_t> client_ip_to_id;
              unordered_map <int, bool> client_state;       //true free , false busy
              set <int> group_set;

              DB()
              {
                  DATABASE_DEBUG("Created Object of database");
              }

              int add_new_client_state(int client_id);
    
    public  : static DB get_db_instance(void)
              {
                  static DB db;                  
                  return db;
              }
            
              //APIs (All are made int to return error )
              int add_group(int group_id);
              int add_client_group(int client_id , int group_id);
              set<int> get_clients_for_group(int group_id);
              set<int> get_groups_for_client(int client_id);
              int remove_client_group(int client_id , int group_id);
              int remove_client(int client_id);
              in_addr_t get_client_ip_from_id(int client_id);
              int set_client_id_from_ip(in_addr_t client_ip);

              // For client state 
              set<int> get_free_client_list(int group_id);
              int set_client_busy_free(int client_id,bool state);

              //DISPLAY for debugging
              void display_group();
              void display_client_to_group();
              void display_group_to_client();
              void display_all();
};

