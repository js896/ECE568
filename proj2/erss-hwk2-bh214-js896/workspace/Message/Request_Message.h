#ifndef RQS_MSG
#define RQS_MSG

#include <cstdlib>
#include <string>
#include <unordered_map>

#include "../Parser/Request_Parser.h"

class Request_Message {
 public:
  std::unordered_map<std::string, std::string> map;
  Request_Message(std::vector<char> request);
  std::string get_ip();
  std::string get_port();
  std::string get_uri();
};

#endif
