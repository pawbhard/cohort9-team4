#include <iostream>
#include <list>

#include "../data_buffer/data_buffer.h"

Result *Result::result = NULL;
DB *DB::db = NULL;
track_data *track_data::td = NULL;

using namespace std;

extern list <SwitchDataBuffer *>init_data_buffers();
void handle_result(uint32_t *arr) {
    if(arr == NULL) assert(0);
    SERVER_DEBUG("Received %d, %d, %d for updating result");
    Result *r;
    r = Result::get_instance();
    r->update_result(arr[0],arr[1],arr[2]);
}

void startServerDataCollection(list <SwitchDataBuffer *> &switch_list) {
    int *cpu_data;
    int i;
    
    cout << "Num Switches : "<<switch_list.size()<<endl;

    while(1) {
        for(list<SwitchDataBuffer *>::iterator it = switch_list.begin(); it != switch_list.end(); it++)
        {
            if( (*it)->bufLimitReached((*it)->getBuffer(CPU_RATE), STORE_LIMIT)) {
                cpu_data = (*it)->getData((*it)->getBuffer(CPU_RATE));

                if( cpu_data ) {
                    for( i=0; i<STORE_LIMIT; i++)
                        cout<<"Data get "<<cpu_data[i]<<endl;
                    free(cpu_data);
                }
            }

            if( (*it)->bufLimitReached((*it)->getBuffer(CPU_MEM_USAGE), STORE_LIMIT)) {
                cpu_data = (*it)->getData((*it)->getBuffer(CPU_MEM_USAGE));

                if( cpu_data ) {
                    for( i=0; i<STORE_LIMIT; i++)
                        cout<<"Data get "<<cpu_data[i]<<endl;
                    free(cpu_data);
                }
            }
        }
    }
}


int main()
{
    list <SwitchDataBuffer *>switch_list;
    switch_list = init_data_buffers();
    startServerDataCollection(switch_list);
}
