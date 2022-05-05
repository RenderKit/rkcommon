// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"
#include "AbstractArray.h"
#include "FixedArrayView.h"

namespace rkcommon {
  namespace utility {

    /*  'FixedArrayView<T>' implements an array interface on a pointer to
     *  data which is owned by the FixedArrayView. The array is not
     *  initialized on creation and cannot be resized, though it can
     *  be recreated with a new size
     */
    template <typename T>
    struct FixedArrayView : public AbstractArray<T>
    {
      FixedArrayView()           = default;
      ~FixedArrayView() override = default;

      FixedArrayView(std::shared_ptr<FixedArray<T>> &data,
                     size_t offset,
                     size_t size);

     private:
      // The underlying array from the fixed array being viewed, to keep
      // the data alive for the view's lifetime
      std::shared_ptr<FixedArray<T>> data;
    };

    // Inlined FixedArrayView definitions

    template <typename T>
    FixedArrayView<T>::FixedArrayView(std::shared_ptr<FixedArray<T>> &_data,
                                      size_t offset,
                                      size_t size)
        : data(_data)
    {
      AbstractArray<T>::setPtr(data->begin() + offset, size);
    }

  }  // namespace utility
}  // namespace rkcommon
