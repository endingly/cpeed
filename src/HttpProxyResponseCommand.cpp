#include "HttpProxyResponseCommand.hpp"

#include "DlRetryException.hpp"
#include "HttpConnection.hpp"
#include "HttpRequestCommand.hpp"
#include "Message.hpp"

HttpProxyResponseCommand::HttpProxyResponseCommand(int cuid, Request* req, DownloadEngine* e,
                                                   Socket* s)
    : AbstractCommand(cuid, req, e, s) {
  AbstractCommand::checkSocketIsReadable = true;
}

HttpProxyResponseCommand::~HttpProxyResponseCommand() {}

bool HttpProxyResponseCommand::executeInternal(Segment segment) {
  HttpHeader     headers;
  HttpConnection httpConnection(cuid, socket, e->option, e->logger);
  int            status = httpConnection.receiveResponse(headers);
  if (status != 200) {
    throw new DlRetryException(EX_PROXY_CONNECTION_FAILED);
  }
  HttpRequestCommand* command = new HttpRequestCommand(cuid, req, e, socket);
  e->commands.push(command);
  return true;
}
