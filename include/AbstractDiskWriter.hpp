
#pragma once

#include "DiskWriter.hpp"

class AbstractDiskWriter : public DiskWriter {
 protected:
  int fd;

  void createFile(std::string filename, int addFlags = 0);

  void writeDataInternal(const char* data, int len);

 public:
  AbstractDiskWriter();
  virtual ~AbstractDiskWriter();

  void closeFile();

  void openExistingFile(std::string filename);
};
