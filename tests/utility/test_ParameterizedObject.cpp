// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/utility/ParameterizedObject.h"

using rkcommon::utility::ParameterizedObject;

SCENARIO("ParameterizedObject interface", "[ParameterizedObject]")
{
  GIVEN("A ParameterizedObject with a parameter set")
  {
    ParameterizedObject obj;

    auto name = "test_int";

    obj.setParam(name, 5);

    THEN("The named parameter should have the correct type and value")
    {
      REQUIRE(obj.hasParam(name));
      REQUIRE(obj.getParam<int>(name, 4) == 5);
      REQUIRE(obj.getParam<short>(name, 4) == 4);
    }

    WHEN("The parameter is removed")
    {
      obj.removeParam(name);

      THEN("The paramter should no longer exist on the object")
      {
        REQUIRE(!obj.hasParam(name));
        REQUIRE(obj.getParam<int>(name, 4) == 4);
        REQUIRE(obj.getParam<short>(name, 4) == 4);
      }
    }
  }
}
