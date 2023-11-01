#pragma once

#include "AbstractCommand.hpp"

class HttpProxyResponseCommand : public AbstractCommand {
 protected:
  bool executeInternal(Segment segment);

 public:
  HttpProxyResponseCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  ~HttpProxyResponseCommand();
};
