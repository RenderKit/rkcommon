// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace rkcommon {
  namespace tasking {

    void RKCOMMON_INTERFACE initTaskingSystem(int numThreads      = -1,
                                               bool flushDenormals = false);
    void RKCOMMON_INTERFACE shutdownTaskingSystem();

    int RKCOMMON_INTERFACE numTaskingThreads();

  }  // namespace tasking
}  // namespace rkcommon
