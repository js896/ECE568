#include "Tunnel_Ver1.h"

#include <iostream>

#include "../Exceptions/Socket_Exceptions.h"
#include "../Exceptions/Tunnel_Exceptions.h"
#include "../Socket/Socket_Ver1.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

void Tunnel_Ver1::run() {
  int client_fd = this->client_socket->get_fd();
  int server_fd = this->server_socket->get_fd();
  int max = (client_fd >= server_fd ? client_fd : server_fd) + 1;

  fd_set read_set;

  std::vector<char> container;
  String_Tools::load_into_vector(container, "200 OK");
  client_socket->send_str(container);

  while (true) {
    // clear read_set
    FD_ZERO(&read_set);

    // add in fds
    FD_SET(client_fd, &read_set);
    FD_SET(server_fd, &read_set);

    int select_state = select(max, &read_set, NULL, NULL, NULL);
    if (select_state < 0) {
      //std::cout << "Tunnel select() returns negative value; " << strerror(errno) << "\n";
      throw new select_exception;
    }
    else {
      if (FD_ISSET(server_fd, &read_set)) {  // msg comes from server
        std::vector<char> content;
        try {
          content = this->server_socket->recv_str(SERVER_BUF_SIZE);
        }
        catch (connection_break_exception * e) {
          // std::cout << "tunnel has been closed by server\n";
          break;
        }

        // forward message to client
        try {
          this->client_socket->send_str(content);
          //          std::cout << "Tunnel to client: " << content.data();
        }
        catch (send_str_exception & e) {
          //          std::cout << "tunnel failed to forward msg to "
          //"client!\n";
          throw new forward_to_client_exception;
        }
      }
      else if (FD_ISSET(client_fd, &read_set)) {  // msg comes from client
        std::vector<char> content;
        try {
          content = this->client_socket->recv_str(SERVER_BUF_SIZE);
        }
        catch (connection_break_exception * e) {
          //std::cout << "tunnel has been closed by client\n";
          break;
        }

        // forward message to server
        try {
          this->server_socket->send_str(content);
          //  std::cout << "Tunnel to server: " << content.data();
        }
        catch (send_str_exception e) {
          //std::cout << "tunnel failed to forward msg to server!\n";
          throw new forward_to_server_exception;
        }
      }
      else {
        //  std::cout << "tunnel failed to select either client or server!\n";
        throw new select_exception;
      }
    }
  }
}

Tunnel_Ver1::Tunnel_Ver1(Proxy_Socket * client_socket, Proxy_Socket * server_socket) {
  this->client_socket = client_socket;
  this->server_socket = server_socket;
}

Tunnel_Ver1::~Tunnel_Ver1() {
  this->client_socket->close_socket();
  this->server_socket->close_socket();
}
