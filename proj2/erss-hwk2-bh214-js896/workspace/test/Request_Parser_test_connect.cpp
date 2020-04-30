#include "../Parser/Request_Parser.h"
#include "../Tools/String_Tools.h"

int main() {
  std::string connect_str =
      "CONNECT server.example.com:80 HTTP/1.1\r\nHost: "
      "server.example.com:80\r\nProxy-Authorization: basic aGVsbG86d29ybGQ=\r\n";
  std::vector<char> connect_vector;
  String_Tools::load_into_vector(connect_vector, connect_str);
  for (size_t i = 0; i < connect_vector.size(); ++i) {
    std::cout << connect_vector[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
