#pragma once
#include <string>

class TimeoutException {
 public:
  int         err;
  std::string msg;

 public:
  TimeoutException() : err(0), msg(""){};
  TimeoutException(int err) : err(err), msg(""){};
  TimeoutException(std::string msg) : err(0), msg(msg){};
  TimeoutException(int err, std::string msg) : err(err), msg(msg) {}
};
