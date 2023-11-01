#pragma once

#include "AbstractDiskWriter.hpp"

class DefaultDiskWriter : public AbstractDiskWriter {
 public:
  DefaultDiskWriter();
  ~DefaultDiskWriter();

  void initAndOpenFile(std::string filename);

  void writeData(const char* data, int len, long long int position);
};
