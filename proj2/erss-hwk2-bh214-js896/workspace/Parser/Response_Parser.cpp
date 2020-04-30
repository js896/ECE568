#include "Response_Parser.h"

#define LN_END "\r\n"
#define KV_DLM ": "

void Response_Parser::parse(std::unordered_map<std::string, std::string> & map,
                            const std::vector<char> & response) {
  std::string response_str(response.data());
  std::vector<std::string> parts = String_Tools::split_str(response_str, LN_END);
  if (parts.size() >= 1) {
    map.insert(std::make_pair("line1", parts[0]));
  }
  for (size_t i = 0; i < parts.size(); ++i) {
    if (parts[i].size() == 0) {
      break;
    }
    if (parts[i].find(KV_DLM) != std::string::npos) {
      std::string::size_type pos = parts[i].find(KV_DLM);
      std::string key = parts[i].substr(0, pos);
      std::string value = parts[i].substr(pos + 2);
      map.insert(std::make_pair(key, value));
    }
  }
}
