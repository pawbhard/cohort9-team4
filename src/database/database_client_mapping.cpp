#if 0
#include"database_def.h"

/* Contains following apis 
 in_addr_t get_client_ip_from_id(int client_id);
int set_client_id_from_ip(in_addr_t client_ip);
*/

//gives a unique id for client corresponding to ip 
int DB::set_client_id_from_ip(in_addr_t client_ip) {
    static unsigned int id = 0;
    id +=1;

    //unordered_map <in_addr_t, int> client_ip_to_id;
    client_ip_to_id.insert(id,client_ip);

    return id;

}

//getip of client from ip
in_addr_t DB::get_client_ip_from_id(int client_id) {

    unordered_map<int, in_addr_t>::iterator it;
    it = client_ip_to_id.find(client_id);
    in_addr_t ip;
    if(it = client_ip_to_id.end()) {
        DATABASE_ERROR("Client id not present");
    }
    ip = ti.second;
    return ip;
}
#endif

