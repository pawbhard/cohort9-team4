
#include "database_track_client.h"

using std::make_pair;

int track_data::set_track(const int task_id, int client_id, int s, int e) {
    position p;
    p.client_id = client_id;
    p.start = s;
    p.end = e;
    track.insert({task_id,p});
    return SUCCESS;
}

int track_data::get_track(int task_id, int *client_id, int *s ,int *e) {
    unordered_map<int, position>::iterator it = track.find(task_id);
    if(it == track.end()) {
        DATABASE_DEBUG("Task done ");
        return FAILURE;
    }
    
    *client_id = it->second.client_id;
    *s = it->second.start;
    *e = it->second.end;
    track.erase(it);
    return SUCCESS;
}
