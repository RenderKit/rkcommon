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

#include <cstdlib>
#include <memory>
#include "../common.h"
#include "../utility/AbstractArray.h"

namespace ospcommon {
  namespace networking {

    /*! abstraction for a physical fabric that can transmit data -
      sockets, mpi, etc */
    struct OSPCOMMON_INTERFACE Fabric
    {
      virtual ~Fabric() = default;

      // Broadcast the data to all clients on the other end of the fabric
      // TODO: only makes sense to call on the root rank, so maybe a separate
      // "send" fabric ?
      virtual void sendBcast(
          std::shared_ptr<utility::AbstractArray<uint8_t>> buf) = 0;

      // Receive a broadcast of data from the fabric sender
      // TODO: only makes sense to call on the receivers, so maybe a separate
      // "recv" fabric ?
      virtual void recvBcast(utility::AbstractArray<uint8_t> &buf) = 0;

      // Send data to a specific rank in the fabric (callable on any rank)
      virtual void send(std::shared_ptr<utility::AbstractArray<uint8_t>> buf,
                        int rank) = 0;

      // Receive data from a specific rank on the fabric (callable on any rank)
      virtual void recv(utility::AbstractArray<uint8_t> &buf, int rank) = 0;
    };

  }  // namespace networking
}  // namespace ospcommon

