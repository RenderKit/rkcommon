// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "../utility/AbstractArray.h"
#include "../utility/FixedArray.h"
#include "../utility/FixedArrayView.h"
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

      virtual bool end() = 0;
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

      bool end() override;

      size_t cursor = 0;
      const std::shared_ptr<utility::AbstractArray<uint8_t>> buffer;
    };

    /*! Utility which behaves as a write stream, but just computes the number of
     * bytes which have been written to it
     */
    struct RKCOMMON_INTERFACE WriteSizeCalculator : public WriteStream
    {
      void write(const void *mem, size_t size) override;

      size_t writtenSize = 0;
    };

    /*! Buffer writer for writing to a fixed size output buffer. The cursor
     * points to the next location to write at. Trying to write more than the
     * fixed buffer's size will throw an error
     */
    struct RKCOMMON_INTERFACE FixedBufferWriter : public WriteStream
    {
      FixedBufferWriter() = default;

      FixedBufferWriter(size_t size);

      void write(const void *mem, size_t size) override;

      // Reserve space in the buffer and return the pointer to the start of it
      void *reserve(size_t size);

      // Get a view of the region written so far of the buffer
      std::shared_ptr<utility::FixedArray<uint8_t>::View> getWrittenView();

      // Get the space available to write in the buffer
      size_t available() const;

      // Get the underlying buffer size being written to
      size_t capacity() const;

      size_t cursor = 0;
      std::shared_ptr<utility::FixedArray<uint8_t>> buffer;
    };

    /*! generic stream operators into/out of streams, for raw data blocks */
    template <typename T>
    inline WriteStream &operator<<(WriteStream &buf, const T &rh)
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
    inline WriteStream &operator<<(WriteStream &buf, const std::vector<T> &rh)
    {
      const size_t sz = rh.size();
      buf << sz;

      for (const auto &x : rh)
        buf << x;

      return buf;
    }

    template <typename T>
    inline ReadStream &operator>>(ReadStream &buf, std::vector<T> &rh)
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
    inline WriteStream &operator<<(WriteStream &buf,
                                   const utility::AbstractArray<T> &rh)
    {
      const size_t sz = rh.size();
      buf << sz;
      buf.write((const byte_t *)rh.data(), sizeof(T) * sz);
      return buf;
    }
    /*! @} */

    /*! @{ serialize operations for strings */
    inline WriteStream &operator<<(WriteStream &buf, const std::string &rh)
    {
      const size_t sz = rh.size();
      buf << sz;
      buf.write((const void *)rh.data(), sz);
      return buf;
    }

    inline WriteStream &operator<<(WriteStream &buf, const char *rh)
    {
      const size_t sz = std::strlen(rh);
      buf << sz;
      buf.write((const void *)rh, sz);
      return buf;
    }

    inline ReadStream &operator>>(ReadStream &buf, std::string &rh)
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
