#include "../src/database/database_def.h"


int main()
{
    DB d = DB::get_db_instance();
    int i;
    int group[] = { 1,2,3,4,5,6,7,2};
    int num_of_gp = 8;
    for(i =0; i < num_of_gp; i++)
    {
        d.add_group(group[i]);
    }

    d.add_client_group(100,1);
    d.add_client_group(100,2);
    d.add_client_group(300,1);
    d.add_client_group(400,4);
    d.add_client_group(500,5);
    d.add_client_group(600,2);
    d.add_client_group(700,7);
    d.add_client_group(500,3);
    d.add_client_group(200,7);
    d.add_client_group(800,7);
    d.add_client_group(900,7);
    d.display_all();

    d.remove_client_group(700,7);
    d.remove_client_group(100,1);
    d.remove_client_group(100,4);
    
    d.display_all();

    d.add_client_group(800,3);
    d.add_client_group(800,1);
    d.add_client_group(800,2);

    d.display_all();
    
    d.set_client_busy_free(800,false);
    
    set<int> free_clients = d.get_free_client_list(7);
    set<int>::iterator it;
    //display free clients
    cout<<"Displaying free clients for group 7 : ";
    for (it=free_clients.begin(); it!=free_clients.end(); ++it)
    {
        cout<<"  --> "<<*it;
    }
    cout<<"\n";

    d.remove_client(800);

    d.display_all();
    return 0;
}

