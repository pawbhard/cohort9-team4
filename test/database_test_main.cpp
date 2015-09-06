#include "../database/database_def.h"


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

    d.display_all();

    d.remove_client_group(700,7);
    d.remove_client_group(100,1);
    d.remove_client_group(100,4);
    
    d.display_all();

    d.add_client_group(800,3);
    d.add_client_group(800,1);
    d.add_client_group(800,2);

    d.display_all();

    d.remove_client(800);

    d.display_all();
    return 0;
}

