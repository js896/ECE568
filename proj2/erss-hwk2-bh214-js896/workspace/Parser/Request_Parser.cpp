#include "Request_Parser.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>

#include "../Exceptions/Parser_Exceptions.h"
#include "../Tools/String_Tools.h"
#include "../global_settings.h"

#define LN_END "\r\n"        // ending of each line in a request
#define LN_DLM " "           // delimiter in each line
#define HTTP_FLAG "http://"  // a flag in the uri that indicates the protocol is http
#define KV_DLM ": "
#define HTTP_DFT_PORT "80"    // default http port
#define HTTPS_DFT_PORT "443"  // default https port

void Request_Parser::parse(std::unordered_map<std::string, std::string> & map,
                           const std::vector<char> & request) {
  std::string request_str(request.data());
  std::vector<std::string> parts = String_Tools::split_str(request_str, LN_END);

  if (parts.size() <= 0) {
    std::cout << "Nothing parsed out of request!\n";
    throw new parse_request_exception;
  }
  if (parts.size() <= 1) {
    std::cout << "The request only has line1!\n";
    throw new parse_request_exception;
  }

  map.insert(std::make_pair("line1", parts[0]));

  parse_line1(map, parts[0]);

  int header_start = 1;

  int header_end = parse_header(map, parts, header_start);

  int body_start = header_end + 1;  // skip the extra "\r\n"
  if ((unsigned int)body_start < parts.size()) {
    parse_body(map, parts, body_start);
  }
  // get ip & port
  get_ip_port(map);
}

// parse line1: method, uri, http_ver
void Request_Parser::parse_line1(std::unordered_map<std::string, std::string> & map,
                                 const std::string & line1) {
  std::string ln_dlm(LN_DLM);
  std::vector<std::string> strs = String_Tools::split_str(line1, ln_dlm);
  if (strs.size() != 3) {
    std::cout << "The line1 has " << strs.size() << " elements instead of 3!\n";
    throw new get_line1_exception;
  }

  // method
  map.insert(std::make_pair("method", strs[0]));
  // uri
  map.insert(std::make_pair("uri", strs[1]));
  parse_uri(map, strs[1]);
  // http_ver
  map.insert(std::make_pair("http_ver", strs[2]));
}

// parse uri: get protocol (http or https)
void Request_Parser::parse_uri(std::unordered_map<std::string, std::string> & map,
                               const std::string & uri) {
  std::string::size_type uri_len = uri.length();
  std::string http_flag(HTTP_FLAG);
  std::string::size_type flag_len = http_flag.length();
  if (uri_len >= flag_len && uri.substr(0, flag_len).compare(http_flag) == 0) {  // http
    map.insert(std::make_pair("protocol", "http"));
  }
  else {
    map.insert(std::make_pair("protocol", "https"));
  }
}

// parse header
int Request_Parser::parse_header(std::unordered_map<std::string, std::string> & map,
                                 const std::vector<std::string> & parts,
                                 int header_start) {
  std::string kv_dlm(KV_DLM);
  int index = header_start;
  // add every line into the map
  while ((unsigned int)index < parts.size() && parts[index].length() > 0) {
    std::string::size_type key_end = parts[index].find(kv_dlm);
    if (key_end == std::string::npos) {
      std::cout << "\"" << kv_dlm << "\" not found in a header line!\n";
      throw new header_line_format_exception;
    }
    std::string key = parts[index].substr(0, key_end);
    std::string val = parts[index].substr(key_end + kv_dlm.length());
    map.insert(std::make_pair(key, val));
    index++;
  }

  return index;
}

// parse body: currently does nothing
void Request_Parser::parse_body(std::unordered_map<std::string, std::string> & map,
                                const std::vector<std::string> & parts,
                                int body_start) {
}

// extract ip & port info from 'Host'
void Request_Parser::get_ip_port(std::unordered_map<std::string, std::string> & map) {
  std::unordered_map<std::string, std::string>::iterator it;
  it = map.find("Host");
  if (it == map.end()) {
    std::cout << "No parsed Host info found; thus cannot get ip&port info!\n";
    throw new get_ip_port_exception;
  }
  std::string host = it->second;
  std::string::size_type ip_end = host.find(":");

  if (ip_end == std::string::npos) {  // default port
    map.insert(std::make_pair("ip", host));
    std::string protocol = map.find("protocol")->second;
    std::string port;
    if (protocol.compare("http") == 0) {  // protocol is http
      port = HTTP_DFT_PORT;
    }
    else {  // protocol is https
      port = HTTPS_DFT_PORT;
    }
    map.insert(std::make_pair("port", port));
  }
  else {  // specified port
    std::string ip = host.substr(0, ip_end);
    map.insert(std::make_pair("ip", ip));
    std::string port = host.substr(ip_end + 1);
    map.insert(std::make_pair("port", port));
  }
}
