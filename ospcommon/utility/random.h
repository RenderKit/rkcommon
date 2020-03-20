// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

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
