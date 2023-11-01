
#include "HttpInitiateConnectionCommand.hpp"

#include "HttpProxyRequestCommand.hpp"
#include "HttpRequestCommand.hpp"
#include "Message.hpp"
#include "Util.hpp"

HttpInitiateConnectionCommand::HttpInitiateConnectionCommand(int cuid, Request* req,
                                                             DownloadEngine* e)
    : AbstractCommand(cuid, req, e) {}

HttpInitiateConnectionCommand::~HttpInitiateConnectionCommand() {}

bool HttpInitiateConnectionCommand::executeInternal(Segment segment) {
  socket = new Socket();
  // socket->establishConnection(...);
  Command* command;
  if (useProxy()) {
    e->logger->info(MSG_CONNECTING_TO_SERVER, cuid, e->option->get("http_proxy_host").c_str(),
                    e->option->getAsInt("http_proxy_port"));
    socket->establishConnection(e->option->get("http_proxy_host"),
                                e->option->getAsInt("http_proxy_port"));
    command = new HttpProxyRequestCommand(cuid, req, e, socket);

  } else {
    e->logger->info(MSG_CONNECTING_TO_SERVER, cuid, req->getHost().c_str(), req->getPort());
    socket->establishConnection(req->getHost(), req->getPort());
    command = new HttpRequestCommand(cuid, req, e, socket);
  }
  e->commands.push(command);
  return true;
}

bool HttpInitiateConnectionCommand::useProxy() {
  return e->option->defined("http_proxy_enabled") && e->option->get("http_proxy_enabled") == "true";
}
