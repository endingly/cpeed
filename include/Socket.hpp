#pragma once

#include <string>

/// @brief 套接字封装类
class Socket {
 public:
  // socket 描述符
  int sockfd;

 public:
  Socket();
  Socket(const Socket& s);
  ~Socket();

  Socket& operator=(const Socket& s);

  /// @brief 通过主机名与端口号连接到服务器
  /// @details 此方法使套接字处于非阻塞模式.如果需要以阻塞模式运行,请在连接建立后调用 setNonBlockingMode()
  /// @param host
  /// @param port
  void establishConnection(std::string host, int port);

  void setNonBlockingMode();

  /// @brief 关闭套接字
  void closeConnection();

  /// @brief 测试套接字是否可写
  /// @param timeout 超时设置
  /// @return 如果可写返回 true, 否则返回 false
  bool isWritable(int timeout);

  /// @brief 测试套接字是否可读
  /// @param timeout 超时设置
  /// @return 如果可读返回 true, 否则返回 false
  bool isReadable(int timeout);

  /// @brief 向套接字写入数据
  /// @param data 字符数据
  /// @param len 长度
  /// @param timeout 超时时间，毫秒
  void writeData(const char* data, int len, int timeout = 5);

  // Reads up to len bytes from this socket.
  // data is a pointer pointing the first
  // byte of the data, which must be allocated before this method is called.
  // len is the size of the allocated memory. When this method returns
  // successfully, len is replaced by the size of the read data.

  /// @brief 读取套接字数据
  /// @details 如果成功， len 将被替换为读取数据的大小
  /// @param data 字符数据容器指针
  /// @param len 长度，字节
  /// @param timeout 超时时间，毫秒
  void readData(char* data, int& len, int timeout = 5);
  // Reads up to len bytes from this socket, but bytes are not removed from
  // this socket.

  /// @brief 提取套接字数据
  /// @details 不会将数据从套接字中删除
  /// @param data 字符数据容器指针
  /// @param len 长度，字节
  /// @param timeout 超时时间，毫秒
  void peekData(char* data, int& len, int timeout = 5);
};
