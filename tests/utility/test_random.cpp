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

#include <random>
#include "../catch.hpp"
#include "ospcommon/utility/random.h"

using ospcommon::utility::float_distribution;

TEST_CASE("random", "[random]")
{
  std::random_device rd;
  std::default_random_engine e1(rd());
  std::uniform_real_distribution<float> dist(-1000, 1000);

  for (int iter = 0; iter < 1000; iter++) {
    float lower = dist(e1);
    float upper = dist(e1);
    float_distribution osp_dist(iter, iter, lower, upper);

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