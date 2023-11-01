#pragma once

#include <string>
#include <vector>

class Cookie {
 public:
  std::string name;
  std::string value;
  std::string expires;
  std::string path;
  std::string domain;
  bool        secure;

 public:
  Cookie(std::string name, std::string value, std::string expires, std::string path,
         std::string domain, bool secure)
      : name(name), value(value), expires(expires), path(path), domain(domain), secure(secure) {}
  Cookie() : secure(false) {}
  ~Cookie() {}
  std::string toString() const { return name + "=" + value; }
  void        clear() {
    name = value = expires = path = domain = "";
    secure                                 = false;
  }
};

class CookieBox {
 private:
  std::vector<Cookie> cookies;
  void                setField(Cookie& cookie, std::string name, std::string value) const;

 public:
  CookieBox();
  ~CookieBox();
  void                clear();
  void                add(const Cookie& cookie);
  void                add(std::string cookieStr);
  void                parse(Cookie& cookie, std::string cookieStr) const;
  std::vector<Cookie> criteriaFind(std::string host, std::string dir, bool secure) const;
};
