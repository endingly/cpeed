#pragma once

#include "AbstractDiskWriter.hpp"

class PreAllocationDiskWriter : public AbstractDiskWriter {
 private:
  unsigned long int size;

 public:
  PreAllocationDiskWriter(unsigned long int size);
  ~PreAllocationDiskWriter();

  void initAndOpenFile(std::string filename);

  void writeData(const char* data, int len, unsigned long int position);
};
