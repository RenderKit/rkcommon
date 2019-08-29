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

#pragma once

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>
#include "Fabric.h"
#include "Socket.h"
#include "utility/ArrayView.h"

namespace ospcommon {
  namespace networking {

    // TODO: Potentially remove this.

    /*! A fabric which sends and receives over a TCP socket connection */
    struct OSPCOMMON_INTERFACE SocketFabric
    {
      /*! Connecting to another socket on the host on the desired port */
      SocketFabric(const std::string &hostname, const uint16_t port);
      ~SocketFabric();
      SocketFabric(SocketFabric &&other);
      SocketFabric &operator=(SocketFabric &&other);

      SocketFabric(const SocketFabric &) = delete;
      SocketFabric &operator=(const SocketFabric &) = delete;

      void send(std::shared_ptr<utility::ArrayView<uint8_t>> &buf);

      void recv(utility::ArrayView<uint8_t> &buf);

     private:
      explicit SocketFabric(ospcommon::socket_t socket);
      friend struct SocketListener;

      // Data //

      ospcommon::socket_t socket;
    };

    /*! An abstraction for listening on a socket for connections,
     * for each accepted connection a SocketFabric will be returned */
    struct OSPCOMMON_INTERFACE SocketListener
    {
      /*! Setup the connection by listening for an incoming
          connection on the desired port */
      SocketListener(const uint16_t port);
      ~SocketListener();

      SocketListener(const SocketListener &) = delete;
      SocketListener &operator=(const SocketListener &) = delete;

      SocketFabric accept();

     private:
      ospcommon::socket_t socket;
    };

  }  // namespace networking
}  // namespace ospcommon

