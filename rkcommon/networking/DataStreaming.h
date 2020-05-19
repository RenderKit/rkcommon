// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "../utility/AbstractArray.h"
#include "../utility/OwnedArray.h"

#include <vector>

namespace rkcommon {
  namespace networking {

    /*! abstraction of an object that we can serailize/write (raw) data into */
    struct RKCOMMON_INTERFACE WriteStream
    {
      virtual ~WriteStream() = default;

      virtual void write(const void *mem, size_t size) = 0;
      virtual void flush() {}
    };

    /*! abstraction of an object that we can read (raw) data from to
      then de-serialize into work objects */
    struct RKCOMMON_INTERFACE ReadStream
    {
      virtual ~ReadStream() = default;

      virtual void read(void *mem, size_t size) = 0;
    };

    struct RKCOMMON_INTERFACE BufferWriter : WriteStream
    {
      BufferWriter();

      void write(const void *mem, size_t size) override;

      std::shared_ptr<utility::OwnedArray<uint8_t>> buffer;
    };

    struct RKCOMMON_INTERFACE BufferReader : ReadStream
    {
      BufferReader(const std::shared_ptr<utility::AbstractArray<uint8_t>> &buf);

      void read(void *mem, size_t size) override;

      size_t cursor = 0;
      const std::shared_ptr<utility::AbstractArray<uint8_t>> buffer;
    };

    /*! generic stream operators into/out of streams, for raw data blocks */
    template <typename T>
    inline WriteStream &operator<<(WriteStream &buf,
                                                       const T &rh)
    {
      buf.write((const byte_t *)&rh, sizeof(T));
      return buf;
    }

    template <typename T>
    inline ReadStream &operator>>(ReadStream &buf, T &rh)
    {
      buf.read((byte_t *)&rh, sizeof(T));
      return buf;
    }

    /*! @{ stream operators into/out of read/write streams, for std::vectors
     * of non-POD types*/
    template <typename T>
    inline WriteStream &operator<<(WriteStream &buf,
                                                       const std::vector<T> &rh)
    {
      const size_t sz = rh.size();
      buf << sz;

      for (const auto &x : rh)
        buf << x;

      return buf;
    }

    template <typename T>
    inline ReadStream &operator>>(ReadStream &buf,
                                                      std::vector<T> &rh)
    {
      size_t sz;
      buf >> sz;
      rh.resize(sz);

      for (size_t i = 0; i < sz; ++i)
        buf >> rh[i];

      return buf;
    }
    /*! @} */

    /*! @{ stream operators into/out of read/write streams, for AbstractArray<T>
     */
    template <typename T>
    inline WriteStream &operator<<(
        WriteStream &buf, const utility::AbstractArray<T> &rh)
    {
      const size_t sz = rh.size();
      buf << sz;
      buf.write((const byte_t *)rh.data(), sizeof(T) * sz);
      return buf;
    }
    /*! @} */

    /*! @{ serialize operations for strings */
    inline WriteStream &operator<<(WriteStream &buf,
                                                       const std::string &rh)
    {
      const size_t sz = rh.size();
      buf << sz;
      buf.write((const void *)rh.data(), sz);
      return buf;
    }

    inline ReadStream &operator>>(ReadStream &buf,
                                                      std::string &rh)
    {
      size_t sz;
      buf >> sz;
      rh.resize(sz + 1);
      buf.read((void *)rh.data(), sz);
      return buf;
    }
    /*! @} */

  }  // namespace networking
}  // namespace rkcommon
