
#include "AbstractDiskWriter.hpp"

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cassert>
#include <cstring>

#include "DlAbortException.hpp"
#include "File.hpp"
using std::string;

AbstractDiskWriter::AbstractDiskWriter() : fd(0) {}

AbstractDiskWriter::~AbstractDiskWriter() {
  if (fd != 0) {
    close(fd);
  }
}

void AbstractDiskWriter::closeFile() {
  if (fd != 0) {
    close(fd);
    fd = 0;
  }
}

void AbstractDiskWriter::openExistingFile(string filename) {
  File f(filename);
  if (!f.isFile()) {
    throw new DlAbortException(strerror(errno));
  }

  if ((fd = open(filename.c_str(), O_WRONLY, S_IRUSR | S_IWUSR)) < 0) {
    throw new DlAbortException(strerror(errno));
  }
}

void AbstractDiskWriter::createFile(string filename, int addFlags) {
  // TODO proper filename handling needed
  assert(filename.size());
  //   if(filename.empty()) {
  //     filename = "index.html";
  //   }
  if ((fd = open(filename.c_str(), O_CREAT | O_WRONLY | O_TRUNC | addFlags, S_IRUSR | S_IWUSR)) <
      0) {
    throw new DlAbortException(strerror(errno));
  }
}

void AbstractDiskWriter::writeDataInternal(const char* data, int len) {
  if (write(fd, data, len) < 0) {
    throw new DlAbortException(strerror(errno));
  }
}
