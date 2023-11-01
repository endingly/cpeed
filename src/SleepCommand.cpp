#include "SleepCommand.hpp"

#include "Util.hpp"

#define WAIT_SEC 5

SleepCommand::SleepCommand(int cuid, DownloadEngine* e, Command* nextCommand)
    : Command(cuid), engine(e), nextCommand(nextCommand) {
  gettimeofday(&checkPoint, NULL);
}

SleepCommand::~SleepCommand() {
  if (nextCommand != NULL) {
    delete (nextCommand);
  }
}

bool SleepCommand::execute() {
  struct timeval now;
  gettimeofday(&now, NULL);
  if (Util::difftv(now, checkPoint) >= WAIT_SEC * 1000000) {
    engine->commands.push(nextCommand);
    nextCommand = NULL;
    return true;
  } else {
    engine->commands.push(this);
    return false;
  }
}
