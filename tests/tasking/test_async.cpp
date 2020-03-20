// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "ospcommon/tasking/async.h"

using ospcommon::tasking::async;

TEST_CASE("async", "[async]")
{
  auto future  = async([=]() { return 1; });
  int retvalue = future.get();
  REQUIRE(retvalue == 1);
}
