#include "Socket_Ver1.h"

#include <arpa/inet.h>  // inet_ntop
#include <netdb.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <iostream>
#include <vector>

#include "../Exceptions/Socket_Exceptions.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"
// default constructor
Socket_Ver1::Socket_Ver1() {
  std::string ip_hint(ADDRINFO_HINT_IP);
  std::string port_hint(ADDRINFO_HINT_PORT);

  struct addrinfo * info = this->get_addrinfo(ip_hint, port_hint);
  int created_fd = socket(info->ai_family,
                          info->ai_socktype,
                          info->ai_protocol);  // that only useful thing
  this->sock_fd = created_fd;

  // free memory
  freeaddrinfo(info);
}

// construct with the socket fd
Socket_Ver1::Socket_Ver1(int sock_fd) {
  this->sock_fd = sock_fd;
}

// close the socket fd when distructed
Socket_Ver1::~Socket_Ver1() {
  this->close_socket();
}

std::string Socket_Ver1::get_socket_port() {
  int status = 0;
  int sock_fd = this->sock_fd;

  // get sockaddr
  struct sockaddr_storage localaddr;
  socklen_t addrlen = sizeof(localaddr);

  status = getsockname(sock_fd, (struct sockaddr *)(&localaddr), &addrlen);
  if (status != 0) {
    printf("Failed to get local socket port number!\n");
    throw new get_socket_port_exception;
  }

  // extract & ntohs the port number
  u_int16_t port_num = 0;
  if (localaddr.ss_family == AF_INET) {  // IPv4
    port_num = ntohs(((struct sockaddr_in *)(&localaddr))->sin_port);
  }
  else {  // IPv6
    port_num = ntohs(((struct sockaddr_in6 *)(&localaddr))->sin6_port);
  }

  // store port number as string
  std::string str = std::to_string(port_num);

  return str;
}

// get addrinfo
struct addrinfo * Socket_Ver1::get_addrinfo(const std::string & ip,
                                            const std::string & port) {
  int status = 0;

  struct addrinfo hint;
  struct addrinfo * res;
  memset(&hint, 0, sizeof(hint));
  hint.ai_family = AF_UNSPEC;
  hint.ai_socktype = SOCK_STREAM;

  char * ip_char = (char *)malloc(ip.length() + 1);
  char * port_char = (char *)malloc(port.length() + 1);
  strcpy(ip_char, ip.c_str());
  strcpy(port_char, port.c_str());

  status = getaddrinfo(ip_char, port_char, &hint, &res);
  if (status != 0) {
    std::cout << gai_strerror(status) << "\n";
    std::cout << ip_char << "(ip)\n";
    std::cout << port_char << "(port)\n";
    throw new get_addrinfo_exception();
  }
  return res;
}

// connect to a designated ip & port
void Socket_Ver1::connect_to(const std::string & ip, const std::string & port) {
  int status = 0;

  struct addrinfo * info = this->get_addrinfo(ip, port);

  status = connect(this->sock_fd, info->ai_addr, info->ai_addrlen);

  if (status == -1) {
    throw new connect_to_exception();
  }
  freeaddrinfo(info);
}

void Socket_Ver1::send_str(const std::vector<char> & str) {
  int total_bytes = str.size();
  // +1 for sending the null-terminator

  int sent_bytes = 0;

  char * buffer = String_Tools::vector_to_chars(str);

  while (sent_bytes != total_bytes) {
    int this_time_sent_bytes =
        send(this->sock_fd, buffer + sent_bytes, total_bytes - sent_bytes, 0);
    if (this_time_sent_bytes == -1) {
      free(buffer);
      throw new send_str_exception();
    }
    sent_bytes += this_time_sent_bytes;
  }

  free(buffer);
}

std::vector<char> Socket_Ver1::recv_str(int buffer_size) {
  char * buffer = (char *)malloc(buffer_size);
  memset(buffer, 0, buffer_size);

  int buffer_len = recv(sock_fd, buffer, buffer_size, 0);

  //std::cout << "What is in the recv buffer??::" << buffer << "\n";
  if (buffer_len == 0) {
    std::cout << "Received string length = 0; " << strerror(errno) << "\n";
    throw new connection_break_exception();
  }

  if (buffer_len < 0) {
    std::cout << "Received string length < 0; " << strerror(errno) << "\n";
    throw new recv_str_exception();
  }

  std::vector<char> container;
  String_Tools::load_chars_into_vector(container, buffer, buffer_len);

  // vector of chars to string
  return container;
}

void Socket_Ver1::set_listen(int back_log) {
  int status;

  status = listen(this->sock_fd, back_log);
  if (status == -1) {
    throw new socket_listen_exception();
  }
  return;
}

Proxy_Socket * Socket_Ver1::accept_connection() {
  // make useless stuff
  struct sockaddr_storage ss;
  unsigned int ss_size = sizeof(ss);

  // accept connection & get the respawned socket's fd
  int respawned_sock_fd = accept(this->sock_fd, (struct sockaddr *)&ss, &ss_size);

  if (respawned_sock_fd == -1) {
    throw new socket_accept_exception();
    exit(-1);
  }

  Proxy_Socket * respawned_sock = new Socket_Ver1(respawned_sock_fd);

  return respawned_sock;
}

void Socket_Ver1::bind_to(const std::string & ip, const std::string & port) {
  int status = 0;
  struct addrinfo * addrinfo_list = this->get_addrinfo(ip, port);

  // bind to ip&port
  status = bind(this->sock_fd, addrinfo_list->ai_addr, addrinfo_list->ai_addrlen);

  if (status == -1) {
    std::cout << "Socket failed to bind to " << ip << ":" << port << "\n";
    throw new socket_bind_exception();
  }
}

void Socket_Ver1::allow_port_reuse() {
  int reuse = 1;
  int state = 0;
  state = setsockopt(this->sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

  if (state == -1) {
    throw new allow_port_reuse_exception();
  }
}

int Socket_Ver1::get_fd() {
  return this->sock_fd;
}

//close fd
void Socket_Ver1::close_socket() {
  close(this->sock_fd);
}

std::string Socket_Ver1::get_peer_ip() {
  char * buffer = (char *)malloc(IP_LEN);

  // clear buffer
  memset(buffer, 0, IP_LEN);

  struct sockaddr_storage localaddr;
  socklen_t addrlen = sizeof(localaddr);
  getpeername(this->sock_fd, (struct sockaddr *)(&localaddr), &addrlen);

  if (localaddr.ss_family == AF_INET) {  // IPv4
    inet_ntop(localaddr.ss_family,
              &(((struct sockaddr_in *)(&localaddr))->sin_addr),
              buffer,
              INET_ADDRSTRLEN);
  }
  else {  // IPv6
    inet_ntop(localaddr.ss_family,
              &(((struct sockaddr_in6 *)(&localaddr))->sin6_addr),
              buffer,
              INET6_ADDRSTRLEN);
  }
  std::string res(buffer);
  free(buffer);
  return res;
}
