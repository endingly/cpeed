
#include "HttpProxyRequestCommand.hpp"

#include "HttpConnection.hpp"
#include "HttpProxyResponseCommand.hpp"

HttpProxyRequestCommand::HttpProxyRequestCommand(int cuid, Request* req, DownloadEngine* e,
                                                 Socket* s)
    : AbstractCommand(cuid, req, e, s) {
  AbstractCommand::checkSocketIsWritable = true;
  e->deleteSocketForReadCheck(socket);
  e->addSocketForWriteCheck(socket);
}

HttpProxyRequestCommand::~HttpProxyRequestCommand() {}

bool HttpProxyRequestCommand::executeInternal(Segment segment) {
  socket->setNonBlockingMode();
  HttpConnection httpConnection(cuid, socket, e->option, e->logger);
  httpConnection.sendProxyRequest(req);

  HttpProxyResponseCommand* command = new HttpProxyResponseCommand(cuid, req, e, socket);
  e->commands.push(command);
  return true;
}
