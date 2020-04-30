#include "../Parser/Request_Parser.h"
#include "../Tools/String_Tools.h"

int main() {
  std::string connect_str =
      "POST /cgi-bin/process.cgi HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; "
      "Windows NT)\r\nHost: www.tutorialspoint.com\r\nContent-Type: text/xml; "
      "charset=utf-8\r\nContent-Length: length\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, "
      "deflate\r\nConnection: "
      "Keep-Alive\r\n\r\nlicenseID=string&content=string&/paramsXML=string\r\n";
  std::vector<char> connect_vector;
  String_Tools::load_into_vector(connect_vector, connect_str);
  for (size_t i = 0; i < connect_vector.size(); ++i) {
    std::cout << connect_vector[i] << std::endl;
  }
  return EXIT_SUCCESS;
}
