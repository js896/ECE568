#include <iostream>

#include "../Tools/String_Tools.h"

int main() {
  std::string str1 = "Wed, 21 Oct 2015 07:28:00 GMT";
  // std::string str2 = "Mon, 27 Jul 2009 12:28:53 GMT";
  // std::string str3 = "Sun, 18 Oct 2012 10:36:20 GMT";
  struct tm tm1 = String_Tools::rqs_to_tm(str1);
  // struct tm tm2 = String_Tools::rqs_to_tm(str2);
  // struct tm tm3 = String_Tools::rqs_to_tm(str3);

  std::cout << "second: " << tm1.tm_sec << std::endl;
  std::cout << "minute: " << tm1.tm_min << std::endl;
  std::cout << "hour: " << tm1.tm_hour << std::endl;
  std::cout << "day: " << tm1.tm_mday << std::endl;
  std::cout << "month: " << tm1.tm_mon << std::endl;
  std::cout << "year: " << tm1.tm_year << std::endl;
  std::cout << "weekday: " << tm1.tm_wday << std::endl;
  std::cout << "yearday: " << tm1.tm_yday << std::endl;
  std::cout << "is_dst: " << tm1.tm_isdst << std::endl;
  return EXIT_SUCCESS;
}
