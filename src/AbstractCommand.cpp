#include "AbstractCommand.hpp"

#include <sys/time.h>

#include "DlAbortException.hpp"
#include "DlRetryException.hpp"
#include "InitiateConnectionCommandFactory.hpp"
#include "Message.hpp"
#include "Util.hpp"

#define TIMEOUT_SEC 5

AbstractCommand::AbstractCommand(int cuid, Request* req, DownloadEngine* e, Socket* s)
    : Command(cuid), req(req), e(e), checkSocketIsReadable(false), checkSocketIsWritable(false) {
  if (s != NULL) {
    socket = new Socket(*s);
    e->addSocketForReadCheck(socket);
  } else {
    socket = NULL;
  }
  this->checkPoint.tv_sec  = 0;
  this->checkPoint.tv_usec = 0;
}

AbstractCommand::~AbstractCommand() {
  e->deleteSocketForReadCheck(socket);
  e->deleteSocketForWriteCheck(socket);
  if (socket != NULL) {
    delete (socket);
  }
}

void AbstractCommand::updateCheckPoint() { gettimeofday(&checkPoint, NULL); }

bool AbstractCommand::isTimeoutDetected() {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (checkPoint.tv_sec == 0 && checkPoint.tv_usec == 0) {
    return false;
  } else {
    long long int elapsed = Util::difftv(now, checkPoint);
    if (elapsed >= TIMEOUT_SEC * 1000000) {
      return true;
    } else {
      return false;
    }
  }
}

bool AbstractCommand::execute() {
  try {
    if (checkSocketIsReadable && !socket->isReadable(0) ||
        checkSocketIsWritable && !socket->isWritable(0)) {
      if (isTimeoutDetected()) {
        throw new DlRetryException(EX_TIME_OUT);
      }
      updateCheckPoint();
      e->commands.push(this);
      return false;
    }
    updateCheckPoint();
    Segment seg = {0, 0, 0, false};
    if (e->segmentMan->downloadStarted) {
      // get segment information in order to set Range header.
      if (!e->segmentMan->getSegment(seg, cuid)) {
        // no segment available
        e->logger->info(MSG_NO_SEGMENT_AVAILABLE, cuid);
        return true;
      }
    }
    return executeInternal(seg);
  } catch (DlAbortException* err) {
    e->logger->error(MSG_DOWNLOAD_ABORTED, err);
    onError(err);
    delete (err);
    req->resetUrl();
    return true;
  } catch (DlRetryException* err) {
    e->logger->error(MSG_RESTARTING_DOWNLOAD, err);
    onError(err);
    delete (err);
    req->resetUrl();
    req->addRetryCount();
    if (req->noMoreRetry()) {
      e->logger->error(MSG_MAX_RETRY);
      return true;
    } else {
      return prepareForRetry();
    }
  }
}

bool AbstractCommand::prepareForRetry() {
  e->commands.push(InitiateConnectionCommandFactory::createInitiateConnectionCommand(cuid, req, e));
  return true;
}

void AbstractCommand::onError(Exception* e) {}
