// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "ospcommon/tasking/schedule.h"

#include <atomic>

using ospcommon::tasking::schedule;

TEST_CASE("schedule", "[schedule]")
{
  std::atomic<int> val{0};

  auto *val_p = &val;

  schedule([=]() { *val_p = 1; });

  while (val.load() == 0)
    ;

  REQUIRE(val.load() == 1);
}
