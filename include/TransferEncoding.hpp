#pragma once

class TransferEncoding {
 public:
  virtual ~TransferEncoding() {}
  virtual void init()                                                           = 0;
  virtual void inflate(char* outbuf, int& outlen, const char* inbuf, int inlen) = 0;
  virtual bool finished()                                                       = 0;
  virtual void end()                                                            = 0;
};
