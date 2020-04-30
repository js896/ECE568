#include <iostream>
#include <string>
#include <vector>

#include "../Tools/String_Tools.h"

int main(int argc, char ** args) {
  if (argc != 3) {
    std::cout << "usage: str_split_test <str> <delimiter>\n";
    return 0;
  }

  std::string str(args[1]);
  std::string delimiter(args[2]);

  std::vector<std::string> res = String_Tools::split_str(str, delimiter);

  for (unsigned int i = 0; i < res.size(); i++) {
    std::cout << res[i] << "\n";
  }
}
