// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../platform.h"
#include "constants.h"
// std
#include <algorithm>  // std::min()/std::max() on Windows
#include <cmath>

// Include vector intrinsics
#ifndef RKCOMMON_NO_SIMD
#if defined(_WIN32)
#include <intrin.h>
#elif defined(__ARM_NEON)
#include "arm/emulation.h"
#else
#include <emmintrin.h>
#include <xmmintrin.h>
#endif
#endif

namespace rkcommon {
  namespace math {

    using std::cos;
    using std::sin;
    using std::tan;

    using std::max;
    using std::min;

    using std::fmod;

    __forceinline float sign(const float x)
    {
      return x < 0 ? -1.0f : 1.0f;
    }

    __forceinline float rcp(const float x)
    {
#ifdef RKCOMMON_NO_SIMD
      return 1.f / x;
#else
      const __m128 a = _mm_set_ss(x);
      const __m128 r = _mm_rcp_ss(a);
      return _mm_cvtss_f32(
          _mm_mul_ss(r, _mm_sub_ss(_mm_set_ss(2.0f), _mm_mul_ss(r, a))));
#endif
    }

    __forceinline double rcp(const double x)
    {
      return 1. / x;
    }

    template <typename T>
    __forceinline T rcp_safe_t(const T x)
    {
      const T flt_min = std::numeric_limits<T>::min();
      return rcp(std::abs(x) < flt_min ? (x >= 0.f ? flt_min : -flt_min) : x);
    }

    __forceinline float rcp_safe(const float x)
    {
      return rcp_safe_t<float>(x);
    }

    __forceinline double rcp_safe(const double x)
    {
      return rcp_safe_t<double>(x);
    }

    __forceinline float rsqrt(const float x)
    {
#ifdef RKCOMMON_NO_SIMD
      return 1.f / std::sqrt(x);
#else
      const __m128 a = _mm_set_ss(x);
      const __m128 r = _mm_rsqrt_ss(a);
      const __m128 c =
          _mm_add_ss(_mm_mul_ss(_mm_set_ss(1.5f), r),
                     _mm_mul_ss(_mm_mul_ss(_mm_mul_ss(a, _mm_set_ss(-0.5f)), r),
                                _mm_mul_ss(r, r)));
      return _mm_cvtss_f32(c);
#endif
    }

    __forceinline double rsqrt(const double x)
    {
      return 1. / std::sqrt(x);
    }

    template <typename T>
    __forceinline T clamp(const T &x,
                          const T &lower = T(zero),
                          const T &upper = T(one))
    {
      return max(min(x, upper), lower);
    }

    template <typename T>
    __forceinline T deg2rad(const T &x)
    {
      return x * T(1.745329251994329576923690768489e-2);
    }

    __forceinline float madd(const float a, const float b, const float c)
    {
      return a * b + c;
    }

    template <typename T>
    inline T lerp(const float factor, const T &a, const T &b)
    {
      return (1.f - factor) * a + factor * b;
    }

    template <typename T>
    inline T divRoundUp(T a, T b)
    {
      return (a + b - 1) / b;
    }

#define APPROXIMATE_SRGB

    inline float linear_to_srgb(const float f)
    {
      const float c = std::max(f, 0.f);
#ifdef APPROXIMATE_SRGB
      return std::pow(c, 1.f / 2.2f);
#else
      return c <= 0.0031308f ? 12.92f * c
                             : std::pow(c, 1.f / 2.4f) * 1.055f - 0.055f;
#endif
    }

  }  // namespace math
}  // namespace rkcommon
