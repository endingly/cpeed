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

#endif // _D_DOWNLOAD_COMMAND_H_
