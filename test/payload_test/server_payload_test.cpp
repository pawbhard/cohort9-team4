
#include "../../src/server/server_payload.h"
#include<iostream>

using namespace std;
int main()
{
    const int NUM_OF_ELE = 9;
    int ar_int[NUM_OF_ELE] = { 120, 450, 1,
                                3000 , 15, 700000,
                                10,800,16, };
    char buffer[73];
    memset(&buffer,0,73);
    convert_int_hex_buffer(ar_int, NUM_OF_ELE,
            buffer, sizeof(buffer));

    cout<<buffer<<"size is "<<strlen(buffer)<<"\n";
}
