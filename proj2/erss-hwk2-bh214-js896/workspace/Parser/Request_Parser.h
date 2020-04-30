#ifndef RQS_PAR
#define RQS_PAR

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

// in a request, we have:
// 1. request line --> line1
// 2. request header --> header
// 3. request body --> body
class Request_Parser {
 public:
  static void parse(std::unordered_map<std::string, std::string> & map,
                    const std::vector<char> & request);

 protected:
  // cut line1 into 3 parts: method, uri, http_ver
  static void parse_line1(std::unordered_map<std::string, std::string> & map,
                          const std::string & line1);

  // parse uri, get protocol from it
  static void parse_uri(std::unordered_map<std::string, std::string> & map,
                        const std::string & uri);

  // parse header (just store every line into hashmap)
  // returns the end index of header
  static int parse_header(std::unordered_map<std::string, std::string> & map,
                          const std::vector<std::string> & parts,
                          int header_start);

  // parse body (currently does nothing)
  static void parse_body(std::unordered_map<std::string, std::string> & map,
                         const std::vector<std::string> & parts,
                         int body_start);

  // extract ip & port info from 'Host'
  static void get_ip_port(std::unordered_map<std::string, std::string> & map);
};

#endif
