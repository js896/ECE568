#ifndef LOGGER
#define LOGGER
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Logger {
 public:
  static std::mutex mtx;
  static void write_to_file(std::string log, std::string path);
};

#endif
