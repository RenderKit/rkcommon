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

#include "SocketFabric.h"

namespace ospcommon {
  namespace networking {

    // Static helper functions ////////////////////////////////////////////////

    static void closeIfExists(ospcommon::socket_t socket)
    {
      if (socket != OSP_INVALID_SOCKET)
        ospcommon::close(socket);
    }

    // SocketFabric definitions ///////////////////////////////////////////////

    SocketFabric::SocketFabric(const std::string &hostname, const uint16_t port)
        : socket(ospcommon::connect(hostname.c_str(), port))
    {
    }

    SocketFabric::SocketFabric(ospcommon::socket_t socket) : socket(socket) {}

    SocketFabric::~SocketFabric()
    {
      closeIfExists(socket);
    }

    SocketFabric::SocketFabric(SocketFabric &&other) : socket(other.socket)
    {
      // Note: the buffered socket destructor does not call shutdown
      other.socket = OSP_INVALID_SOCKET;
    }

    SocketFabric &SocketFabric::operator=(SocketFabric &&other)
    {
      closeIfExists(socket);

      socket       = other.socket;
      other.socket = OSP_INVALID_SOCKET;

      return *this;
    }

    void SocketFabric::send(std::shared_ptr<utility::ArrayView<uint8_t>> &buf)
    {
      ospcommon::write(socket, buf->data(), buf->size());
    }

    void SocketFabric::recv(utility::ArrayView<uint8_t> &buf)
    {
      ospcommon::read(socket, buf.data(), buf.size());
    }

    // SocketListener definitions /////////////////////////////////////////////

    SocketListener::SocketListener(const uint16_t port)
        : socket(ospcommon::listen(port))
    {
    }

    SocketListener::~SocketListener()
    {
      closeIfExists(socket);
    }

    SocketFabric SocketListener::accept()
    {
      return SocketFabric(ospcommon::accept(socket));
    }
  }  // namespace networking
}  // namespace ospcommon

