// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

/*! iw: note this code is literally copied (and then adapted) from
    embree's original common/sys/network.h */

#pragma once

#include "../common.h"

namespace rkcommon {
  /*! type for a socket */
  using socket_t = uint32_t;
  RKCOMMON_INTERFACE extern const socket_t OSP_INVALID_SOCKET;

  /*! exception thrown when other side disconnects */
  struct Disconnect : public std::exception
  {
#ifdef _WIN32
    virtual const char *what() const override
#else
    virtual const char *what() const noexcept override
#endif
    {
      return "network disconnect";
    }
  };

  /*! creates a socket bound to a port and listens for incoming connections */
  RKCOMMON_INTERFACE socket_t listen(uint16_t port);

  /*! wait for and accept an incoing connection on the socket */
  RKCOMMON_INTERFACE socket_t accept(socket_t sockfd);

  /*! initiates a connection */
  RKCOMMON_INTERFACE socket_t connect(const char *host, uint16_t port);

  /*! read data from the socket */
  RKCOMMON_INTERFACE void read(socket_t socket, void *data, size_t bytes);

  /*! read the available data on the socket, up to 'bytes' bytes.
      Returns the number of bytes read. */
  RKCOMMON_INTERFACE size_t read_some(socket_t socket,
                                       void *data,
                                       const size_t bytes);

  /*! write data to the socket */
  RKCOMMON_INTERFACE void write(socket_t socket,
                                 const void *data,
                                 size_t bytes);

  /*! close a socket */
  RKCOMMON_INTERFACE void close(socket_t socket);

  /*! get the port that the socket is listening on */
  RKCOMMON_INTERFACE uint16_t getListenPort(socket_t socket);

  /*! get the IP of this host which is visible through the socket connection */
  RKCOMMON_INTERFACE std::string getIP(socket_t socket);

}  // namespace rkcommon
