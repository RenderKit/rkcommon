// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "../catch.hpp"

#define private public
#include "ospcommon/containers/TransactionalBuffer.h"
#undef private

using ospcommon::containers::TransactionalBuffer;

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
