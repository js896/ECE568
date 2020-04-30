#include <iostream>

#include "../Handler/Handler.h"
#include "../Message/Request_Message.h"
#include "../Parser/Request_Parser.h"
#include "../Socket/Socket_Ver1.h"
#include "../Tunnel/Tunnel_Ver1.h"
#include "../global_settings.h"
int main(int argc, char * argv[]) {
  std::string port(argv[1]);

  Handler hd(port);
  hd.run();
  // Socket_Ver1 main_socket;

  // main_socket.bind_to("0.0.0.0", port);

  // main_socket.set_listen(5);

  // Proxy_Socket * client_socket = main_socket.accept_connection();

  // std::string request = client_socket->recv_str(512);

  // std::cout << request << "\n";

  // std::cout << "parsed result:\n";
  // Request_Message request_message(request);

  // for (std::pair<std::string, std::string> kv : request_message.map) {
  //   std::cout << kv.first << " : " << kv.second << "\n";
  // }
  // client_socket->close_socket();
  // main_socket.close_socket();
}
