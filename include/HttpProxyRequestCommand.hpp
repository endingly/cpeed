#pragma once

#include "AbstractCommand.hpp"

class HttpProxyRequestCommand : public AbstractCommand {
 protected:
  bool executeInternal(Segment segment);

 public:
  HttpProxyRequestCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  ~HttpProxyRequestCommand();
};
