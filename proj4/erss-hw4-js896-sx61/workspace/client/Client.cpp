#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <thread>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <string>
#include <mutex>

using namespace std;
std::mutex cout_mutex;


double calc_time(struct timespec start, struct timespec end)
{
  double start_sec =
      (double)start.tv_sec * 1000000000.0 + (double)start.tv_nsec;
  double end_sec = (double)end.tv_sec * 1000000000.0 + (double)end.tv_nsec;

  if (end_sec < start_sec)
  {
    return 0;
  }
  else
  {
    return end_sec - start_sec;
  }
};

vector<int> getRandom(int lower, int upper, int count)
{
  int i;
  vector<int> ans;
  for (i = 0; i < count; i++)
  {
    int num = (rand() % (upper - lower + 1)) + lower;
    ans.push_back(num);
  }
  return ans;
}


vector<string> bucket(int buck_size, int count, int lower, int upper)
{
  vector<int> pos = getRandom(0, buck_size-1, count);
  vector<int> delays = getRandom(lower, upper, count);
  vector<string> ans;
  for (int i = 0; i < count; i++)
  {
    string curr = to_string(delays[i]) + "," + to_string(pos[i]) + "\n";
    ans.push_back(curr);
  }
  return ans;
}

void client(const char *host, const char *msg)
{
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  
  const char *hostname = host;
  const char *port = "12345";

  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;

  status = getaddrinfo(hostname, port, &host_info, &host_info_list);
  if (status != 0)
  {
    cerr << "Error: cannot get address info for host" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    abort();
  }

  socket_fd = socket(host_info_list->ai_family,
                     host_info_list->ai_socktype,
                     host_info_list->ai_protocol);
  if (socket_fd == -1)
  {
    cerr << "Error: cannot create socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    abort();
  }

  status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
  {
    cerr << "Error: cannot connect to socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    std::cout << "failed: " << std::strerror(errno) << '\n';
    abort();
  }
  //enter the delay and bucket number
  const char *message = msg;
  //cout << message << endl;
  if (send(socket_fd, message, strlen(message), 0) == -1)
  {
    cerr << "ERROR: SEND" << endl;
  }
  //receive the number after add delay in bucket
  char buffer[1024];
  memset(buffer, 0, sizeof(buffer));
  if (recv(socket_fd, buffer, 1024, 0) < 0)
  {
    cerr << "ERROR: RECV" << endl;
  }
  std::lock_guard<std::mutex> lock(cout_mutex);
  cout << "new value of bucket updated: " <<buffer << endl;
  freeaddrinfo(host_info_list);
  close(socket_fd);
  //pthread_mutex_unlock(&lck);
}

int main(int argc, char *argv[])
{
  if (argc < 6)
  {
    cout << "Syntax: ./client <hostname> <client number> <bucket size> <lower> <upper>\n"
         << endl;
    return 1;
  }
  struct timespec start_time, end_time;

  char *host = argv[1];
  int count = atoi(argv[2]);
  int buk_size = atoi(argv[3]);
  int lower = atoi(argv[4]);
  int upper = atoi(argv[5]);

  vector<string> msgs = bucket(buk_size, count, lower, upper);
  std::thread threads[count];

  clock_gettime(CLOCK_MONOTONIC, &start_time);
  for (int i = 0; i < count; i++)
  {
    //cout << "delay,bucket: " << msgs[i].c_str() << endl;
    threads[i] = std::thread(client, host, msgs[i].c_str());
    //threads[i].join();
  }
  for(auto &i: threads){
    i.join();
  }

  clock_gettime(CLOCK_MONOTONIC, &end_time);
  
  double elapsed_ns = calc_time(start_time, end_time);
  cout << "Throughput: " << count/(elapsed_ns/1e9) << endl;
  return 0;
}
