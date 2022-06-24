// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"
#include "rkcommon/math/rkmath.h"

using namespace rkcommon::math;

template <typename T>
inline bool CmpT(const T a, const T b, T epsilon = ulp)
{
  return (abs(b - a) > epsilon) ? false : true;
}

TEST_CASE("rkmath sign function", "[rkmath]")
{
  REQUIRE(sign(1.f) == 1.f);
  REQUIRE(sign(-1.f) == -1.f);
  REQUIRE(sign(0.f) == 1.f);
}

template <typename T>
inline void test_rcp()
{
  REQUIRE(CmpT<T>(rcp(T(1)), T(1)));
  REQUIRE(CmpT<T>(rcp(T(2)), T(.5)));
}

TEST_CASE("rkmath rcp function", "[rkmath]")
{
  test_rcp<float>();
  test_rcp<double>();
}

template <typename T>
inline void test_rcp_safe()
{
  REQUIRE(rcp_safe(T(0)) < T(inf));
  REQUIRE(CmpT<T>(rcp_safe(T(1)), T(1)));
  REQUIRE(CmpT<T>(rcp_safe(T(2)), T(.5)));
}

TEST_CASE("rkmath rcp_safe function", "[rkmath]")
{
  test_rcp_safe<float>();
  test_rcp_safe<double>();
}

template <typename T>
inline void test_rsqrt()
{
  REQUIRE(CmpT<T>(rsqrt(T(1)), T(1)));
  REQUIRE(CmpT<T>(rsqrt(T(4)), T(.5)));
}

TEST_CASE("rkmath rsqrt function", "[rkmath]")
{
  test_rsqrt<float>();
  test_rsqrt<double>();
}

template <typename T>
inline void test_clamp()
{
  REQUIRE(clamp(T(2)) == T(1));
  REQUIRE(clamp(T(-2)) == T(0));
  REQUIRE(clamp(T(10), T(1), T(2)) == T(2));
  REQUIRE(clamp(T(0), T(1), T(2)) == T(1));
}

TEST_CASE("rkmath clamp function", "[rkmath]")
{
  test_clamp<float>();
  test_clamp<double>();
}

template <typename T>
inline void test_deg2rad()
{
  REQUIRE(deg2rad(T(90)) == T(half_pi));
  REQUIRE(deg2rad(T(45)) == T(quarter_pi));
}

TEST_CASE("rkmath deg2rad function", "[rkmath]")
{
  test_deg2rad<float>();
  test_deg2rad<double>();
}

template <typename T>
inline void test_lerp()
{
  REQUIRE(lerp(.5f, T(0), T(10)) == T(5));
  REQUIRE(lerp(0.f, T(0), T(10)) == T(0));
  REQUIRE(lerp(1.f, T(0), T(10)) == T(10));
}

TEST_CASE("rkmath lerp function", "[rkmath]")
{
  test_lerp<float>();
  test_lerp<double>();
}

template <typename T>
inline void test_divRoundUp()
{
  REQUIRE(divRoundUp(T(5), T(2)) == T(3));
}

TEST_CASE("rkmath divRoundUp function", "[rkmath]")
{
  test_divRoundUp<float>();
  test_divRoundUp<double>();
}
