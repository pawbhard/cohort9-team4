#ifndef thread_common_h
#define thread_common_h

#include <pthread.h>

class Thread {
    public:
        void start();
        void join();
        virtual ~Thread();

    private:
        pthread_t threadId;

        static void *startThread(void *arg);
        virtual void run() {}

};

#endif
