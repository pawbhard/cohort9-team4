#include "server_collect.h"
#include "server_distribution.h"

using namespace std;

unsigned int total_num = 0;
float updated_answer = 0;

int get_result(in_addr_t client_ip, int task_id, int task, int num, float answer)
{
    DB db = DB::get_db_instance();
    Result result = Result::get_result_instance();
    int ret_val;

    total_num = total_num + num;

    switch(task)
    {
        case AVERAGE :
        ret_val = result.update_avg(total_num, updated_answer, num, answer);
        break;

        case DEVIATION :
        ret_val = result.update_deviation(total_num, updated_answer, num, answer);
        break;

        default :
        cout << "Invalid task\n";
        return FAILURE;
    }

    updated_answer = updated_answer + ret_val;

    int client_id = db.get_client_id_from_ip(client_ip);

    get_track(client_id, p);
    //TODO : With the above p flush the buffer
}


