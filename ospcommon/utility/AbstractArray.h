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

#include <stdexcept>
#include <array>
#include <vector>

namespace ospcommon {
  namespace utility {

    /*  'AbstractArray<T>' implements an array interface on a pointer
     *  to data which may be owned by the object.
     */
    template <typename T>
    struct AbstractArray
    {
      AbstractArray()  = default;
      virtual ~AbstractArray() = default;

      virtual void reset() = 0;
      virtual void reset(T *data, size_t size) = 0;

      virtual size_t size() const = 0;

      virtual T &operator[](size_t offset) const;
      // with bounds checking
      virtual T &at(size_t offset) const;

      virtual operator bool() const;
      virtual explicit operator T *() const;

      virtual T *data() const;

      virtual T *begin() const;
      virtual T *end() const;

      virtual const T *cbegin() const = 0;
      virtual const T *cend() const = 0;
    };

    template<typename T>
    T &AbstractArray<T>::operator[](size_t offset) const
    {
      return *(begin() + offset);
    }

    template<typename T>
    T &AbstractArray<T>::at(size_t offset) const
    {
      if (offset >= size())
        throw std::runtime_error("ArrayView<T>: out of bounds access!");

      return *(begin() + offset);
    }

    template<typename T>
    AbstractArray<T>::operator bool() const
    {
      return size() != 0;
    }

    template<typename T>
    AbstractArray<T>::operator T *() const
    {
      return begin();
    }

    template<typename T>
    T *AbstractArray<T>::data() const
    {
      return begin();
    }

    template<typename T>
    T *AbstractArray<T>::begin() const
    {
      return const_cast<T*>(cbegin());
    }

    template<typename T>
    T *AbstractArray<T>::end() const
    {
      return const_cast<T*>(cend());
    }

  }  // namespace utility
}  // namespace ospcommon

