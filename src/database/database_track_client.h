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

    public  : static track_data get_db_instance(void)
              {
                  static track_data db;                  
                  return db;
              }
             int set_track(int client_id, position p);
             int get_track(int client_id , position *p);
};
