// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <vector>
#include "aligned_allocator.h"

namespace rkcommon {
  namespace containers {

    template <typename T>
    using AlignedVector = std::vector<T, aligned_allocator<T>>;

  }  // namespace containers
}  // namespace rkcommon