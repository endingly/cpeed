#pragma once
#include <cstdarg>
#include <string>

/// @brief 异常基类
class Exception {
 private:
  std::string msg;

 protected:
  void setMsg(std::string msgsrc, ...) {
    va_list ap;
    va_start(ap, msgsrc);
    char buf[256];
    vsnprintf(buf, sizeof(buf), msgsrc.c_str(), ap);
    msg = buf;
    va_end(ap);
  }

 public:
  Exception() {}
  virtual ~Exception() {}
  std::string getMsg() { return msg; }
};
