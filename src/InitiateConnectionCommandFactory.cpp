
#include "InitiateConnectionCommandFactory.hpp"

#include "HttpInitiateConnectionCommand.hpp"

Command* InitiateConnectionCommandFactory::createInitiateConnectionCommand(int cuid, Request* req,
                                                                           DownloadEngine* e) {
  if (req->getProtocol() == "http") {
    return new HttpInitiateConnectionCommand(cuid, req, e);
  } else {
    // these protocols are not supported yet
    return NULL;
  }
}
