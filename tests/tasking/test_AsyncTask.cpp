// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/tasking/AsyncTask.h"

using rkcommon::tasking::AsyncTask;

TEST_CASE("AsyncTask", "[AsyncTask]")
{
  AsyncTask<float> t([=]() { return 1.f; });
  float retValue = t.get();
  REQUIRE(retValue == 1.f);
}
