// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/utility/Observer.h"

using namespace rkcommon::utility;

SCENARIO("Observable/Observer interfaces", "[Observers]")
{
  GIVEN("A single observable and two observers")
  {
    Observable at;

    Observer look1(at);
    Observer look2(at);

    THEN("Neither observer has been notified after construction")
    {
      REQUIRE(!look1.wasNotified());
      REQUIRE(!look2.wasNotified());
    }

    WHEN("The observable notifies")
    {
      at.notifyObservers();

      THEN("Both observers independently are notified exactly once")
      {
        REQUIRE(look1.wasNotified());
        REQUIRE(look2.wasNotified());

        REQUIRE(!look1.wasNotified());
        REQUIRE(!look2.wasNotified());
      }
    }
  }
}
