#pragma once

#include "DownloadEngine.hpp"
#include "Request.hpp"

class InitiateConnectionCommandFactory {
 public:
  static Command* createInitiateConnectionCommand(int cuid, Request* req, DownloadEngine* e);
};
