// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/utility/OnScopeExit.h"

using rkcommon::utility::OnScopeExit;

SCENARIO("OnScopeExit correctness", "[OnScopeExit]")
{
  GIVEN("A value captured by an OnScopeExit lambda")
  {
    int testValue = 0;

    THEN("The value is only affected after OnScopeExit is out of scope")
    {
      {
        OnScopeExit guard([&]() { testValue++; });

        REQUIRE(testValue == 0);
      }

      REQUIRE(testValue == 1);
    }
  }
}
