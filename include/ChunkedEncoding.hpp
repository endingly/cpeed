#pragma once

#include "TransferEncoding.hpp"

/// @brief 分片编码
class ChunkedEncoding : public TransferEncoding {
 private:
  enum STATE { READ_SIZE, READ_DATA, FINISH };
  long int chunkSize;
  int      state;
  char*    strbuf;
  int      strbufSize;

  int  readChunkSize(char** pp);
  int  readData(char** pp, char* buf, int& len, int maxlen);
  void addBuffer(const char* inbuf, int inlen);
  int  readDataEOL(char** pp);

 public:
  ChunkedEncoding();
  ~ChunkedEncoding();

  void init();
  void inflate(char* outbuf, int& outlen, const char* inbuf, int inlen);
  bool finished();
  void end();
};
