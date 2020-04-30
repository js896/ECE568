#ifndef RSP_PAR
#define RSP_PAR

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Tools/String_Tools.h"

class Response_Parser {
 public:
  static void parse(std::unordered_map<std::string, std::string> & map,
                    const std::vector<char> & response);
};

#endif
