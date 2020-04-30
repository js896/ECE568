#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <sstream>
using namespace std;

class Server{
  public:
  int status;
  int socket_fd;
  struct addrinfo host_info;
  struct addrinfo *host_info_list;
  const char *hostname = NULL;
  const char *port;
  int client_connection_fd;

  Server();
  void startListen();
  vector<int> getPara();
  //void sendToClient(int num);
  int getclient(){return client_connection_fd;}
};

