#pragma once

#include <string>
#include <vector>

using namespace std;

class Cookie {
 public:
  string name;
  string value;
  string expires;
  string path;
  string domain;
  bool   secure;

 public:
  Cookie(string name, string value, string expires, string path, string domain, bool secure)
      : name(name), value(value), expires(expires), path(path), domain(domain), secure(secure) {}
  Cookie() : secure(false) {}
  ~Cookie() {}
  string toString() const { return name + "=" + value; }
  void   clear() {
    name = value = expires = path = domain = "";
    secure                                 = false;
  }
};

class CookieBox {
 private:
  vector<Cookie> cookies;
  void           setField(Cookie& cookie, string name, string value) const;

 public:
  CookieBox();
  ~CookieBox();
  void           clear();
  void           add(const Cookie& cookie);
  void           add(string cookieStr);
  void           parse(Cookie& cookie, string cookieStr) const;
  vector<Cookie> criteriaFind(string host, string dir, bool secure) const;
};
