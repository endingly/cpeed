#pragma once
#include <map>
#include <string>

#include "CookieBox.hpp"
#include "Segment.hpp"

#define SAFE_CHARS             \
  "abcdefghijklmnopqrstuvwxyz" \
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ" \
  "0123456789"                 \
  ":/?[]@"                     \
  "!$&'()*+,;="                \
  "-._~"                       \
  "%"

class Request {
 private:
  std::string                url;
  std::string                currentUrl;
  std::string                protocol;
  std::string                host;
  int                        port;
  std::string                dir;
  std::string                file;
  std::map<std::string, int> defaultPorts;
  int                        retryCount;
  bool                       parseUrl(std::string url);

 public:
  Segment    seg;
  CookieBox* cookieBox;

 public:
  Request();
  virtual ~Request();

  // Parses URL and sets url, host, port, dir, file fields.
  // Returns true if parsing goes successful, otherwise returns false.
  bool setUrl(std::string url);
  // Parses URL and sets host, port, dir, file fields.
  // url field are not altered by this method.
  // Returns true if parsing goes successful, otherwise returns false.
  bool redirectUrl(std::string url);
  bool resetUrl();
  void resetRetryCount();
  void addRetryCount();
  bool noMoreRetry();

  std::string getUrl() const { return url; }
  std::string getCurrentUrl() const { return currentUrl; }
  std::string getProtocol() const { return protocol; }
  std::string getHost() const { return host; }
  int         getPort() const { return port; }
  std::string getDir() const { return dir; }
  std::string getFile() const { return file; }
};
