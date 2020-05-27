// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace rkcommon {
  namespace tasking {

    void RKCOMMON_INTERFACE initTaskingSystem(int numThreads      = -1,
                                               bool flushDenormals = false);

    int RKCOMMON_INTERFACE numTaskingThreads();

  }  // namespace tasking
}  // namespace rkcommon
