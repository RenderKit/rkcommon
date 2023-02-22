// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <limits>
#include "../platform.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>  // using cmath causes issues under Windows
#include <cfloat>

namespace rkcommon {
  namespace math {

    static const float one_over_255 = 1.0f / 255.0f;

    static struct ZeroTy
    {
      __forceinline operator double() const
      {
        return 0;
      }
      __forceinline operator float() const
      {
        return 0;
      }
      __forceinline operator long long() const
      {
        return 0;
      }
      __forceinline operator unsigned long long() const
      {
        return 0;
      }
      __forceinline operator long() const
      {
        return 0;
      }
      __forceinline operator unsigned long() const
      {
        return 0;
      }
      __forceinline operator int() const
      {
        return 0;
      }
      __forceinline operator unsigned int() const
      {
        return 0;
      }
      __forceinline operator short() const
      {
        return 0;
      }
      __forceinline operator unsigned short() const
      {
        return 0;
      }
      __forceinline operator char() const
      {
        return 0;
      }
      __forceinline operator unsigned char() const
      {
        return 0;
      }
    } zero MAYBE_UNUSED;

    static struct OneTy
    {
      __forceinline operator double() const
      {
        return 1;
      }
      __forceinline operator float() const
      {
        return 1;
      }
      __forceinline operator long long() const
      {
        return 1;
      }
      __forceinline operator unsigned long long() const
      {
        return 1;
      }
      __forceinline operator long() const
      {
        return 1;
      }
      __forceinline operator unsigned long() const
      {
        return 1;
      }
      __forceinline operator int() const
      {
        return 1;
      }
      __forceinline operator unsigned int() const
      {
        return 1;
      }
      __forceinline operator short() const
      {
        return 1;
      }
      __forceinline operator unsigned short() const
      {
        return 1;
      }
      __forceinline operator char() const
      {
        return 1;
      }
      __forceinline operator unsigned char() const
      {
        return 1;
      }
    } one MAYBE_UNUSED;

    static struct NegInfTy
    {
      __forceinline operator double() const
      {
        return -std::numeric_limits<double>::infinity();
      }
      __forceinline operator float() const
      {
        return -std::numeric_limits<float>::infinity();
      }
      __forceinline operator long long() const
      {
        return std::numeric_limits<long long>::min();
      }
      __forceinline operator unsigned long long() const
      {
        return std::numeric_limits<unsigned long long>::min();
      }
      __forceinline operator long() const
      {
        return std::numeric_limits<long>::min();
      }
      __forceinline operator unsigned long() const
      {
        return std::numeric_limits<unsigned long>::min();
      }
      __forceinline operator int() const
      {
        return std::numeric_limits<int>::min();
      }
      __forceinline operator unsigned int() const
      {
        return std::numeric_limits<unsigned int>::min();
      }
      __forceinline operator short() const
      {
        return std::numeric_limits<short>::min();
      }
      __forceinline operator unsigned short() const
      {
        return std::numeric_limits<unsigned short>::min();
      }
      __forceinline operator char() const
      {
        return std::numeric_limits<char>::min();
      }
      __forceinline operator unsigned char() const
      {
        return std::numeric_limits<unsigned char>::min();
      }

    } neg_inf MAYBE_UNUSED;

    static struct PosInfTy
    {
      __forceinline operator double() const
      {
        return std::numeric_limits<double>::infinity();
      }
      __forceinline operator float() const
      {
        return std::numeric_limits<float>::infinity();
      }
      __forceinline operator long long() const
      {
        return std::numeric_limits<long long>::max();
      }
      __forceinline operator unsigned long long() const
      {
        return std::numeric_limits<unsigned long long>::max();
      }
      __forceinline operator long() const
      {
        return std::numeric_limits<long>::max();
      }
      __forceinline operator unsigned long() const
      {
        return std::numeric_limits<unsigned long>::max();
      }
      __forceinline operator int() const
      {
        return std::numeric_limits<int>::max();
      }
      __forceinline operator unsigned int() const
      {
        return std::numeric_limits<unsigned int>::max();
      }
      __forceinline operator short() const
      {
        return std::numeric_limits<short>::max();
      }
      __forceinline operator unsigned short() const
      {
        return std::numeric_limits<unsigned short>::max();
      }
      __forceinline operator char() const
      {
        return std::numeric_limits<char>::max();
      }
      __forceinline operator unsigned char() const
      {
        return std::numeric_limits<unsigned char>::max();
      }
    } inf MAYBE_UNUSED, pos_inf MAYBE_UNUSED;

    static struct NaNTy
    {
      __forceinline operator double() const
      {
        return std::numeric_limits<double>::quiet_NaN();
      }
      __forceinline operator float() const
      {
        return std::numeric_limits<float>::quiet_NaN();
      }
    } nan MAYBE_UNUSED;

    static struct UlpTy
    {
      __forceinline operator double() const
      {
        return std::numeric_limits<double>::epsilon();
      }
      __forceinline operator float() const
      {
        return std::numeric_limits<float>::epsilon();
      }
    } ulp MAYBE_UNUSED;

    static struct PiTy
    {
      __forceinline operator double() const
      {
        return M_PI;
      }
      __forceinline operator float() const
      {
        return M_PI;
      }
    } pi MAYBE_UNUSED;

    static struct OneOverPiTy
    {
      __forceinline operator double() const
      {
        return M_1_PI;
      }
      __forceinline operator float() const
      {
        return M_1_PI;
      }
    } one_over_pi MAYBE_UNUSED;

    static struct TwoPiTy
    {
      __forceinline operator double() const
      {
        return 2.0 * M_PI;
      }
      __forceinline operator float() const
      {
        return 2.0 * M_PI;
      }
    } two_pi MAYBE_UNUSED;

    static struct HalfPiTy
    {
      __forceinline operator double() const
      {
        return M_PI_2;
      }
      __forceinline operator float() const
      {
        return M_PI_2;
      }
    } half_pi MAYBE_UNUSED;

    static struct OneOverTwoPiTy
    {
      __forceinline operator double() const
      {
        return 0.5 * M_1_PI;
      }
      __forceinline operator float() const
      {
        return 0.5 * M_1_PI;
      }
    } one_over_two_pi MAYBE_UNUSED;

    static struct FourPiTy
    {
      __forceinline operator double() const
      {
        return 4.0 * M_PI;
      }
      __forceinline operator float() const
      {
        return 4.0 * M_PI;
      }
    } four_pi MAYBE_UNUSED;

    static struct QuarterPiTy
    {
      __forceinline operator double() const
      {
        return M_PI_4;
      }
      __forceinline operator float() const
      {
        return M_PI_4;
      }
    } quarter_pi MAYBE_UNUSED;

    static struct OneOverFourPiTy
    {
      __forceinline operator double() const
      {
        return 0.25 * M_1_PI;
      }
      __forceinline operator float() const
      {
        return 0.25 * M_1_PI;
      }
    } one_over_four_pi MAYBE_UNUSED;

    static struct StepTy
    {
    } step MAYBE_UNUSED;

    static struct ReverseStepTy
    {
    } reverse_step MAYBE_UNUSED;

    static struct EmptyTy
    {
    } empty MAYBE_UNUSED;

    static struct FullTy
    {
    } full MAYBE_UNUSED;

  }  // namespace math
}  // namespace rkcommon
