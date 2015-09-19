#include <iostream>
#include <list>

#include "../data_buffer/data_buffer.h"

using namespace std;

extern list <SwitchDataBuffer *>init_data_buffers();

void startServerDataCollection(list <SwitchDataBuffer *> switch_list) {
    int *cpu_data;
    int i;
    
    if( !switch_list.size() ) {
        cout << "Switch list size : "<<switch_list.size()<<endl;
        return;
    }

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
    startServer(switch_list);
}
