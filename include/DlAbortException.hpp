#pragma once
#include "Exception.hpp"

class DlAbortException : public Exception {
 public:
  DlAbortException() : Exception() {}
  DlAbortException(std::string msg, ...) : Exception() {
    va_list ap;
    va_start(ap, msg);
    setMsg(msg, ap);
    va_end(ap);
  }
};
