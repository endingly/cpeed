#pragma once

#include <string>

#include "AbstractCommand.hpp"
#include "TransferEncoding.hpp"

class DownloadCommand : public AbstractCommand {
 protected:
  bool executeInternal(Segment segment);

  bool prepareForRetry();
  bool prepareForNextSegment();

 public:
  DownloadCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  virtual ~DownloadCommand();

  virtual TransferEncoding* getTransferEncoding(std::string transferEncoding) = 0;

  std::string transferEncoding;
};
