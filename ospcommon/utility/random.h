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

#pragma once

#include <cmath>
#include "../common.h"
#include "detail/pcg_random.hpp"

namespace ospcommon {
  namespace utility {

    class pcg32_biased_float_distribution
    {
     public:
      pcg32_biased_float_distribution(int seed,
                                      int sequence,
                                      float lower,
                                      float upper);
      float operator()();

     private:
      pcg32 rng;
      float lower, upper, diff;
    };

    // Inlined pcg32_biased_float_distribution definitions ///////////////////

    inline pcg32_biased_float_distribution::pcg32_biased_float_distribution(
        int seed, int sequence, float lower, float upper)
        : lower(lower), upper(upper)
    {
      diff = upper - lower;
      rng.seed(seed, sequence);
    }

    inline float pcg32_biased_float_distribution::operator()()
    {
      const unsigned scaleBits = 0x2F800000;  // 2^(-32)
      const float scale        = *(float *)&scaleBits;
      return (scale * rng()) * diff + lower;
    }

  }  // namespace utility
}  // namespace ospcommon
