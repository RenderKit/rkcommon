// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

#include <array>
#include <stdexcept>
#include <vector>

namespace rkcommon {
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
}  // namespace rkcommon
