#include "Request_Message.h"

#include <vector>

#include "../Parser/Request_Parser.h"
Request_Message::Request_Message(std::vector<char> request) {
  Request_Parser::parse(this->map, request);
}

std::string Request_Message::get_ip() {
  return this->map.find("ip")->second;
}
std::string Request_Message::get_port() {
  return this->map.find("port")->second;
}
std::string Request_Message::get_uri() {
  return this->map.find("uri")->second;
}
