// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "DataStreaming.h"
#include "../common.h"

#include <vector>

namespace rkcommon {
  namespace networking {

    BufferWriter::BufferWriter()
        : buffer(std::make_shared<utility::OwnedArray<uint8_t>>())
    {
    }

    void BufferWriter::write(const void *mem, size_t size)
    {
      const size_t bsize = buffer->size();
      buffer->resize(buffer->size() + size, 0);
      std::memcpy(buffer->begin() + bsize, mem, size);
    }

    BufferReader::BufferReader(
        const std::shared_ptr<utility::AbstractArray<uint8_t>> &buf)
        : buffer(buf)
    {
    }

    void BufferReader::read(void *mem, size_t size)
    {
      if (cursor + size > buffer->size())
        throw std::runtime_error("Attempt to read past end of BufferReader!");

      std::memcpy(mem, buffer->begin() + cursor, size);
      cursor += size;
    }
  }  // namespace networking
}  // namespace rkcommon

