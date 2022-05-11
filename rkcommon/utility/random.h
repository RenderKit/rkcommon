// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cmath>
#include "../common.h"
#include "../math/vec.h"
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

    inline math::vec3f makeRandomColor(const unsigned int i)
    {
      const unsigned int mx = 13 * 17 * 43;
      const unsigned int my = 11 * 29;
      const unsigned int mz = 7 * 23 * 63;
      const unsigned int g = (i * (3 * 5 * 127) + 12312314);
      return math::vec3f((g % mx) * (1.f / (mx - 1)),
          (g % my) * (1.f / (my - 1)),
          (g % mz) * (1.f / (mz - 1)));
    }

  }  // namespace utility
}  // namespace rkcommon
