// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "Socket.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <string>

#ifdef _WIN32
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#define SHUT_RDWR SD_BOTH
typedef int socklen_t;
#else
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#endif

/*! ignore if not supported */
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

namespace rkcommon {
  const socket_t OSP_INVALID_SOCKET = -1;

#ifdef _WIN32
  struct WinSockContext
  {
    WinSockContext()
    {
      WSADATA wsadata;
      if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        throw std::runtime_error("WinSock initialization failed");
      }
    }
    ~WinSockContext()
    {
      WSACleanup();
    }
  };

  static std::unique_ptr<WinSockContext> winsock = nullptr;

  void initialize()
  {
    static std::atomic<bool> initialized = false;
    static std::mutex initMutex;
    if (!initialized) {
      std::lock_guard<std::mutex> lock(initMutex);
      if (!winsock) {
        winsock = std::make_unique<WinSockContext>();
      }
      initialized = true;
    }
  }
#else
  void initialize() {}
#endif

  struct AutoCloseSocket
  {
    socket_t sock;
    AutoCloseSocket(socket_t sock) : sock(sock) {}
    ~AutoCloseSocket()
    {
      if (sock != OSP_INVALID_SOCKET) {
        close(sock);
      }
    }
  };

  socket_t connect(const char *host, uint16_t port)
  {
    initialize();

    /*! create a new socket */
    socket_t sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == OSP_INVALID_SOCKET)
      throw std::runtime_error("cannot create socket");
    AutoCloseSocket auto_close(sockfd);

    /*! perform DNS lookup */
    struct hostent *server = ::gethostbyname(host);
    if (server == nullptr)
      throw std::runtime_error("server " + std::string(host) + " not found");

    /*! perform connection */
    struct sockaddr_in serv_addr;
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port   = (uint16_t)htons(port);
    memcpy((char *)&serv_addr.sin_addr.s_addr,
           (char *)server->h_addr,
           server->h_length);

    if (::connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      throw std::runtime_error("connection to " + std::string(host) + ":" +
                               std::to_string((long long)port) + " failed");

      /*! enable TCP_NODELAY */
#ifdef TCP_NODELAY
    {
      int flag = 1;
      ::setsockopt(
          sockfd, IPPROTO_TCP, TCP_NODELAY, (const char *)&flag, sizeof(int));
    }
#endif

    /*! we do not want SIGPIPE to be thrown */
#ifdef SO_NOSIGPIPE
    {
      int flag = 1;
      setsockopt(
          sockfd, SOL_SOCKET, SO_NOSIGPIPE, (const char *)&flag, sizeof(int));
    }
#endif

    auto_close.sock = OSP_INVALID_SOCKET;
    return sockfd;
  }

  socket_t listen(uint16_t port)
  {
    initialize();

    /*! create a new socket */
    socket_t sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == OSP_INVALID_SOCKET)
      throw std::runtime_error("cannot create socket");
    AutoCloseSocket auto_close(sockfd);

    /* When the server completes, the server socket enters a time-wait state
       during which the local address and port used by the socket are believed
       to be in use by the OS. The wait state may last several minutes. This
       socket option allows bind() to reuse the port immediately. */
#ifdef SO_REUSEADDR
    {
      int flag = true;
      ::setsockopt(
          sockfd, SOL_SOCKET, SO_REUSEADDR, (const char *)&flag, sizeof(int));
    }
#endif

    /*! bind socket to port */
    struct sockaddr_in serv_addr;
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_port        = (uint16_t)htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
      throw std::runtime_error("binding to port " +
                               std::to_string((long long)port) + " failed");

    /*! listen to port, up to 32 pending connections */
    if (::listen(sockfd, 32) < 0)
      throw std::runtime_error("listening on socket failed");

    auto_close.sock = OSP_INVALID_SOCKET;
    return sockfd;
  }

  socket_t accept(socket_t hsock)
  {
    /*! accept incoming connection */
    struct sockaddr_in addr = {};
    socklen_t len           = sizeof(addr);
    socket_t fd             = ::accept(hsock, (struct sockaddr *)&addr, &len);
    if (fd == OSP_INVALID_SOCKET)
      throw std::runtime_error("cannot accept connection");

      /*! enable TCP_NODELAY */
#ifdef TCP_NODELAY
    {
      int flag = 1;
      ::setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (char *)&flag, sizeof(int));
    }
#endif

    /*! we do not want SIGPIPE to be thrown */
#ifdef SO_NOSIGPIPE
    {
      int flag = 1;
      setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&flag, sizeof(int));
    }
#endif

    return fd;
  }

  void read(socket_t hsock, void *data_i, size_t bytes)
  {
    char *data = reinterpret_cast<char *>(data_i);
    while (bytes) {
      size_t readBytes = read_some(hsock, data, bytes);
      data += readBytes;
      bytes -= readBytes;
    }
  }

  size_t read_some(socket_t hsock, void *data_i, const size_t bytes)
  {
    char *data = reinterpret_cast<char *>(data_i);
    ssize_t n  = ::recv(hsock, data, bytes, 0);
    if (n == 0)
      throw Disconnect();
    else if (n < 0)
      throw std::runtime_error("error reading from socket");
    return n;
  }

  void write(socket_t hsock, const void *data_i, size_t bytes)
  {
    const char *data = reinterpret_cast<const char *>(data_i);
    while (bytes) {
      ssize_t n = ::send(hsock, data, bytes, 0);
      if (n < 0)
        throw std::runtime_error("error writing to socket");
      data += n;
      bytes -= n;
    }
  }

  void close(socket_t hsock)
  {
    ::shutdown(hsock, SHUT_RDWR);
#ifdef _WIN32
    ::closesocket(hsock);
#else
    ::close(hsock);
#endif
  }

  uint16_t getListenPort(socket_t socket)
  {
    struct sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    socklen_t socklen = sizeof(servAddr);
    getsockname(socket, (struct sockaddr *)&servAddr, &socklen);
    return ntohs(servAddr.sin_port);
  }

  std::string getIP(socket_t socket)
  {
    struct sockaddr_in servAddr;
    std::memset(&servAddr, 0, sizeof(servAddr));
    socklen_t socklen = sizeof(servAddr);
    getsockname(socket, (struct sockaddr *)&servAddr, &socklen);

    char myIP[256] = {0};
    if (!inet_ntop(AF_INET, &servAddr.sin_addr, myIP, 255))
      throw std::runtime_error("Failed to query my IP address");

    return std::string(myIP);
  }
}  // namespace rkcommon

