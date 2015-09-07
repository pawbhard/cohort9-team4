#include "database_def.h"

/*
   43               void display_group();
   44               void display_client_to_group();
   45               void display_group_to_client();
   46               void display_all();
 */

using namespace std;
void DB::display_group()
{
    cout<<"\nDisplaying all groups present :\n";
    set<int>::iterator it;
    for (it = group_set.begin(); it != group_set.end(); ++it)
    {
        cout<<"  "<<*it;
    }
}

void DB::display_client_to_group()
{
    cout<<"\nDisplaying client to group\n";
    set<int>::iterator it;
    for (auto& x: client_to_group)
    {
        cout<<"Client id : "<<x.first<<". Groups :";
        for(it = x.second.begin() ; it != x.second.end(); ++it)
        {
            cout<<"  "<<*it;
        }
        cout<<"\n";
    }
}

void DB::display_group_to_client()
{
    cout<<"\nDisplaying group to client\n";
    set<int>::iterator it;
    for (auto& x: group_to_client)
    {
        cout<<"Group id : "<<x.first<<". Clients :";
        for(it = x.second.begin() ; it != x.second.end(); ++it)
        {
            cout<<"  "<<*it;
        }
        cout<<"\n";
    }
}

void DB::display_all()
{
    display_group();
    display_client_to_group();
    display_group_to_client();
}
