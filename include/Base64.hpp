#pragma once
#include <string>

/// @brief base 64 编码的实现类，只包含静态方法
class Base64 {
 private:
  /// @brief 编码字符串的一部分
  /// @param subplain
  /// @return
  static std::string part_encode(const std::string& subplain);
  /// @brief 解码字符串一部分
  /// @param subCrypted
  /// @return
  static std::string part_decode(const std::string& subCrypted);
  /// @brief 获取单个字符对应的base64编码
  /// @param ch
  /// @return
  static char getValue(char ch);

 public:
  /// @brief 编码字符串
  /// @param plain
  /// @return
  static std::string encode(std::string plain);
  /// @brief 解码字符串
  /// @param crypted
  /// @return
  static std::string decode(std::string crypted);
};
