// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "AbstractArray.h"

#include <array>
#include <vector>

namespace rkcommon {
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
    };

    // Inlined ArrayView definitions //////////////////////////////////////////

    template <typename T>
    inline ArrayView<T>::ArrayView(T *_data, size_t _size)
    {
      AbstractArray<T>::setPtr(_data, _size);
    }

    template <typename T>
    template <size_t SIZE>
    inline ArrayView<T>::ArrayView(std::array<T, SIZE> &init)
    {
      AbstractArray<T>::setPtr(init.data(), init.size());
    }

    template <typename T>
    inline ArrayView<T>::ArrayView(std::vector<T> &init)
    {
      AbstractArray<T>::setPtr(init.data(), init.size());
    }

    template <typename T>
    inline void ArrayView<T>::reset()
    {
      AbstractArray<T>::setPtr(nullptr, 0);
    }

    template <typename T>
    inline void ArrayView<T>::reset(T *_data, size_t _size)
    {
      AbstractArray<T>::setPtr(_data, _size);
    }

    template <typename T>
    template <size_t SIZE>
    inline ArrayView<T> &ArrayView<T>::operator=(std::array<T, SIZE> &rhs)
    {
      AbstractArray<T>::setPtr(rhs.data(), rhs.size());
      return *this;
    }

    template <typename T>
    inline ArrayView<T> &ArrayView<T>::operator=(std::vector<T> &rhs)
    {
      AbstractArray<T>::setPtr(rhs.data(), rhs.size());
      return *this;
    }

    // ArrayView utility functions ////////////////////////////////////////////

    template <typename T>
    inline ArrayView<T> make_ArrayView(T *data, size_t size)
    {
      return ArrayView<T>(data, size);
    }

  }  // namespace utility
}  // namespace rkcommon
