#pragma once
#include <sys/time.h>

#include <string>
#include <utility>
#include <vector>

#define STRTOLL(X) std::strtoll(X, (char**)NULL, 10);  // 字符串转 long

/// @brief 工具杂类
class Util {
 public:
  /// @brief 以某一个特定字符为限，将字符串拆分成一个字符串组，只拆分一次
  /// @param hp 返回结果
  /// @param src 源字符串
  /// @param delim 分界字符
  static void split(std::pair<std::string, std::string>& hp, std::string src, char delim);

  /// @brief 长整型转字符串
  /// @param value
  /// @param comma
  /// @return
  static std::string llitos(long long int value, bool comma = false);

  /// @brief 整形转字符串
  /// @param value
  /// @param comma
  /// @return
  static std::string itos(int value, bool comma = false);

  /// @brief 比较两个时间戳，返回其间隔的毫秒数
  /// @param tv1
  /// @param tv2
  /// @return 如果 tv1 < tv2 则返回 0
  static long long int difftv(struct timeval tv1, struct timeval tv2);

  /// @brief 以某一个特定字符为限，将字符串拆分成一个字符串数组
  /// @param result 返回的结果
  /// @param src 源字符串
  /// @param delim 分界字符
  static void slice(std::vector<std::string>& result, std::string src, char delim);

  /// @brief 消除字符串的空格
  /// @param src
  /// @return
  static std::string trim(std::string src);
};
