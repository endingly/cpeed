#pragma once

#include "DlRetryException.hpp"

class SocketIOException : public DlRetryException {
 public:
  SocketIOException() : DlRetryException(){};
  SocketIOException(std::string msg) : DlRetryException(msg){};
};
