#include "DownloadCommand.hpp"

#include <sys/time.h>

#include "DlRetryException.hpp"
#include "HttpInitiateConnectionCommand.hpp"
#include "InitiateConnectionCommandFactory.hpp"
#include "Message.hpp"
#include "SleepCommand.hpp"
#include "Util.hpp"

DownloadCommand::DownloadCommand(int cuid, Request* req, DownloadEngine* e, Socket* s)
    : AbstractCommand(cuid, req, e, s) {
  AbstractCommand::checkSocketIsReadable = true;
}

DownloadCommand::~DownloadCommand() {}

bool DownloadCommand::executeInternal(Segment seg) {
  TransferEncoding* te = NULL;
  if (transferEncoding.size()) {
    te = getTransferEncoding(transferEncoding);
    assert(te != NULL);
  }
  int  bufSize = 4096;
  char buf[bufSize];
  socket->readData(buf, bufSize);
  if (te != NULL) {
    int  infbufSize = 4096;
    char infbuf[infbufSize];
    te->inflate(infbuf, infbufSize, buf, bufSize);
    e->diskWriter->writeData(infbuf, infbufSize, seg.sp + seg.ds);
  } else {
    e->diskWriter->writeData(buf, bufSize, seg.sp + seg.ds);
  }
  seg.ds += bufSize;

  if (te != NULL && te->finished() || te == NULL && seg.ds >= seg.ep - seg.sp + 1 ||
      e->segmentMan->totalSize == 0 && bufSize == 0) {
    if (te != NULL) te->end();
    e->logger->info(MSG_DOWNLOAD_COMPLETED, cuid);
    seg.ds     = seg.ep - seg.sp + 1;
    seg.finish = true;
    e->segmentMan->updateSegment(seg);
    // this unit is going to download another segment.
    return prepareForNextSegment();
  } else {
    e->segmentMan->updateSegment(seg);
    e->commands.push(this);
    return false;
  }
}

bool DownloadCommand::prepareForRetry() {
  req->resetUrl();
  Command* command =
      InitiateConnectionCommandFactory::createInitiateConnectionCommand(cuid, req, e);
  SleepCommand* scom = new SleepCommand(cuid, e, command);
  e->commands.push(scom);
  return true;
}

bool DownloadCommand::prepareForNextSegment() {
  req->resetUrl();
  if (!e->segmentMan->finished()) {
    return AbstractCommand::prepareForRetry();
  } else {
    return true;
  }
}
