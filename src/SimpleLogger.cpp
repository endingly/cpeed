#include "SimpleLogger.hpp"

#include <cstdarg>
using std::string;

SimpleLogger::SimpleLogger(string filename) { file = fopen(filename.c_str(), "a"); }

SimpleLogger::SimpleLogger(FILE* logfile) { file = logfile; }

SimpleLogger::~SimpleLogger() {
  if (file != NULL) {
    fclose(file);
  }
}

void SimpleLogger::writeLog(int level, string msg, va_list ap, Exception* e) {
  string levelStr;
  switch (level) {
    case DEBUG:
      levelStr = "DEBUG";
      break;
    case ERROR:
      levelStr = "ERROR";
      break;
    case INFO:
    default:
      levelStr = "INFO";
  }
  vfprintf(file, string(levelStr + " - " + msg + "\n").c_str(), ap);
  if (e != NULL) {
    fprintf(file, string(levelStr + " - exception: " + e->getMsg() + "\n").c_str());
  }
  fflush(stdout);
}

void SimpleLogger::debug(string msg, ...) {
  va_list ap;
  va_start(ap, msg);
  writeLog(DEBUG, msg, ap);
  va_end(ap);
}

void SimpleLogger::debug(string msg, Exception* e, ...) {
  va_list ap;
  va_start(ap, e);
  writeLog(DEBUG, msg, ap, e);
  va_end(ap);
}

void SimpleLogger::info(string msg, ...) {
  va_list ap;
  va_start(ap, msg);
  writeLog(INFO, msg, ap);
  va_end(ap);
}

void SimpleLogger::info(string msg, Exception* e, ...) {
  va_list ap;
  va_start(ap, e);
  writeLog(INFO, msg, ap, e);
  va_end(ap);
}

void SimpleLogger::error(string msg, ...) {
  va_list ap;
  va_start(ap, msg);
  writeLog(ERROR, msg, ap);
  va_end(ap);
}

void SimpleLogger::error(string msg, Exception* e, ...) {
  va_list ap;
  va_start(ap, e);
  writeLog(ERROR, msg, ap, e);
  va_end(ap);
}
