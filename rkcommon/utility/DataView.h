// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace rkcommon {
  namespace utility {

    template <typename T>
    struct DataView
    {
      DataView()  = default;
      ~DataView() = default;

      DataView(const void *data, size_t stride = sizeof(T));

      void reset(const void *data, size_t stride = sizeof(T));
      const T &operator[](size_t index) const;

     protected:
      const byte_t *ptr{nullptr};
      size_t stride{1};
    };

    // Inlined member definitions
    // ///////////////////////////////////////////////

    template <typename T>
    inline DataView<T>::DataView(const void *_data, size_t _stride)
        : ptr(static_cast<const byte_t *>(_data)), stride(_stride)
    {
    }

    template <typename T>
    inline void DataView<T>::reset(const void *_data, size_t _stride)
    {
      ptr    = static_cast<const byte_t *>(_data);
      stride = _stride;
    }

    template <typename T>
    inline const T &DataView<T>::operator[](size_t index) const
    {
      return *reinterpret_cast<const T *>(ptr + (index * stride));
    }

  }  // namespace utility
}  // namespace rkcommon
