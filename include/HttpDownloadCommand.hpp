#pragma once

#include <map>
#include <string>

#include "DownloadCommand.hpp"
#include "DownloadEngine.hpp"
#include "Request.hpp"
#include "Socket.hpp"
#include "TransferEncoding.hpp"

class HttpDownloadCommand : public DownloadCommand {
 private:
  std::map<std::string, TransferEncoding*> transferEncodings;

 public:
  int cuid;

  HttpDownloadCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  ~HttpDownloadCommand();

  TransferEncoding* getTransferEncoding(std::string transferEncoding);
};
