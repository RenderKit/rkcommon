// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/containers/AlignedVector.h"

using rkcommon::containers::AlignedVector;

SCENARIO("verify alignment", "[AlignedVector]")
{
  GIVEN("An AlignedVector<> with reserved memory")
  {
    AlignedVector<int> aligned_vec;

    aligned_vec.resize(500);

    THEN("The allocated memory should be aligned")
    {
      REQUIRE(rkcommon::memory::isAligned(aligned_vec.data()));
    }
  }
}
