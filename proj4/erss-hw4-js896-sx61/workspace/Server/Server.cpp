#include "Server.h"

Server::Server()
{
  port = "12345";
}

void Server::startListen()
{
  memset(&host_info, 0, sizeof(host_info));
  host_info.ai_family = AF_UNSPEC;
  host_info.ai_socktype = SOCK_STREAM;
  host_info.ai_flags = AI_PASSIVE;

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

  int yes = 1;
  status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  ::bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
  if (status == -1)
  {
    cerr << "Error: cannot bind socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    abort();
  }

  status = listen(socket_fd, 100);
  if (status == -1)
  {
    cerr << "Error: cannot listen on socket" << endl;
    cerr << "  (" << hostname << "," << port << ")" << endl;
    abort();
  }
}

vector<int> Server::getPara()
{
  struct sockaddr_storage socket_addr;
  socklen_t socket_addr_len = sizeof(socket_addr);
  client_connection_fd = accept(socket_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
  if (client_connection_fd == -1)
  {
    cerr << "Error: cannot accept connection on socket" << endl;
    cout << "error: " << strerror(errno) << endl;
    abort();
  }
  //cout << "error: "<<strerror(errno) << endl;
  char *buffer = (char *)malloc(1024);
  int buffer_len = recv(client_connection_fd, buffer, 1024, 0);

  if (buffer_len <= 0)
  {
    cout << "ERROR RECV" << endl;
  }
  vector<int> res;
  vector<char> container;
  for (int i = 0; i < buffer_len; ++i)
  {
    container.push_back(buffer[i]);
  }
  string temp(container.begin(), container.end());
  string delay = temp.substr(0, temp.find(","));
  string num_bucket = temp.substr(temp.find(",") + 1, temp.find("\n") - temp.find(",") - 1);
  cout << "delay: " << delay << endl
       << "num_bucket: " << num_bucket << endl;
  res.push_back(stoi(delay));
  res.push_back(stoi(num_bucket));
  //freeaddrinfo(host_info_list);
  //close(socket_fd);
  free(buffer);
  return res;
}
/*
void Server::sendToClient(int num)
{
  stringstream ss;
  ss << num;
  send(client_connection_fd, ss.str().c_str(), strlen(ss.str().c_str()), 0);
  cout << "fd: " << client_connection_fd << endl;
  freeaddrinfo(host_info_list);
  //    close(socket_fd);
}
*/