// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include <random>
#include "../catch.hpp"
#include "rkcommon/utility/random.h"

using rkcommon::utility::pcg32_biased_float_distribution;

TEST_CASE("random", "[random]")
{
  std::random_device rd;
  std::default_random_engine e1(rd());
  std::uniform_real_distribution<float> dist(-1000, 1000);

  for (int iter = 0; iter < 1000; iter++) {
    float lower = dist(e1);
    float upper = dist(e1);
    pcg32_biased_float_distribution osp_dist(iter, iter, lower, upper);

    INFO("seed = " << iter << " lower = " << lower << " upper = " << upper);
    if (lower > upper)
      std::swap(lower, upper);
    for (int i = 0; i < 1000; i++) {
      float val = osp_dist();
      INFO("val = " << val);
      REQUIRE(val >= lower);
      REQUIRE(val <= upper);
    }
  }
}