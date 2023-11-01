#pragma once
#include "Exception.hpp"

class Logger {
 public:
  virtual ~Logger() {}
  virtual void debug(std::string msg, ...)                = 0;
  virtual void debug(std::string msg, Exception* ex, ...) = 0;
  virtual void info(std::string msg, ...)                 = 0;
  virtual void info(std::string msg, Exception* ex, ...)  = 0;
  virtual void error(std::string msg, ...)                = 0;
  virtual void error(std::string msg, Exception* ex, ...) = 0;
};
