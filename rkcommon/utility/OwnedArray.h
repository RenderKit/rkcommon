// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "AbstractArray.h"

#include <array>
#include <vector>

namespace rkcommon {
  namespace utility {

    /*  'OwnedArray<T>' implements an array interface on a pointer to
     *  data which is owned by the OwnedArray.
     */
    template <typename T>
    struct OwnedArray : public AbstractArray<T>
    {
      OwnedArray()           = default;
      ~OwnedArray() override = default;

      template <size_t SIZE>
      OwnedArray(std::array<T, SIZE> &init);

      OwnedArray(std::vector<T> &init);

      explicit OwnedArray(T *data, size_t size);

      template <size_t SIZE>
      OwnedArray &operator=(std::array<T, SIZE> &rhs);

      OwnedArray &operator=(std::vector<T> &rhs);

      void reset();
      void reset(T *_data, size_t _size);

      void resize(size_t size, const T &val);

     private:
      void setPtr();

      std::vector<T> dataBuf;
    };

    // Inlined OwnedArray definitions /////////////////////////////////////////

    template <typename T>
    inline OwnedArray<T>::OwnedArray(T *_data, size_t _size)
        : dataBuf(_data, _data + _size)
    {
      setPtr();
    }

    template <typename T>
    template <size_t SIZE>
    inline OwnedArray<T>::OwnedArray(std::array<T, SIZE> &init)
        : dataBuf(init.begin(), init.end())
    {
      setPtr();
    }

    template <typename T>
    inline OwnedArray<T>::OwnedArray(std::vector<T> &init) : dataBuf(init)
    {
      setPtr();
    }

    template <typename T>
    template <size_t SIZE>
    inline OwnedArray<T> &OwnedArray<T>::operator=(std::array<T, SIZE> &rhs)
    {
      dataBuf = std::vector<T>(rhs.begin(), rhs.end());
      setPtr();
      return *this;
    }

    template <typename T>
    inline OwnedArray<T> &OwnedArray<T>::operator=(std::vector<T> &rhs)
    {
      dataBuf = std::vector<T>(rhs.begin(), rhs.end());
      setPtr();
      return *this;
    }

    template <typename T>
    inline void OwnedArray<T>::reset()
    {
      dataBuf.clear();
      dataBuf.shrink_to_fit();
    }

    template <typename T>
    inline void OwnedArray<T>::reset(T *_data, size_t _size)
    {
      dataBuf = std::vector<T>(_data, _data + _size);
      setPtr();
    }

    template <typename T>
    inline void OwnedArray<T>::resize(size_t size, const T &val)
    {
      dataBuf.resize(size, val);
      setPtr();
    }

    template <typename T>
    inline void OwnedArray<T>::setPtr()
    {
      this->ptr      = dataBuf.data();
      this->numItems = dataBuf.size();
    }

  }  // namespace utility
}  // namespace rkcommon
