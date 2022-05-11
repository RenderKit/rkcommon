// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#define private public
#include "rkcommon/containers/TransactionalBuffer.h"
#undef private

using rkcommon::containers::TransactionalBuffer;

SCENARIO("TransactionalBuffer interface tests", "[TransactionalBuffer]")
{
  GIVEN("A default constructed TransactionalBuffer<>")
  {
    TransactionalBuffer<int> tb;

    THEN("The buffer should be empty")
    {
      REQUIRE(tb.size() == 0);
      REQUIRE(tb.empty());
    }

    WHEN("The buffer has a value pushed back on it")
    {
      tb.push_back(2);

      THEN("The buffer adds it to the underlying storage")
      {
        REQUIRE(tb.buffer.size() == 1);
        REQUIRE(tb.size() == 1);
        REQUIRE(!tb.empty());
        REQUIRE(tb.buffer[0] == 2);
      }

      THEN("Consuming the buffer empties it")
      {
        auto v = tb.consume();

        REQUIRE(v.size() == 1);
        REQUIRE(v[0] == 2);

        REQUIRE(tb.buffer.empty());
      }
    }
  }
}
