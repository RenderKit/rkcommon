// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace ospcommon {
  namespace tasking {

    void OSPCOMMON_INTERFACE initTaskingSystem(int numThreads      = -1,
                                               bool flushDenormals = false);

    int OSPCOMMON_INTERFACE numTaskingThreads();

  }  // namespace tasking
}  // namespace ospcommon
