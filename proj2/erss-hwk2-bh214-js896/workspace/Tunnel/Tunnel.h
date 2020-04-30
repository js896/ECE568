#ifndef TNNL
#define TNNL

#include "../Socket/Proxy_Socket.h"

class Tunnel {
 protected:
  Proxy_Socket * client_socket;
  Proxy_Socket * server_socket;

 public:
  virtual void run() = 0;
};

#endif
