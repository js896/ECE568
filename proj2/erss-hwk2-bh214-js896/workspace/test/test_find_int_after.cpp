#include <iostream>

#include "../Tools/String_Tools.h"

int main() {
  std::string test = "AbCD78910";
  std::string prefix = "AbCD";
  std::string test1 = "AbCD78910fghj";
  std::string prefix1 = "AbCD";

  int res = String_Tools::find_int_after(test, prefix);
  int res1 = String_Tools::find_int_after(test1, prefix1);
  std::cout << res << std::endl;
  std::cout << res1 << std::endl;
  return EXIT_SUCCESS;
}
