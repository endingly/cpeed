#pragma once

#include "AbstractCommand.hpp"
#include "HttpConnection.hpp"

class HttpResponseCommand : public AbstractCommand {
 private:
  void checkResponse(int status, const Segment& segment);
  bool handleRedirect(std::string url, const HttpHeader& headers);
  bool handleDefaultEncoding(const HttpHeader& headers);
  bool handleOtherEncoding(std::string transferEncoding, const HttpHeader& headers);
  void createHttpDownloadCommand(std::string transferEncoding = "");
  void retrieveCookie(const HttpHeader& headers);

 protected:
  bool executeInternal(Segment segment);

 public:
  HttpResponseCommand(int cuid, Request* req, DownloadEngine* e, Socket* s);
  ~HttpResponseCommand();
};
