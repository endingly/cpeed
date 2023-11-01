#pragma once

#include "DlAbortException.hpp"

class ConnectionException : public DlAbortException {
 public:
  ConnectionException() : DlAbortException() {}
  ConnectionException(std::string msg) : DlAbortException() {}
};
