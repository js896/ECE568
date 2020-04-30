#include "../Parser/Response_Parser.h"

#include <cstdlib>
#include <string>
#include <vector>

int main() {
  std::string test = "HTTP/1.0 200 OK\r\nContent-Type: text/plain\r\nTransfer-Encoding: "
                     "chunked\r\nTrailer: Expires\r\n\r\n7\r\nMozilla\r\n\r\n";
  std::vector<char> test_vector;
  std::unordered_map<std::string, std::string> map;
  String_Tools::load_into_vector(test_vector, test);
  Response_Parser::parse(map, test_vector);
  for (auto it : map) {
    std::cout << it.first << ": " << it.second << std::endl;
  }

  return EXIT_SUCCESS;
}
