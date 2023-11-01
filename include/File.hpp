#pragma once

#include <string>

// TODO: 可以修改为 C++17 filesystem 库

/// @brief 表示一个文件或一个路径
class File {
 private:
  std::string name;

  /// @brief 填充 fstat 结构体
  /// @param fstat
  /// @return
  int fillStat(struct stat& fstat);

 public:
  File(std::string name);
  ~File();

  /// @brief 测试一个文件或者路径能否安全退出
  /// @return
  bool exists();

  /// @brief 测试一个文件是否是常规文件
  /// @return
  bool isFile();

  /// @brief 测试一个文件是否是常规目录
  /// @return
  bool isDir();

  /// @brief 删除一个文件或者路径，如果是路径，必须为空路径
  /// @return
  bool remove();
};
