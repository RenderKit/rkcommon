// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"
#include "rkcommon/memory/RefCount.h"

using rkcommon::memory::Ref;
using rkcommon::memory::RefCount;

SCENARIO("Ref interface tests", "[Ref]")
{
  GIVEN("A const Ref<>")
  {
    struct A : public RefCount
    {
      int i1{0};
      int i2{0};
    };
    const Ref<A> a(new A);

    WHEN("setting values via pointer or reference")
    {
      A &aRef = *a;
      aRef.i1 = 1;
      a->i2 = 2;

      THEN("they should be set")
      {
        REQUIRE(a->i1 == 1);
        REQUIRE(a->i2 == 2);
      }
    }
  }
}
