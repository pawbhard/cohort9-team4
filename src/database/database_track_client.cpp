
#include "database_track_client.h"

int DB::set_track(int client_id, position p) {
    track.insert(client_id,p)
    return SUCCESS;
}

int get_track(int client_id , position *p) {
    unordered_map<int, position>::iterator it = track.find(client_id);
    if(it == track.end()) {
        DATABASE_ERROR("Not found");
        return FAILURE;
    }
    *p = it->second;
    track.erase(it);
    return SUCCESS;
}
