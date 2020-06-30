// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/utility/StringManip.h"

TEST_CASE("longestBeginningMatch() correctness", "[StringManip]")
{
  std::string input1 = "0123456";
  std::string input2 = "01234";

  auto output = rkcommon::utility::longestBeginningMatch(input1, input2);
  REQUIRE(output == "01234");
}

TEST_CASE("beginsWith() correctness", "[StringManip]")
{
  std::string input = "0123456";

  REQUIRE(rkcommon::utility::beginsWith(input, "012"));
  REQUIRE(!rkcommon::utility::beginsWith(input, "12"));
}

TEST_CASE("split() correctness", "[StringManip]")
{
  std::string input = "str0,str1,str2";

  auto output = rkcommon::utility::split(input, ',');
  REQUIRE(output[0] == "str0");
  REQUIRE(output[1] == "str1");
  REQUIRE(output[2] == "str2");
}

TEST_CASE("lowerCase() correctness", "[StringManip]")
{
  std::string input = "ABCd";

  auto output = rkcommon::utility::lowerCase(input);
  REQUIRE(output == "abcd");
}

TEST_CASE("upperCase() correctness", "[StringManip]")
{
  std::string input = "abcD";

  auto output = rkcommon::utility::upperCase(input);
  REQUIRE(output == "ABCD");
}
