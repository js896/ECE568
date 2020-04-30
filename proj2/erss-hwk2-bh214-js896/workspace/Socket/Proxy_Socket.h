#ifndef PROXY_SOCKET
#define PROXY_SOCKET

#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <string>
#include <vector>

class Proxy_Socket {
 protected:
  int sock_fd;

  // prepares addrinfo with specified ip & port
  virtual struct addrinfo * get_addrinfo(const std::string & ip,
                                         const std::string & port) = 0;

 public:
  // get the current port number of the socket
  virtual std::string get_socket_port() = 0;

  // bind socket to specific ip & port
  virtual void bind_to(const std::string & ip, const std::string & port) = 0;

  // send a string
  virtual void send_str(const std::vector<char> & content) = 0;

  // receive a string
  virtual std::vector<char> recv_str(int buffer_size) = 0;

  // start listening
  virtual void set_listen(int back_log) = 0;

  // accept a connection & return the respawned socket
  virtual Proxy_Socket * accept_connection() = 0;

  // connect to a remote ip & port
  virtual void connect_to(const std::string & ip, const std::string & port) = 0;

  // allow port number reuse
  virtual void allow_port_reuse() = 0;

  // get socket fd
  virtual int get_fd() = 0;

  //close socket
  virtual void close_socket() = 0;

  // get peer ip
  virtual std::string get_peer_ip() = 0;
};

#endif
