#include "threadpool.h"
#include <unistd.h>
#include <iostream>
#include "threadpool.cpp"
using namespace std;

const int MAX_TASKS = 0;

void hello1(void* arg)
{
  int* x = (int*) arg;
  cout << "Hello " << *x << endl;
//  sleep(10);
}

void hello2(void* arg)
{
      int* x = (int*) arg;
        cout << "Helloooo " << *x << endl;
}


int main(int argc, char* argv[])
{
  ThreadPool tp(2);
  int ret = tp.initialize_threadpool();
  if (ret == -1) {
    cerr << "Failed to initialize thread pool!" << endl;
    return 0;
  }

 /* for (int i = 0; i < MAX_TASKS; i++) {
    int* x = new int();
    *x = i+1;
    Task* t = new Task(&hello, (void*) x);
    cout << "Adding to pool, task " << i+1 << endl;
    tp.add_task(t);
    cout << "Added to pool, task " << i+1 << endl;
  }
*/
  int i = 0;
  int* x = new int();
  *x = i+1;
  Task* t = new Task(&hello1, (void*) x);
  cout << "Adding to pool, task " << i+1 << endl;
  tp.add_task(t);
  cout << "Added to pool, task " << i+1 << endl;

  i++;
  x = new int();
  *x = i+1;
  t = new Task(&hello2, (void*) x);
  cout << "Adding to pool, task " << i+1 << endl;
  tp.add_task(t);
  cout << "Added to pool, task " << i+1 << endl;


  sleep(2);

  tp.destroy_threadpool();

  cout << "Exiting app..." << endl;

  return 0;
}
