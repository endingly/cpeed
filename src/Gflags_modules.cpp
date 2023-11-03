#include "Gflags_modules.hpp"

#include <Util.hpp>

DEFINE_string(dir, "./", "The directory to store downloaded file.");
DEFINE_string(out, "./", "The file name for downloaded file.");
DEFINE_string(log, "-",
              "The file path to store log. If '-' is specified,log is written to stdout.");
DEFINE_bool(daemon, false, "Run as daemon.");
DEFINE_int32(split, 1,
             "Download a file using s connections. s must be between 1 and 5. If this option is "
             "specified the first URL is used, and the other URLs are ignored.");
DEFINE_string(http_proxy, "", "Use HTTP proxy server. This affects to all URLs.");
DEFINE_string(http_user, "", "Set HTTP user. This affects to all URLs.");
DEFINE_string(http_passwd, "", "Set HTTP password. This affects to all URLs.");
DEFINE_string(http_proxy_user, "", "Set HTTP proxy user. This affects to all URLs");
DEFINE_string(http_proxy_passwd, "", "Set HTTP proxy password. This affects to all URLs.");
DEFINE_string(http_auth_scheme, "",
              "Set HTTP authentication scheme. Currently, BASIC is the only supported scheme.");

bool ValidateSplit(const char* flagname, int32_t value) {
  if (value > 0 && value <= 5) {
    return true;
  }
  printf("Invalid value for --%s: %d\n", flagname, (int)value);
  return false;
}
DEFINE_validator(split, &ValidateSplit);

bool ValidateHttpProxy(const char* flagname, const std::string& value) {
  if (value.empty()) {
    return true;
  }
  std::pair<std::string, std::string> hp;
  Util::split(hp, value, ':');
  auto port = strtol(hp.second.c_str(), NULL, 10);
  if (!hp.first.empty() || port > 0 && port < 65536) {
    return true;
  }

  printf("Invalid value for --%s: %s\n", flagname, value.c_str());
  return false;
}
DEFINE_validator(http_proxy, &ValidateHttpProxy);

bool ValidateHttpAuthScheme(const char* flagname, const std::string& value) {
  if (value.empty() || value == "BASIC") {
    return true;
  }
  printf("Currently, supported authentication scheme is BASIC. Invalid value for --%s: %s\n",
         flagname, value.c_str());
  return false;
}
