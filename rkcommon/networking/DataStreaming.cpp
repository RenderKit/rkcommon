// Copyright 2009-2020 Intel Corporation
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

    bool BufferReader::end()
    {
      return cursor >= buffer->size();
    }

    void WriteSizeCalculator::write(const void *, size_t size)
    {
      writtenSize += size;
    }

    FixedBufferWriter::FixedBufferWriter(size_t size)
        : buffer(std::make_shared<utility::FixedArray<uint8_t>>(size))
    {
    }

    void FixedBufferWriter::write(const void *mem, size_t size)
    {
      if (cursor + size >= buffer->size()) {
        throw std::runtime_error(
            "FixedBufferWriter::write size exceeds buffer");
      }
      std::memcpy(buffer->begin() + cursor, mem, size);
      cursor += size;
    }

    void *FixedBufferWriter::reserve(size_t size)
    {
      if (cursor + size >= buffer->size()) {
        throw std::runtime_error(
            "FixedBufferWriter::reserve size exceeds buffer");
      }
      void *mem = buffer->begin() + cursor;
      cursor += size;
      return mem;
    }

    std::shared_ptr<utility::FixedArray<uint8_t>::View>
    FixedBufferWriter::getWrittenView()
    {
      return std::make_shared<utility::FixedArray<uint8_t>::View>(
          buffer, 0, cursor);
    }

    size_t FixedBufferWriter::available() const
    {
      return buffer->size() - cursor;
    }

    size_t FixedBufferWriter::capacity() const
    {
      return buffer->size();
    }

  }  // namespace networking
}  // namespace rkcommon
