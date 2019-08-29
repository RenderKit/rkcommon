// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

/*! iw: note this code is literally copied (and then adapted) from
    embree's original common/sys/network.h */

#pragma once

#include "../common.h"

namespace ospcommon {
  /*! type for a socket */
  using socket_t = uint32_t;
  OSPCOMMON_INTERFACE extern const socket_t OSP_INVALID_SOCKET;

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
  OSPCOMMON_INTERFACE socket_t listen(uint16_t port);

  /*! wait for and accept an incoing connection on the socket */
  OSPCOMMON_INTERFACE socket_t accept(socket_t sockfd);

  /*! initiates a connection */
  OSPCOMMON_INTERFACE socket_t connect(const char *host, uint16_t port);

  /*! read data from the socket */
  OSPCOMMON_INTERFACE void read(socket_t socket, void *data, size_t bytes);

  /*! read the available data on the socket, up to 'bytes' bytes.
      Returns the number of bytes read. */
  OSPCOMMON_INTERFACE size_t read_some(socket_t socket,
                                       void *data,
                                       const size_t bytes);

  /*! write data to the socket */
  OSPCOMMON_INTERFACE void write(socket_t socket,
                                 const void *data,
                                 size_t bytes);

  /*! close a socket */
  OSPCOMMON_INTERFACE void close(socket_t socket);

  /*! get the port that the socket is listening on */
  OSPCOMMON_INTERFACE uint16_t getListenPort(socket_t socket);

  /*! get the IP of this host which is visible through the socket connection */
  OSPCOMMON_INTERFACE std::string getIP(socket_t socket);

}  // namespace ospcommon
