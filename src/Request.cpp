#include "Request.hpp"

#include "Util.hpp"

using std::string, std::map, std::pair;
#define MAX_RETRY_COUNT 5

Request::Request() : port(0), retryCount(0) {
  defaultPorts["http"] = 80;
  seg.sp               = 0;
  seg.ep               = 0;
  seg.ds               = 0;
  seg.finish           = false;
  cookieBox            = new CookieBox();
}

Request::~Request() {}

bool Request::setUrl(string url) {
  this->url = url;
  return parseUrl(url);
}

bool Request::resetUrl() { return setUrl(url); }

bool Request::redirectUrl(string url) { return parseUrl(url); }

bool Request::parseUrl(string url) {
  currentUrl = url;
  host       = "";
  port       = 0;
  dir        = "";
  file       = "";
  if (url.find_first_not_of(SAFE_CHARS) != string::npos) {
    return false;
  }
  unsigned int hp = url.find("://");
  if (hp == string::npos) return false;
  protocol = url.substr(0, hp);
  int defPort;
  if ((defPort = defaultPorts[protocol]) == 0) {
    return false;
  }
  hp += 3;
  if (url.size() <= hp) return false;
  unsigned int hep = url.find("/", hp);
  if (hep == string::npos) {
    hep = url.size();
  }
  pair<string, string> hostAndPort;
  Util::split(hostAndPort, url.substr(hp, hep - hp), ':');
  host = hostAndPort.first;
  if (hostAndPort.second != "") {
    // TODO rewrite this using strtoul function. If strtoul fails,
    // return false.
    port = (int)strtol(hostAndPort.second.c_str(), NULL, 10);
    if (!(0 < port && port <= 65535)) {
      return false;
    }
  } else {
    // If port is not specified, then we leave it 0.
    port = defPort;
  }
  unsigned int direp = url.find_last_of("/");
  if (direp == string::npos || direp <= hep) {
    dir   = "/";
    direp = hep;
  } else {
    dir = url.substr(hep, direp - hep);
  }
  if (url.size() > direp + 1) {
    file = url.substr(direp + 1);
  }
  return true;
}

void Request::resetRetryCount() { this->retryCount = 0; }

void Request::addRetryCount() { retryCount++; }

bool Request::noMoreRetry() { return retryCount >= MAX_RETRY_COUNT; }
