#include "server_header.h"

int get_result(in_addr_t client_ip, int task_id, int task, int num, double answer)
{
    position *p;
    DB *db = DB::get_instance();
    track_data *td = track_data::get_instance();
    Result *result = Result::get_instance();

    result->update_result (task, num, answer);

    int client_id = db->set_client_id_from_ip(client_ip);

//    td->get_track(client_id, p);
    //TODO : With the above p flush the buffer
}
