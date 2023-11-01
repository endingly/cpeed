#pragma once

#include "AbstractCommand.hpp"

class HttpInitiateConnectionCommand : public AbstractCommand {
 private:
  bool useProxy();

 protected:
  /**
   * Connect to the server.
   * This method just send connection request to the server.
   * Using nonblocking mode of socket, this funtion returns immediately
   * after send connection packet to the server.
   * Whether or not the connection is established successfully is
   * evaluated by RequestCommand.
   */
  bool executeInternal(Segment segment);

 public:
  HttpInitiateConnectionCommand(int cuid, Request* req, DownloadEngine* e);
  ~HttpInitiateConnectionCommand();
};
