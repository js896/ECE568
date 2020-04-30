#ifndef PAS_EXC
#define PAS_EXC

#include <exception>

class get_line1_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Parser failed to get line1 from the Request!";
  }
};

class get_header_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Parser failed to get header from the Request!";
  }
};

class get_body_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Parser failed to get body from the Request!";
  }
};

class get_uri_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Parser failed to get uri from the Request!";
  }
};

class get_ip_port_exception : public std::exception {
  virtual const char * what() const throw() {
    return "Parser failed to get ip & port info from the Request!";
  }
};

class get_method_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "Parser failed to get the method from the Request line1!";
  }
};

class get_protocol_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "Parser failed to get the protocol from the Request line1!";
  }
};

class parse_request_exception : public std::exception {
 public:
  virtual const char * what() const throw() { return "Parser failed to parse this request!"; }
};

class header_line_format_exception : public std::exception {
 public:
  virtual const char * what() const throw() {
    return "The line in header part has incorrect format!";
  }
};

#endif
