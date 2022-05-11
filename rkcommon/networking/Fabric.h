// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdlib>
#include <memory>
#include "../common.h"
#include "../utility/AbstractArray.h"

namespace rkcommon {
  namespace networking {

    /*! abstraction for a physical fabric that can transmit data -
      sockets, mpi, etc */
    struct RKCOMMON_INTERFACE Fabric
    {
      Fabric();
      virtual ~Fabric() = default;

      // Broadcast the data to all clients on the other end of the fabric
      // TODO: only makes sense to call on the root rank, so maybe a separate
      // "send" fabric ?
      virtual void sendBcast(
          std::shared_ptr<utility::AbstractArray<uint8_t>> buf) = 0;

      virtual void flushBcastSends() = 0;

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
}  // namespace rkcommon

