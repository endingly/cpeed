#include "HttpRequestCommand.hpp"

#include "HttpConnection.hpp"
#include "HttpInitiateConnectionCommand.hpp"
#include "HttpResponseCommand.hpp"
#include "Socket.hpp"
#include "Util.hpp"

HttpRequestCommand::HttpRequestCommand(int cuid, Request* req, DownloadEngine* e, Socket* s)
    : AbstractCommand(cuid, req, e, s) {
  AbstractCommand::checkSocketIsWritable = true;
  e->deleteSocketForReadCheck(socket);
  e->addSocketForWriteCheck(socket);
}

HttpRequestCommand::~HttpRequestCommand() {}

bool HttpRequestCommand::executeInternal(Segment seg) {
  socket->setNonBlockingMode();
  HttpConnection httpConnection(cuid, socket, e->option, e->logger);
  // set seg to request in order to remember the request range
  req->seg = seg;
  httpConnection.sendRequest(req, seg);

  HttpResponseCommand* command = new HttpResponseCommand(cuid, req, e, socket);
  e->commands.push(command);
  return true;
}
