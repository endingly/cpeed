#pragma once

#include <map>
#include <string>

#include "Logger.hpp"
#include "Option.hpp"
#include "Request.hpp"
#include "SegmentManager.hpp"
#include "Socket.hpp"

typedef std::multimap<std::string, std::string> HttpHeader;

class HttpConnection {
 private:
  std::string        getHost(const std::string& host, int port);
  std::string        createRequest(const Request* req, const Segment& segment);
  bool          useProxy();
  bool          useProxyAuth();
  bool          useBasicAuth();
  int           cuid;
  Socket*       socket;
  const Option* option;
  Logger*       logger;

 public:
  HttpConnection(int cuid, Socket* socket, const Option* op, Logger* logger);

  void sendRequest(const Request* req, const Segment& segment);
  void sendProxyRequest(const Request* req);
  int  receiveResponse(HttpHeader& headers);
};
