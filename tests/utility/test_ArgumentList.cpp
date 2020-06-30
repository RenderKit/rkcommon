// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/utility/ArgumentList.h"

using rkcommon::utility::ArgumentList;

static const char *test_arguments[5] = {
    "testApp", "arg1", "arg2", "arg3", "arg4"};

SCENARIO("ArgumentList correctness", "[ArgumentList]")
{
  GIVEN("A constructed ArgumentList from a given const char *[]")
  {
    ArgumentList args(5, test_arguments);

    THEN("The list is initially not empty")
    {
      REQUIRE(!args.empty());
    }

    THEN("The list contains only the arguments, and not the application")
    {
      REQUIRE(args.size() == 4);
      REQUIRE(args[0] != "testApp");
    }

    THEN("Removal of arguments from given indices mutates the list")
    {
      args.remove(0);
      REQUIRE(args[0] == "arg2");
      REQUIRE(args[1] == "arg3");
      REQUIRE(args[2] == "arg4");

      args.remove(0, 2);
      REQUIRE(args[0] == "arg4");

      args.remove(0);
      REQUIRE(args.empty());
    }
  }
}

struct TestParser final : public rkcommon::utility::ArgumentsParser
{
  int tryConsume(ArgumentList &argList, int argID) override
  {
    const auto arg = argList[argID];
    if (arg == "arg1")
      return 1;
    else if (arg == "arg3")
      return 2;

    return 0;
  }
};

TEST_CASE("ArgumentsParser correctness", "[ArgumentList]")
{
  ArgumentList args(5, test_arguments);

  TestParser parser;
  parser.parseAndRemove(args);

  REQUIRE(args.size() == 1);
  REQUIRE(args[0] == "arg2");
}
