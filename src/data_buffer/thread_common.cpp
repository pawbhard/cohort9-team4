#include <iostream>
#include <stdlib.h>
#include <pthread.h>

#include "thread_common.h"

using namespace std;

void * Thread::startThread(void *arg) {
    Thread *t = static_cast<Thread *>(arg);
    t->run();
    return NULL;
}


void Thread::start() {

    int status;

    status = pthread_create(&threadId, NULL, Thread::startThread, (void*)this);

    if(status)
    {
        cout << "FATAL: Thread creation failed"<<endl;
        exit(status);
    }
}

void Thread::join() {
    int status = pthread_join(threadId, NULL);

    if (status)
    {
        cout << "FATAL: Thread join failed"<<endl;
        exit(status);
    }
}

Thread::~Thread() 
{ }

