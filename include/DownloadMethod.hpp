#pragma once
#include "Request.hpp"

class DownloadMethod {
 public:
  virtual void sendRequest(Request& request)          = 0;
  virtual void getDataFragment(char* buffer, int len) = 0;
};
