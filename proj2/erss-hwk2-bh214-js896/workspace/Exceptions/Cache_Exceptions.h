#ifndef CACH_EXCP
#define CACH_EXCP

#include <exception>

class cache_not_found_exception : public std::exception {
 public:
  virtual const char * what() const throw() override {
    return "Cannot find the required node in the cache!";
  }
};

#endif
