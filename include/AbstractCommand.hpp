#pragma once

#include "Command.hpp"
#include "DownloadEngine.hpp"
#include "Request.hpp"
#include "SegmentManager.hpp"

class AbstractCommand : public Command {
 private:
  void           updateCheckPoint();
  bool           isTimeoutDetected();
  struct timeval checkPoint;

 protected:
  Request*        req;
  DownloadEngine* e;
  Socket*         socket;
  bool            checkSocketIsReadable;
  bool            checkSocketIsWritable;
  virtual bool    prepareForRetry();
  virtual void    onError(Exception* e);
  virtual bool    executeInternal(Segment segment) = 0;

 public:
  AbstractCommand(int cuid, Request* req, DownloadEngine* e, Socket* s = NULL);
  virtual ~AbstractCommand();
  bool execute();
};
