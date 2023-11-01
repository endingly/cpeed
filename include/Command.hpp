#pragma once

/// @brief 命令基类
class Command {
 protected:
  int cuid;

 public:
  Command(int cuid) : cuid(cuid) {}
  virtual ~Command() {}
  virtual bool execute() = 0;

  int getCuid() const { return cuid; }
};
