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
#include "AbstractArray.h"

#include <array>
#include <vector>

namespace ospcommon {
  namespace utility {

    /*  'OwnedArray<T>' implements an array interface on a pointer to
     *  data which is owned by the OwnedArray.
     */
    template <typename T>
    struct OwnedArray : public AbstractArray<T>
    {
      OwnedArray() = default;

      template <size_t SIZE>
      OwnedArray(std::array<T, SIZE> &init);

      OwnedArray(std::vector<T> &init);

      explicit OwnedArray(T *data, size_t size);

      template <size_t SIZE>
      OwnedArray& operator=(std::array<T, SIZE> &rhs);

      OwnedArray& operator=(std::vector<T> &rhs);

      void reset() override;
      void reset(T *data, size_t size) override;

      void resize(size_t size, const T &val);

      size_t size() const override;

      const T *cbegin() const override;
      const T *cend() const override;

    protected:
      std::vector<T> dataBuf;
    };

    // Inlined OwnedArray definitions //////////////////////////////////////////

    template <typename T>
    inline OwnedArray<T>::OwnedArray(T *_data, size_t _size)
      : dataBuf(_data, _data + _size)
    {}

    template <typename T>
    template <size_t SIZE>
    inline OwnedArray<T>::OwnedArray(std::array<T, SIZE> &init)
      : dataBuf(init.begin(), init.end())
    {}

    template <typename T>
    inline OwnedArray<T>::OwnedArray(std::vector<T> &init)
      : dataBuf(init)
    {}

    template <typename T>
    template <size_t SIZE>
    inline OwnedArray<T>&
    OwnedArray<T>::operator=(std::array<T, SIZE> &rhs)
    {
      dataBuf = std::vector<T>(rhs.begin(), rhs.end());
      return *this;
    }

    template <typename T>
    inline OwnedArray<T>& OwnedArray<T>::operator=(std::vector<T> &rhs)
    {
      dataBuf = std::vector<T>(rhs.begin(), rhs.end());
      return *this;
    }

    template <typename T>
    inline void OwnedArray<T>::reset()
    {
      dataBuf.clear();
    }

    template <typename T>
    inline void OwnedArray<T>::reset(T *_data, size_t _size)
    {
      dataBuf = std::vector<T>(_data, _data + _size);
    }

    template <typename T>
    inline void OwnedArray<T>::resize(size_t size, const T &val)
    {
      dataBuf.resize(size, val);
    }

    template <typename T>
    size_t OwnedArray<T>::size() const
    {
      return dataBuf.size();
    }

    template <typename T>
    const T *OwnedArray<T>::cbegin() const
    {
      if (size() > 0) {
        return &dataBuf[0];
      }
      return nullptr;
    }

    template <typename T>
    const T *OwnedArray<T>::cend() const
    {
      if (size() > 0) {
        return &dataBuf.back();
      }
      return nullptr;
    }

  } // ::ospcommon::utility
} // ::ospcommon


