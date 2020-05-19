// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/tasking/parallel_foreach.h"

#include <algorithm>
#include <vector>

using rkcommon::tasking::parallel_foreach;

TEST_CASE("parallel_foreach", "[parallel_foreach]")
{
  const size_t N_ELEMENTS = size_t(1e8);

  const int bad_value  = 0;
  const int good_value = 1;

  std::vector<int> v(N_ELEMENTS);
  std::fill(v.begin(), v.end(), bad_value);

  parallel_foreach(v, [&](int &value) { value = good_value; });

  auto found = std::find(v.begin(), v.end(), bad_value);

  REQUIRE(found == v.end());
}
