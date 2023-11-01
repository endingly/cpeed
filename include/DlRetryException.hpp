#include "Exception.hpp"

class DlRetryException : public Exception {
 public:
  DlRetryException() : Exception() {}
  DlRetryException(std::string msg, ...) : Exception() {
    va_list ap;
    va_start(ap, msg);
    setMsg(msg, ap);
    va_end(ap);
  }
};
