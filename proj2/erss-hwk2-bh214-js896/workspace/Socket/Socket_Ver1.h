#ifndef SOCKET_VER1
#define SOCKET_VER1

#include <vector>

#include "Proxy_Socket.h"

class Socket_Ver1 : public Proxy_Socket {
 protected:
  virtual struct addrinfo * get_addrinfo(const std::string & ip,
                                         const std::string & port) override;

 public:
  Socket_Ver1();
  Socket_Ver1(int fd);  // use socket fd to create a socket object
  ~Socket_Ver1();       // close the socket fd when destructed

  // get the current port number of the socket
  virtual std::string get_socket_port() override;

  // send a string
  virtual void send_str(const std::vector<char> & content) override;

  // receive a string
  virtual std::vector<char> recv_str(int buffer_size) override;  // receive a string

  // enter listen state
  virtual void set_listen(int back_log) override;

  // accept a connection & return the respawned socket
  virtual Proxy_Socket * accept_connection() override;

  // connect to remove ip & port
  virtual void connect_to(const std::string & ip, const std::string & port) override;

  // bind socket to specific ip & port
  virtual void bind_to(const std::string & ip, const std::string & port) override;

  // allow port reuse
  virtual void allow_port_reuse() override;

  // get socket fd
  virtual int get_fd() override;

  //close socket
  virtual void close_socket() override;

  virtual std::string get_peer_ip() override;
};

#endif
