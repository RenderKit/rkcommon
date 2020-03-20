// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "./IntrusivePtr.h"

namespace ospcommon {
  namespace memory {

    // Type aliases for backward compatibility
    template <typename T>
    using Ref = IntrusivePtr<T>;

    using RefCount = RefCountedObject;

  }  // namespace memory
}  // namespace ospcommon
