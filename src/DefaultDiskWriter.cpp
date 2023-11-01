#include "DefaultDiskWriter.hpp"

#include <errno.h>
#include <unistd.h>

#include <cstring>

#include "DlAbortException.hpp"
using std::string;

DefaultDiskWriter::DefaultDiskWriter() : AbstractDiskWriter() {}

DefaultDiskWriter::~DefaultDiskWriter() {}

void DefaultDiskWriter::initAndOpenFile(string filename) { createFile(filename); }

void DefaultDiskWriter::writeData(const char* data, int len, long long int offset) {
  if (offset != lseek(fd, offset, SEEK_SET)) {
    throw new DlAbortException(strerror(errno));
  }
  writeDataInternal(data, len);
}
