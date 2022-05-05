// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/tasking/async.h"

using rkcommon::tasking::async;

TEST_CASE("async", "[async]")
{
  auto future  = async([=]() { return 1; });
  int retvalue = future.get();
  REQUIRE(retvalue == 1);
}
