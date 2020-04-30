#include "Logger.h"

#include <cstdlib>

std::mutex Logger::mtx;

void Logger::write_to_file(std::string log, std::string path) {
  mtx.lock();
  //  std::cout << inp[0] << std::endl;
  std::fstream logger;
  logger.open(path, std::fstream::app);

  logger << log;

  logger << std::endl;
  logger.close();
  mtx.unlock();
}
