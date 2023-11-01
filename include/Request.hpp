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
  string           url;
  string           currentUrl;
  string           protocol;
  string           host;
  int              port;
  string           dir;
  string           file;
  map<string, int> defaultPorts;
  int              retryCount;
  bool             parseUrl(string url);

 public:
  Segment    seg;
  CookieBox* cookieBox;

 public:
  Request();
  virtual ~Request();

  // Parses URL and sets url, host, port, dir, file fields.
  // Returns true if parsing goes successful, otherwise returns false.
  bool setUrl(string url);
  // Parses URL and sets host, port, dir, file fields.
  // url field are not altered by this method.
  // Returns true if parsing goes successful, otherwise returns false.
  bool redirectUrl(string url);
  bool resetUrl();
  void resetRetryCount();
  void addRetryCount();
  bool noMoreRetry();

  string getUrl() const { return url; }
  string getCurrentUrl() const { return currentUrl; }
  string getProtocol() const { return protocol; }
  string getHost() const { return host; }
  int    getPort() const { return port; }
  string getDir() const { return dir; }
  string getFile() const { return file; }
};

#endif  // _D_REQUEST_H_
