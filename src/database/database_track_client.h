#include "database_def.h"

typedef struct {
    int start;
    int end;
} position;

class track_data {
    private :
        unordered_map <int, position> track;
        track_data()
        {
            DATABASE_DEBUG("Created Object of track_data");
        }

        static track_data *td;
        track_data ( track_data const& ) {};
        track_data& operator = ( track_data const& ) {};

    public :
        static track_data* get_instance()
        {
            if (!td) td = new track_data;
            return td;
        }   
        int set_track(int client_id, position p);
        int get_track(int client_id , position *p);
};
