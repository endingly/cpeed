#pragma once

#include "Logger.hpp"

class SimpleLogger : public Logger {
 private:
  enum LEVEL { DEBUG, INFO, ERROR };
  void  writeLog(int level, std::string msg, va_list ap, Exception* e = NULL);
  FILE* file;

 public:
  SimpleLogger(std::string filename);
  SimpleLogger(FILE* logfile);
  ~SimpleLogger();

  void debug(std::string msg, ...);
  void debug(std::string msg, Exception* ex, ...);
  void info(std::string msg, ...);
  void info(std::string msg, Exception* ex, ...);
  void error(std::string msg, ...);
  void error(std::string msg, Exception* ex, ...);
};
