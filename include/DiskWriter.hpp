#pragma once

#include <string>

/// @brief 接口 用于将数据写入到磁盘接口
class DiskWriter {
 public:
  virtual ~DiskWriter() {}

  /// @brief 初始化一个文件并以写入模式打开
  /// @param filename 指定要打开的文件路径
  virtual void initAndOpenFile(std::string filename) = 0;

  /// @brief 关闭文件
  virtual void closeFile() = 0;

  /// @brief 以写入模式打开一个存在的文件
  /// @details 如果文件不存在，可能会抛出异常
  /// @param filename 指定要打开的文件路径
  virtual void openExistingFile(std::string filename) = 0;

  /// @brief 向文件写入二进制流
  /// @param data 数据指针
  /// @param len 要写入的长度，字节
  /// @param position 二进制流的位置
  virtual void writeData(const char* data, int len, long long int position = 0) = 0;
};
