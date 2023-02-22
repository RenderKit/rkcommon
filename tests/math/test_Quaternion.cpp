// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"
#include "rkcommon/math/Quaternion.h"

using namespace rkcommon::math;

template <typename T>
inline bool CmpT(const QuaternionT<T> &a, const QuaternionT<T> &b, T epsilon = ulp)
{
  QuaternionT<T> d = b - a;
  if ((abs(d.r) > epsilon) || (abs(d.i) > epsilon) || (abs(d.j) > epsilon)
        || (abs(d.k) > epsilon)) return false;

  return true;
}

///////////////////////////////////////////////////////////////////////////////
// Constructors

template <typename T>
inline void test_constructFromVectors()
{
  using V = typename T::Vector;
  REQUIRE(CmpT(T(V(1, 0, 0), V(0, 1, 0), V(0, 0, 1)), T(1, 0, 0, 0)));
}

template <typename T>
inline void test_constructFromAngles()
{
  using S = typename T::Scalar;
  REQUIRE(CmpT(T(0, S(half_pi), -S(half_pi)), T(.5, .5, .5, -.5)));
}

TEST_CASE("Quaternion constructors", "[quat]")
{
  SECTION("Test from 3 vectors")
  {
    test_constructFromVectors<quatf>();
    test_constructFromVectors<quatd>();
  }

  SECTION("Test from yaw, pitch, roll")
  {
    test_constructFromAngles<quatf>();
    test_constructFromAngles<quatd>();
  }
}

///////////////////////////////////////////////////////////////////////////////
// Operators

template <typename T>
inline void test_unary_neg()
{
  REQUIRE(T(-1, -1, -1, -1) == -T(1, 1, 1, 1));
}

template <typename T>
inline void test_binary_add()
{
  using S = typename T::Scalar;
  REQUIRE(T(1, 1, 1, 1) + T(2, 2, 2, 2) == T(3, 3, 3, 3));
  REQUIRE(T(1, 1, 1, 1) + S(2) == T(3, 1, 1, 1));
  REQUIRE(S(1) + T(2, 2, 2, 2) == T(3, 2, 2, 2));
}

template <typename T>
inline void test_binary_sub()
{
  using S = typename T::Scalar;
  REQUIRE(T(3, 3, 3, 3) - T(1, 1, 1, 1) == T(2, 2, 2, 2));
  REQUIRE(T(3, 3, 3, 3) - S(1) == T(2, 3, 3, 3));
  REQUIRE(S(3) - T(1, 1, 1, 1) == T(2, -1, -1, -1));
}

template <typename T>
inline void test_binary_mul()
{
  using S = typename T::Scalar;
  using V = typename T::Vector;
  REQUIRE(T(1, 1, 1, 1) * S(2) == T(2, 2, 2, 2));
  REQUIRE(S(2) * T(1, 1, 1, 1) == T(2, 2, 2, 2));
  REQUIRE(T(1, 1, 1, 1) * 2.f == T(2, 2, 2, 2));
  REQUIRE(2.f * T(1, 1, 1, 1) == T(2, 2, 2, 2));
  REQUIRE(T(.5, .5, .5, .5) * V(1, 0, 0) == V(0, 1, 0));
  REQUIRE(T(1, 1, 1, 1) * T(2, 2, 2, 2) == T(-4, 4, 4, 4));
}

template <typename T>
inline void test_binary_div()
{
  using S = typename T::Scalar;
  REQUIRE(CmpT(T(2, 2, 2, 2) / T(1, 1, 1, 1), T(2, 0, 0, 0)));
  REQUIRE(CmpT(T(2, 2, 2, 2) / S(1), T(2, 2, 2, 2)));
  REQUIRE(CmpT(S(2) / T(1, 1, 1, 1), T(.5, -.5, -.5, -.5)));
}

TEST_CASE("Quaternion operators", "[quat]")
{
  SECTION("Test unary neg")
  {
    test_unary_neg<quatf>();
    test_unary_neg<quatd>();
  }

  SECTION("Test binary add")
  {
    test_binary_add<quatf>();
    test_binary_add<quatd>();
  }

  SECTION("Test binary sub")
  {
    test_binary_sub<quatf>();
    test_binary_sub<quatd>();
  }

  SECTION("Test binary mul")
  {
    test_binary_mul<quatf>();
    test_binary_mul<quatd>();
  }

  SECTION("Test binary div")
  {
    test_binary_div<quatf>();
    test_binary_div<quatd>();
  }
}

///////////////////////////////////////////////////////////////////////////////
// Functions
template <typename T>
inline void test_conj()
{
  REQUIRE(conj(T(1, 1, 1, 1)) == T(1, -1, -1, -1));
}

template <typename T>
inline void test_rcp()
{
  REQUIRE(CmpT(rcp(T(1, 1, 1, 1)), T(.25, -.25, -.25, -.25)));
}

template <typename T>
inline void test_abs()
{
  REQUIRE(abs(T(2, 2, 2, 2)) == 4);
}

template <typename T>
inline void test_normalize()
{
  REQUIRE(CmpT(normalize(T(1, 1, 1, 1)), T(.5, .5, .5, .5)));
}

template <typename T>
inline void test_dot()
{
  REQUIRE(dot(T(.5, .5, .5, .5), T(.5, .5, .5, .5)) == 1);
}

template <typename T>
inline void test_xfmPoint()
{
  using V = typename T::Vector;
  REQUIRE(xfmPoint(T(.5, .5, .5, .5), V(1, 0, 0)) == V(0, 1, 0));
}

template <typename T>
inline void test_xfmQuaternion()
{
  REQUIRE(xfmQuaternion(T(1, 1, 1, 1), T(2, 2, 2, 2)) == T(-4, 4, 4, 4));
}

template <typename T>
inline void test_xfmNormal()
{
  using V = typename T::Vector;
  REQUIRE(
      xfmNormal(T(.5, .5, .5, .5), V(1, 0, 0)) == V(0, 1, 0));
}

template <typename T>
inline void test_slerp()
{
  typename T::Scalar two = 2;
  REQUIRE(CmpT(slerp(.5f, T(1, 0, 0, 0), T(0, 1, 0, 0)), T(rsqrt(two), rsqrt(two), 0, 0)));
}

TEST_CASE("Quaternion functions", "[quat]")
{
  SECTION("Test conj")
  {
    test_conj<quatf>();
    test_conj<quatd>();
  }

  SECTION("Test rcp")
  {
    test_rcp<quatf>();
    test_rcp<quatd>();
  }

  SECTION("Test abs")
  {
    test_abs<quatf>();
    test_abs<quatd>();
  }

  SECTION("Test normalize")
  {
    test_normalize<quatf>();
    test_normalize<quatd>();
  }

  SECTION("Test dot")
  {
    test_dot<quatf>();
    test_dot<quatd>();
  }

  SECTION("Test xfmPoint")
  {
    test_xfmPoint<quatf>();
    test_xfmPoint<quatd>();
  }

  SECTION("Test xfmQuaternion")
  {
    test_xfmQuaternion<quatf>();
    test_xfmQuaternion<quatd>();
  }

  SECTION("Test xfmNormal")
  {
    test_xfmNormal<quatf>();
    test_xfmNormal<quatd>();
  }

  SECTION("Test slerp")
  {
    test_slerp<quatf>();
    test_slerp<quatd>();
  }
}
