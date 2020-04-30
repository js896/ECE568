#include <iostream>

#include "../Tools/String_Tools.h"

int main() {
  std::string str1 = "Wed, 21 Oct 2015 07:28:00 GMT";
  std::string str2 = "Mon, 27 Jul 2009 12:28:53 GMT";
  time_t test1 = String_Tools::rqs_to_time_t(str1);
  time_t test2 = String_Tools::rqs_to_time_t(str2);
  std::string res = test1 > test2 ? "test1" : "test2";
  std::cout << res << std::endl;
}
