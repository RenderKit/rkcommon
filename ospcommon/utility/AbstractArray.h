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

#include "../common.h"

#include <array>
#include <stdexcept>
#include <vector>

namespace ospcommon {
  namespace utility {

    /*  'AbstractArray<T>' implements an array interface on a pointer
     *  to data which may be owned by the object.
     */
    template <typename T>
    struct AbstractArray
    {
      virtual ~AbstractArray() = default;

      size_t size() const;

      T &operator[](size_t offset) const;
      // with bounds checking
      T &at(size_t offset) const;

      operator bool() const;
      explicit operator T *() const;

      T *data() const;

      T *begin() const;
      T *end() const;

      const T *cbegin() const;
      const T *cend() const;

     protected:
      // Can only be constructed by child classes
      AbstractArray() = default;

      T *ptr{nullptr};
      size_t numItems{0};
    };

    // Inlined definitions ////////////////////////////////////////////////////

    template <typename T>
    inline size_t AbstractArray<T>::size() const
    {
      return numItems;
    }

    template <typename T>
    inline T &AbstractArray<T>::operator[](size_t offset) const
    {
      return *(begin() + offset);
    }

    template <typename T>
    inline T &AbstractArray<T>::at(size_t offset) const
    {
      if (offset >= size())
        throw std::runtime_error("ArrayView<T>: out of bounds access!");

      return *(begin() + offset);
    }

    template <typename T>
    inline AbstractArray<T>::operator bool() const
    {
      return size() != 0;
    }

    template <typename T>
    inline AbstractArray<T>::operator T *() const
    {
      return begin();
    }

    template <typename T>
    inline T *AbstractArray<T>::data() const
    {
      return begin();
    }

    template <typename T>
    inline T *AbstractArray<T>::begin() const
    {
      return ptr;
    }

    template <typename T>
    inline T *AbstractArray<T>::end() const
    {
      return ptr + size();
    }

    template <typename T>
    inline const T *AbstractArray<T>::cbegin() const
    {
      return begin();
    }

    template <typename T>
    inline const T *AbstractArray<T>::cend() const
    {
      return end();
    }

  }  // namespace utility
}  // namespace ospcommon
