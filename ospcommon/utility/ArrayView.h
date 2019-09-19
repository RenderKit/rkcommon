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

    /*  'ArrayView<T>' implements an array interface on a pointer to data which
     *  is *NOT* owned by ArrayView. If you want ArrayView to own data, then
     *  instead use std::array<T> or std::vector<T>.
     */
    template <typename T>
    struct ArrayView : public AbstractArray<T>
    {
      ArrayView()           = default;
      ~ArrayView() override = default;

      template <size_t SIZE>
      ArrayView(std::array<T, SIZE> &init);

      ArrayView(std::vector<T> &init);

      explicit ArrayView(T *data, size_t size);

      void reset();
      void reset(T *data, size_t size);

      template <size_t SIZE>
      ArrayView &operator=(std::array<T, SIZE> &rhs);

      ArrayView &operator=(std::vector<T> &rhs);

     private:
      void setPtr(T *data, size_t size);
    };

    // Inlined ArrayView definitions //////////////////////////////////////////

    template <typename T>
    inline ArrayView<T>::ArrayView(T *_data, size_t _size)
    {
      setPtr(_data, _size);
    }

    template <typename T>
    template <size_t SIZE>
    inline ArrayView<T>::ArrayView(std::array<T, SIZE> &init)
    {
      setPtr(init.data(), init.size());
    }

    template <typename T>
    inline ArrayView<T>::ArrayView(std::vector<T> &init)
    {
      setPtr(init.data(), init.size());
    }

    template <typename T>
    inline void ArrayView<T>::reset()
    {
      setPtr(nullptr, 0);
    }

    template <typename T>
    inline void ArrayView<T>::reset(T *_data, size_t _size)
    {
      setPtr(_data, _size);
    }

    template <typename T>
    template <size_t SIZE>
    inline ArrayView<T> &ArrayView<T>::operator=(std::array<T, SIZE> &rhs)
    {
      setPtr(rhs.data(), rhs.size());
      return *this;
    }

    template <typename T>
    inline ArrayView<T> &ArrayView<T>::operator=(std::vector<T> &rhs)
    {
      setPtr(rhs.data(), rhs.size());
      return *this;
    }

    template <typename T>
    inline void ArrayView<T>::setPtr(T *data, size_t size)
    {
      this->ptr      = data;
      this->numItems = size;
    }

    // ArrayView utility functions ////////////////////////////////////////////

    template <typename T>
    inline ArrayView<T> make_ArrayView(T *data, size_t size)
    {
      return ArrayView<T>(data, size);
    }

  }  // namespace utility
}  // namespace ospcommon
