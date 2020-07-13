// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "AbstractArray.h"

#include <array>
#include <vector>

namespace rkcommon {
  namespace utility {

    /*  'FixedArray<T>' implements an array interface on a pointer to
     *  data which is owned by the FixedArray. The array is not
     *  initialized on creation and cannot be resized, though it can
     *  be recreated with a new size
     */
    template <typename T>
    struct FixedArray : public AbstractArray<T>
    {
      FixedArray()           = default;
      ~FixedArray() override = default;

      explicit FixedArray(size_t size);

      explicit FixedArray(T *data, size_t size);

      template <size_t SIZE>
      FixedArray(std::array<T, SIZE> &init);

      FixedArray(std::vector<T> &init);

      template <size_t SIZE>
      FixedArray &operator=(std::array<T, SIZE> &rhs);

      FixedArray &operator=(std::vector<T> &rhs);

     private:
      void setPtr(size_t size);

      // We use a shared ptr to actually manage lifetime the data lifetime
      std::shared_ptr<T> array = nullptr;
    };

    // Inlined FixedArray definitions /////////////////////////////////////////

    template <typename T>
    inline FixedArray<T>::FixedArray(size_t _size)
        : array(std::shared_ptr<T>(new T[_size], std::default_delete<T[]>()))
    {
      setPtr(_size);
    }

    template <typename T>
    inline FixedArray<T>::FixedArray(T *_data, size_t _size) : FixedArray(_size)
    {
      std::memcpy(array.get(), _data, _size * sizeof(T));
    }

    template <typename T>
    template <size_t SIZE>
    inline FixedArray<T>::FixedArray(std::array<T, SIZE> &init)
        : FixedArray(init.data(), init.size())
    {
    }

    template <typename T>
    inline FixedArray<T>::FixedArray(std::vector<T> &init)
        : FixedArray(init.data(), init.size())
    {
    }

    template <typename T>
    template <size_t SIZE>
    inline FixedArray<T> &FixedArray<T>::operator=(std::array<T, SIZE> &rhs)
    {
      array = std::shared_ptr<T>(new T[rhs.size()], std::default_delete<T[]>());
      setPtr(rhs.size());
      std::memcpy(array.get(), rhs.data(), rhs.size() * sizeof(T));
      return *this;
    }

    template <typename T>
    inline FixedArray<T> &FixedArray<T>::operator=(std::vector<T> &rhs)
    {
      array = std::shared_ptr<T>(new T[rhs.size()], std::default_delete<T[]>());
      setPtr(rhs.size());
      std::memcpy(array.get(), rhs.data(), rhs.size() * sizeof(T));
      return *this;
    }

    template <typename T>
    inline void FixedArray<T>::setPtr(size_t size)
    {
      this->ptr      = array.get();
      this->numItems = size;
    }

  }  // namespace utility
}  // namespace rkcommon

