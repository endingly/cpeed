#include "Socket.hpp"

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>

#include "DlAbortException.hpp"
#include "DlRetryException.hpp"

using std::string;

Socket::Socket() : sockfd(-1) {}

Socket::Socket(const Socket& s) { sockfd = dup(s.sockfd); }

Socket::~Socket() { closeConnection(); }

Socket& Socket::operator=(const Socket& s) {
  if (this != &s) {
    closeConnection();
    sockfd = dup(s.sockfd);
  }
  return *this;
}

void Socket::establishConnection(string host, int port) {
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd >= 0) {
    socklen_t sockopt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &sockopt, sizeof(int)) < 0) {
      close(sockfd);
      sockfd = -1;
      throw new DlAbortException(strerror(errno));
    }
  } else {
    throw new DlAbortException(strerror(errno));
  }

  struct sockaddr_in sockaddr;
  memset((char*)&sockaddr, 0, sizeof(sockaddr));
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port   = htons(port);
  if (inet_aton(host.c_str(), &sockaddr.sin_addr)) {
    // ok
  } else {
    struct addrinfo ai;
    ai.ai_flags    = 0;
    ai.ai_family   = PF_INET;
    ai.ai_socktype = SOCK_STREAM;
    ai.ai_protocol = 0;
    ai.ai_addr     = (struct sockaddr*)&sockaddr;
    struct addrinfo* res;
    int              ec;
    if ((ec = getaddrinfo(host.c_str(), NULL, &ai, &res)) != 0) {
      throw new DlAbortException(gai_strerror(ec));
    }
    sockaddr.sin_addr = ((struct sockaddr_in*)res->ai_addr)->sin_addr;
    freeaddrinfo(res);
  }
  // make socket non-blocking mode
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
  if (connect(sockfd, (struct sockaddr*)&sockaddr, (socklen_t)sizeof(sockaddr)) == -1 &&
      errno != EINPROGRESS) {
    throw new DlAbortException(strerror(errno));
  }
}

void Socket::setNonBlockingMode() {
  int flags = fcntl(sockfd, F_GETFL, 0);
  fcntl(sockfd, F_SETFL, flags & ~O_NONBLOCK);
}

void Socket::closeConnection() {
  if (sockfd != -1) {
    close(sockfd);
    sockfd = -1;
  }
}

bool Socket::isWritable(int timeout) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);

  struct timeval tv;
  tv.tv_sec  = timeout;
  tv.tv_usec = 0;

  int r = select(sockfd + 1, NULL, &fds, NULL, &tv);
  if (r == 1) {
    return true;
  } else if (r == 0) {
    // time out
    return false;
  } else {
    throw new DlRetryException(strerror(errno));
  }
}

bool Socket::isReadable(int timeout) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(sockfd, &fds);

  struct timeval tv;
  tv.tv_sec  = timeout;
  tv.tv_usec = 0;

  int r = select(sockfd + 1, &fds, NULL, NULL, &tv);
  if (r == 1) {
    return true;
  } else if (r == 0) {
    // time out
    return false;
  } else {
    throw new DlRetryException(strerror(errno));
  }
}

void Socket::writeData(const char* data, int len, int timeout) {
  if (!isWritable(timeout) || send(sockfd, data, (size_t)len, 0) != len) {
    throw new DlRetryException(strerror(errno));
  }
}

void Socket::readData(char* data, int& len, int timeout) {
  if (!isReadable(timeout) || (len = recv(sockfd, data, (size_t)len, 0)) < 0) {
    throw new DlRetryException(strerror(errno));
  }
}

void Socket::peekData(char* data, int& len, int timeout) {
  if (!isReadable(timeout) || (len = recv(sockfd, data, (size_t)len, MSG_PEEK)) < 0) {
    throw new DlRetryException(strerror(errno));
  }
}
