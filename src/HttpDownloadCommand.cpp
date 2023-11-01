#include "HttpDownloadCommand.hpp"

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>

#include "ChunkedEncoding.hpp"
#include "DlRetryException.hpp"
#include "HttpRequestCommand.hpp"
#include "SleepCommand.hpp"
#include "Util.hpp"

using std::map, std::string;

HttpDownloadCommand::HttpDownloadCommand(int cuid, Request* req, DownloadEngine* e, Socket* socket)
    : DownloadCommand(cuid, req, e, socket) {
  ChunkedEncoding* ce          = new ChunkedEncoding();
  transferEncodings["chunked"] = ce;
}

HttpDownloadCommand::~HttpDownloadCommand() {
  for (map<string, TransferEncoding*>::iterator itr = transferEncodings.begin();
       itr != transferEncodings.end(); itr++) {
    delete ((*itr).second);
  }
}

TransferEncoding* HttpDownloadCommand::getTransferEncoding(string name) {
  return transferEncodings[name];
}
