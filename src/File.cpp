
#include "File.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

using std::string;

File::File(string name) : name(name) {}

File::~File() {}

int File::fillStat(struct stat& fstat) { return stat(name.c_str(), &fstat); }

bool File::exists() {
  struct stat fstat;
  return fillStat(fstat) == 0;
}

bool File::isFile() {
  struct stat fstat;
  if (fillStat(fstat) < 0) {
    return false;
  }
  return S_ISREG(fstat.st_mode) == 1;
}

bool File::isDir() {
  struct stat fstat;
  if (fillStat(fstat) < 0) {
    return false;
  }
  return S_ISDIR(fstat.st_mode) == 1;
}

bool File::remove() {
  if (isFile()) {
    return unlink(name.c_str()) == 0;
  } else if (isDir()) {
    return rmdir(name.c_str()) == 0;
  } else {
    return false;
  }
}
