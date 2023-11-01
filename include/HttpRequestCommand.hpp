#pragma once

#include "AbstractCommand.hpp"

class HttpRequestCommand : public AbstractCommand {
 protected:
  bool executeInternal(Segment segment);

 public:
  HttpRequestCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  ~HttpRequestCommand();
};
