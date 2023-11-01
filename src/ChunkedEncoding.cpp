#include "ChunkedEncoding.hpp"

#include <cerrno>
#include <climits>
#include <cstring>

#include "DlAbortException.hpp"
#include "Message.hpp"

#define MAX_BUFSIZE 8192

ChunkedEncoding::ChunkedEncoding() {
  strbufSize = 4096;
  strbuf     = new char[strbufSize];
  strbuf[0]  = '\0';
  state      = READ_SIZE;
  chunkSize  = 0;
}

ChunkedEncoding::~ChunkedEncoding() {
  if (strbuf != NULL) {
    delete[] strbuf;
  }
}

void ChunkedEncoding::init() {}

bool ChunkedEncoding::finished() { return state == FINISH ? true : false; }

void ChunkedEncoding::end() {}

void ChunkedEncoding::inflate(char* outbuf, int& outlen, const char* inbuf, int inlen) {
  addBuffer(inbuf, inlen);
  char* p    = strbuf;
  int   clen = 0;
  while (1) {
    if (state == READ_SIZE) {
      if (readChunkSize(&p) == 0) {
        if (chunkSize == 0) {
          state = FINISH;
        } else {
          state = READ_DATA;
        }
      } else {
        // chunk size is not fully received.
        break;
      }
    } else if (state == READ_DATA) {
      if (readData(&p, outbuf, clen, outlen) == 0) {
        state = READ_SIZE;
      } else {
        break;
      }
    } else {
      break;
    }
    // Was all bytes in strbuf examined?
    if (strbuf + strlen(strbuf) <= p) {
      break;
    }
  }
  if (strbuf + strlen(strbuf) <= p) {
    // make strbuf NULL-string
    strbuf[0] = '\0';
  } else {
    // copy string between [p, strbuf+strlen(strbuf)+1], +1 is for NULL
    // character.
    char* temp = new char[strbufSize];
    memcpy(temp, p, strbuf + strlen(strbuf) - p + 1);
    delete[] strbuf;
    strbuf = temp;
  }
  outlen = clen;
}

int ChunkedEncoding::readData(char** pp, char* buf, int& len, int maxlen) {
  if (buf + len == buf + maxlen) {
    return -1;
  }
  if (chunkSize == 0) {
    return readDataEOL(pp);
  }
  int wsize;
  if (strlen(*pp) < (unsigned long int)chunkSize) {
    wsize = strlen(*pp) <= (unsigned int)maxlen - len ? strlen(*pp) : maxlen - len;
  } else {
    wsize = chunkSize <= maxlen - len ? chunkSize : maxlen - len;
  }
  memcpy(buf + len, *pp, wsize);
  chunkSize -= wsize;
  len += wsize;
  *pp += wsize;

  if (chunkSize == 0) {
    return readDataEOL(pp);
  } else {
    return -1;
  }
}

int ChunkedEncoding::readDataEOL(char** pp) {
  if (strstr(*pp, "\r\n") == *pp) {
    *pp += 2;
    return 0;
  } else if (strlen(*pp) < 2) {
    return -1;
  } else {
    throw new DlAbortException(EX_INVALID_CHUNK_SIZE);
  }
}

// strbuf is NULL terminated string, and inlen is strlen(strbuf).
// therefore, strbuf[inlen] = '\0'
int ChunkedEncoding::readChunkSize(char** pp) {
  // we read chunk-size from *pp
  char* p = strstr(*pp, "\r\n");
  // \r\n is not found. Return -1
  if (p == NULL) {
    return -1;
  }
  // We ignore chunk-extension
  char* exsp = index(*pp, ';');
  if (exsp == NULL) {
    exsp = p;
  }
  // TODO check invalid characters in buffer
  chunkSize = strtol(*pp, NULL, 16);
  if (chunkSize < 0) {
    throw new DlAbortException(EX_INVALID_CHUNK_SIZE);
  } else if (errno == ERANGE && (chunkSize == LONG_MAX || chunkSize == LONG_MIN)) {
    throw new DlAbortException(strerror(errno));
  }
  *pp = p + 2;

  return 0;
}

void ChunkedEncoding::addBuffer(const char* inbuf, int inlen) {
  if (strlen(strbuf) + inlen >= (unsigned int)strbufSize) {
    if (strlen(strbuf) + inlen + 1 > MAX_BUFSIZE) {
      throw new DlAbortException(EX_TOO_LARGE_CHUNK, strlen(strbuf) + inlen + 1);
    }
    char* temp = new char[strlen(strbuf) + inlen + 1];
    memcpy(temp, strbuf, strlen(strbuf) + 1);
    delete[] strbuf;
    strbuf     = temp;
    strbufSize = strlen(strbuf);
  }
  int origlen = strlen(strbuf);
  memcpy(strbuf + origlen, inbuf, inlen);
  strbuf[origlen + inlen] = '\0';
}
