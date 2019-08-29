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

#include "DataStreaming.h"
#include "../common.h"

#include <vector>

namespace ospcommon {
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
}  // namespace ospcommon

