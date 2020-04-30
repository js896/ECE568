#ifndef TUN1
#define TUN1

#include "../Socket/Proxy_Socket.h"
#include "Tunnel.h"

class Tunnel_Ver1 : public Tunnel {
 public:
  virtual void run() override;
  Tunnel_Ver1(Proxy_Socket * client_socket, Proxy_Socket * server_socket);
  ~Tunnel_Ver1();
};

#endif
