
int get_result(in_addr_t client_ip, int task_id, int task, num, answer)
{
    DB db = DB::get_db_instance();
    int client_id = db.get_client_id_from_ip(client_ip);

    get_track(client_id, p);
    //TODO : With the above p flush the buffer
}
