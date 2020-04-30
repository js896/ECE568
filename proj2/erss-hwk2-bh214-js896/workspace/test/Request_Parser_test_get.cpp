#include "../Parser/Request_Parser.h"
#include "../Tools/String_Tools.h"

int main() {
  std::string get_str =
      "GET /hello.htm HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows "
      "NT)\r\nHost: www.tutorialspoint.com\r\nAccept-Language: en-us\r\nAccept-Encoding: "
      "gzip, deflate\r\nConnection: Keep-Alive\r\n";
  std::vector<char> get_vector;
  String_Tools::load_into_vector(get_vector, get_str);
  for (size_t i = 0; i < get_vector.size(); ++i) {
    std::cout << get_vector[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
