
#include "Option.hpp"
using std::string, std::map;
Option::Option() {}

Option::~Option() {}

void Option::put(const string& name, const string& value) { table[name] = value; }

bool Option::defined(const string& name) const { return table.count(name) == 1; }

string Option::get(const string& name) const {
  map<string, string>::const_iterator itr = table.find(name);
  if (itr == table.end()) {
    return "";
  } else {
    return (*itr).second;
  }
}

int Option::getAsInt(const string& name) const {
  map<string, string>::const_iterator itr = table.find(name);
  if (itr == table.end()) {
    return 0;
  } else {
    return (int)strtol((*itr).second.c_str(), NULL, 10);
  }
}
