#include "CookieBox.hpp"

#include <cstring>

#include "Util.hpp"

using std::string, std::vector, std::pair;

CookieBox::CookieBox() {}

CookieBox::~CookieBox() {}

void CookieBox::add(const Cookie& cookie) { cookies.push_back(cookie); }

void CookieBox::add(string cookieStr) {
  Cookie c;
  parse(c, cookieStr);
  cookies.push_back(c);
}

void CookieBox::setField(Cookie& cookie, string name, string value) const {
  if (name.size() == string("secure").size() && strcasecmp(name.c_str(), "secure") == 0) {
    cookie.secure = true;
  } else if (name.size() == string("domain").size() && strcasecmp(name.c_str(), "domain") == 0) {
    cookie.domain = value;
  } else if (name.size() == string("path").size() && strcasecmp(name.c_str(), "path") == 0) {
    cookie.path = value;
  } else if (name.size() == string("expires").size() && strcasecmp(name.c_str(), "expires") == 0) {
    cookie.expires = value;
  } else {
    cookie.name  = name;
    cookie.value = value;
  }
}

void CookieBox::parse(Cookie& cookie, string cookieStr) const {
  cookie.clear();
  vector<string> terms;
  Util::slice(terms, cookieStr, ';');
  for (vector<string>::iterator itr = terms.begin(); itr != terms.end(); itr++) {
    pair<string, string> nv;
    Util::split(nv, *itr, '=');
    setField(cookie, nv.first, nv.second);
  }
}

vector<Cookie> CookieBox::criteriaFind(string host, string dir, bool secure) const {
  vector<Cookie> result;
  for (vector<Cookie>::const_iterator itr = cookies.begin(); itr != cookies.end(); itr++) {
    const Cookie& c = *itr;
    if ((secure || !c.secure && !secure) && c.domain.size() <= host.size() &&
        c.path.size() <= dir.size() &&
        c.domain.compare(0, c.domain.size(), host, host.size() - c.domain.size(),
                         c.domain.size()) == 0 &&
        c.path.compare(0, c.path.size(), dir, 0, c.path.size()) == 0) {
      // TODO we currently ignore expire date.
      result.push_back(c);
    }
  }
  return result;
}
