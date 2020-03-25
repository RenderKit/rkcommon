// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <memory>
#include <utility>

namespace ospcommon {
  namespace memory {

    template <typename T>
    using DeletedUniquePtr = std::unique_ptr<T, std::function<void(T *)>>;

    template <typename T, typename DELETE_FCN, typename... Args>
    inline DeletedUniquePtr<T> make_deleted_unique(DELETE_FCN &&deleter,
                                                   Args &&... args)
    {
      return DeletedUniquePtr<T>(new T(std::forward<Args>(args)...), deleter);
    }

  }  // namespace memory
}  // namespace ospcommon
