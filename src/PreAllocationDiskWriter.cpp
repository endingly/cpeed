#include "PreAllocationDiskWriter.hpp"

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "DlAbortException.hpp"
using std::string;

PreAllocationDiskWriter::PreAllocationDiskWriter(unsigned long int size)
    : AbstractDiskWriter(), size(size) {}

PreAllocationDiskWriter::~PreAllocationDiskWriter() {}

void PreAllocationDiskWriter::initAndOpenFile(string filename) {
  createFile(filename, O_DIRECT);

  int   pageSize = getpagesize();
  char* buf;
  int   size = pageSize * 4;
  int   rt   = posix_memalign((void**)&buf, pageSize, size);
  if (rt != 0) {
    throw new DlAbortException(strerror(rt));
  }
  memset(buf, 0, size);
  int x = size / 4096;
  int r = size % 4096;
  for (int i = 0; i < x; i++) {
    if (write(fd, buf, size) < 0) {
      free(buf);
      throw new DlAbortException(strerror(errno));
    }
  }
  free(buf);
  closeFile();
  openExistingFile(filename);
  char cbuf[4096];
  memset(cbuf, 0, sizeof(cbuf));
  if (write(fd, cbuf, r) < 0) {
    throw new DlAbortException(strerror(errno));
  }
}

void PreAllocationDiskWriter::writeData(const char* data, int len, unsigned long int offset) {
  int x = lseek(fd, offset, SEEK_SET);
  // TODO check the return value of write
  writeDataInternal(data, len);
}
