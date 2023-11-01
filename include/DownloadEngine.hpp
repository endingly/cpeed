#pragma once

#include <queue>
#include <vector>

#include "Command.hpp"
#include "DiskWriter.hpp"
#include "Logger.hpp"
#include "Option.hpp"
#include "SegmentManager.hpp"
#include "Socket.hpp"

class DownloadEngine {
 private:
  void            waitData();
  vector<Socket*> rsockets;
  vector<Socket*> wsockets;

  bool addSocket(vector<Socket*>& sockets, Socket* socket);
  bool deleteSocket(vector<Socket*>& sockets, Socket* socket);

 public:
  bool            noWait;
  queue<Command*> commands;
  SegmentMan*     segmentMan;
  DiskWriter*     diskWriter;
  Logger*         logger;
  Option*         option;

  DownloadEngine();
  ~DownloadEngine();

  void run();

  bool addSocketForReadCheck(Socket* socket);
  bool deleteSocketForReadCheck(Socket* socket);
  bool addSocketForWriteCheck(Socket* socket);
  bool deleteSocketForWriteCheck(Socket* socket);
};

#endif  // _D_DOWNLOAD_ENGINE_H_
