#pragma once

#include "AbstractCommand.hpp"
#include "TransferEncoding.hpp"
#include <string>

class DownloadCommand : public AbstractCommand {
protected:
  bool executeInternal(Segment segment);

  bool prepareForRetry();
  bool prepareForNextSegment();

public:
  DownloadCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  virtual ~DownloadCommand();

  virtual TransferEncoding* getTransferEncoding(string transferEncoding) = 0;

  string transferEncoding;

};
