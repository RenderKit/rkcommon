// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#define private public
#include "rkcommon/containers/FlatMap.h"
#undef private

#include <string>

using rkcommon::containers::FlatMap;

SCENARIO("FlatMap interface tests", "[FlatMap]")
{
  GIVEN("A default constructed FlatMap<>")
  {
    FlatMap<std::string, int> fm;

    THEN("The map should be empty")
    {
      REQUIRE(fm.size() == 0);
      REQUIRE(fm.empty());
      REQUIRE(fm.values.size() == 0);

      REQUIRE(!fm.contains("first"));

      REQUIRE(std::distance(fm.begin(), fm.end()) == 0);
      REQUIRE(std::distance(fm.cbegin(), fm.cend()) == 0);
      REQUIRE(std::distance(fm.rbegin(), fm.rend()) == 0);
      REQUIRE(std::distance(fm.crbegin(), fm.crend()) == 0);
    }

    WHEN("The map has a value added to it")
    {
      fm["first"] = 1;

      THEN("The map adds it to the underlying storage")
      {
        REQUIRE(fm.size() == 1);
        REQUIRE(!fm.empty());
        REQUIRE(std::distance(fm.begin(), fm.end()) == 1);
        REQUIRE(std::distance(fm.cbegin(), fm.cend()) == 1);
        REQUIRE(std::distance(fm.rbegin(), fm.rend()) == 1);
        REQUIRE(std::distance(fm.crbegin(), fm.crend()) == 1);
      }

      THEN("The map contains the key")
      {
        REQUIRE(fm.contains("first"));
        REQUIRE(!fm.contains("not_in_map"));
      }

      THEN("The map contains the correct value for the key")
      {
        REQUIRE(fm["first"] == 1);
        REQUIRE(fm.at("first") == 1);
      }

      THEN("The map contains the correct key/value at the index")
      {
        REQUIRE(fm.at_index(0) == std::make_pair(std::string("first"), 1));
      }

      THEN("Clearing the map clears underlying storage")
      {
        fm.clear();
        REQUIRE(fm.values.size() == 0);
      }
    }

    WHEN("The map has two values added to it")
    {
      fm["first"]  = 1;
      fm["second"] = 2;

      THEN("The map adds it to the underlying storage")
      {
        REQUIRE(fm.size() == 2);
        REQUIRE(!fm.empty());
        REQUIRE(std::distance(fm.begin(), fm.end()) == 2);
        REQUIRE(std::distance(fm.cbegin(), fm.cend()) == 2);
        REQUIRE(std::distance(fm.rbegin(), fm.rend()) == 2);
        REQUIRE(std::distance(fm.crbegin(), fm.crend()) == 2);
      }

      THEN("The map contains the keys")
      {
        REQUIRE(fm.contains("first"));
        REQUIRE(fm.contains("second"));
        REQUIRE(!fm.contains("not_in_map"));
      }

      THEN("The map contains the correct value for the keys")
      {
        REQUIRE(fm["first"] == 1);
        REQUIRE(fm["second"] == 2);
        REQUIRE(fm.at("first") == 1);
        REQUIRE(fm.at("second") == 2);
      }

      THEN("The map contains the correct key/value at the indices")
      {
        REQUIRE(fm.at_index(0) == std::make_pair(std::string("first"), 1));
        REQUIRE(fm.at_index(1) == std::make_pair(std::string("second"), 2));
      }

      THEN("Clearing the map clears underlying storage")
      {
        fm.clear();
        REQUIRE(fm.values.size() == 0);
      }

      THEN("Erasing a value mutates the elements correctly")
      {
        fm.erase("first");
        REQUIRE(!fm.contains("first"));
        REQUIRE(fm.size() == 1);
        REQUIRE(fm.at_index(0) == std::make_pair(std::string("second"), 2));
      }

    }
  }
}
