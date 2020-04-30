#include <iostream>
#include <vector>
#include <sys/time.h>
#include <mutex>
#include <pthread.h>
#include <thread>
#include "../Server.h"

using namespace std;

std::mutex add_mutex;
//pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void my_sleep(int req_delay)
{
  struct timeval start, check, end;
  double elapsed_seconds;
  gettimeofday(&start, NULL);
  do
  {
    gettimeofday(&check, NULL);
    elapsed_seconds = (check.tv_sec + (check.tv_usec / 1000000.0)) - (start.tv_sec + (start.tv_usec / 1000000.0));
  } while (elapsed_seconds < req_delay);
}

void f(vector<int> *vec, int fd, int delay, int num)
{
  my_sleep(delay);
  std::lock_guard<std::mutex> lock(add_mutex);
  (*vec)[num] += delay;
  cout << "num_bucket: " << num << ", delay:" << (*vec)[num] << endl;
  stringstream ss;
  ss << (*vec)[num]<<" in "<<num;
  send(fd, ss.str().c_str(), strlen(ss.str().c_str()), 0);
}

int main(int argc, char *argv[])
{
  Server server;
  if(argc < 2){
    cout<<"Usage: ./per_request <number>"<<endl;
    return 1;
  }
  server.startListen();
  int num = atoi(argv[1]);
  vector<int> vec(num, 0);
  while (1)
  {
    vector<int> res = server.getPara();
    int delay = res[0];
    int num = res[1];
    int fd = server.getclient();
    std::thread thrd(f, &vec, fd, delay, num);
    //thrd.join();
    thrd.detach();
  }
  return 0;
}
