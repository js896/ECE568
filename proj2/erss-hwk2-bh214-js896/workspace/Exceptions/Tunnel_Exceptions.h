#ifndef TEXCPT
#define TEXCPT

#include <exception>

class select_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "Tunnel select() method returns -1!";
  }
};

class forward_to_client_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "Tunnel failed to forward server's msg to client!";
  }
};

class forward_to_server_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "Tunnel failed to forward client's msg to server!";
  }
};

#endif
