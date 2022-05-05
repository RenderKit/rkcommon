// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include "../common.h"
#include "detail/pcg_random.hpp"

namespace rkcommon {
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

    // The std::uniform_real_distribution from <random> is not portable and may give
    // different results on different plaforms/compilers, we have to use our own
    // implementation for consistency
    template <typename T>
    struct uniform_real_distribution
    {
      uniform_real_distribution(T lowerValue = 0, T upperValue = 1)
          : l(lowerValue), u(upperValue)
      {}

      template <class G>
      T operator()(G &g)
      {
        const T scale = (u - l) / T(g.max() - g.min());
        return l + (g() - g.min()) * scale;
      }

     private:
      T l, u;
    };

  }  // namespace utility
}  // namespace rkcommon
