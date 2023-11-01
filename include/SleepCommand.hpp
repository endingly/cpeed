#pragma once

#include <sys/time.h>

#include "Command.hpp"
#include "DownloadEngine.hpp"

class SleepCommand : public Command {
 private:
  DownloadEngine* engine;
  Command*        nextCommand;
  struct timeval  checkPoint;

 public:
  SleepCommand(int cuid, DownloadEngine* e, Command* nextCommand);
  ~SleepCommand();
  bool execute();
};
