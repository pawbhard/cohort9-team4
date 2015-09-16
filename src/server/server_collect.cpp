#include <iostream.h>
using namespace std;

int main()
{
  int data_exists ,temperature_type_exists, cpu_type_exists;
  while(1) {
    sleep(5);
    data_exists = check_temp_buffer();
    if (data_exists) {
      buffer = read_data_from_temp_buffer(); // TO-DO : define buffer 
      map_buffer(buffer ,temperature_type_exists, ,temp_payload ,cpu_type_exists, cpu_payload);
      if (temperature_type_exists) {
            client = get_client_for_temp_avg();
            if (cient) {
              push_to_client(client,temp_payload);
            }
            client = get_client_for_temp_median();
            if (cient) {
              push_to_client(client,temp_payload);
            }
      }
      if (cpu_type_exists) {
            client = get_client_for_cpu_avg();
            if (cient) {
              push_to_client(client,cpu_payload);
            }
            client = get_client_for_cpu_median();
            if (cient) {
              push_to_client(client,cpu_payload);
            }
      }
    }
  }  
}
