// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"
#include "rkcommon/math/LinearSpace.h"

using namespace rkcommon::math;

inline bool CmpT(const linear2f &a, const linear2f &b, float epsilon = ulp)
{
  linear2f d = b - a;
  if ((reduce_max(abs(d.vx)) > epsilon) || (reduce_max(abs(d.vy)) > epsilon))
    return false;

  return true;
}

inline bool CmpT(const linear3f &a, const linear3f &b, float epsilon = ulp)
{
  linear3f d = b - a;
  if ((reduce_max(abs(d.vx)) > epsilon) || (reduce_max(abs(d.vy)) > epsilon)
      || (reduce_max(abs(d.vz)) > epsilon))
    return false;

  return true;
}

TEST_CASE("linear space constructors", "[linear]")
{
  SECTION("from vector")
  {
    REQUIRE(linear2f(linear2f::Vector(1, 3), linear2f::Vector(2, 4)) == linear2f(1, 2, 3, 4));
    REQUIRE(linear3f(linear3f::Vector(1, 4, 7),
                linear3f::Vector(2, 5, 8),
                linear3f::Vector(3, 6, 9))
        == linear3f(1, 2, 3, 4, 5, 6, 7, 8, 9));
  }

  SECTION("from quaternion")
  {
    REQUIRE(linear3f(quatf(.5f, .5f, .5f, .5f)) == linear3f(0, 0, 1, 1, 0, 0, 0, 1, 0));
  }
}

TEST_CASE("linear space functions", "[linear]")
{
  linear2f l2f(2, 1, 1, 2);
  linear3f l3f(2, 1, 1, 1, 2, 1, 1, 1, 2);

  SECTION("det")
  {
    REQUIRE(linear2f(one).det() == 1);
    REQUIRE(linear3f(one).det() == 1);
    REQUIRE(l2f.det() == 3);
    REQUIRE(l3f.det() == 4);
  }

  SECTION("adjoint")
  {
    REQUIRE(l2f.adjoint() == linear2f(2, -1, -1, 2));
    REQUIRE(l3f.adjoint() == linear3f(3, -1, -1, -1, 3, -1, -1, -1, 3));
  }

  SECTION("inverse")
  {
    REQUIRE(l2f.inverse() == (1. / 3.) * linear2f(2, -1, -1, 2));
    REQUIRE(
        l3f.inverse() == (1. / 4.) * linear3f(3, -1, -1, -1, 3, -1, -1, -1, 3));
  }

  SECTION("transposed")
  {
    REQUIRE(l2f.transposed() == l2f);
    REQUIRE(l3f.transposed() == l3f);
  }
}

TEST_CASE("linear space constants", "[linear]")
{
  SECTION("scale")
  {
    REQUIRE(linear2f::scale(vec2f(2, 3)) == linear2f(2, 0, 0, 3));
    REQUIRE(linear3f::scale(vec3f(2, 3, 4)) == linear3f(2, 0, 0, 0, 3, 0, 0, 0, 4));
  }

  SECTION("rotate")
  {
    REQUIRE(CmpT(linear2f::rotate(half_pi), linear2f(0, -1, 1, 0)));
    REQUIRE(CmpT(linear3f::rotate(vec3f(0, 1, 0), half_pi),
        linear3f(0, 0, 1, 0, 1, 0, -1, 0, 0),
        2 * float(ulp)));
  }
}

TEST_CASE("linear space unary operators", "[linear]")
{
  linear2f l2f(1, 2, 3, 4);
  linear3f l3f(1, 2, 3, 4, 5, 6, 7, 8, 9);

  SECTION("operator-")
  {
    REQUIRE(-l2f == linear2f(-1, -2, -3, -4));
    REQUIRE(-l3f == linear3f(-1, -2, -3, -4, -5, -6, -7, -8, -9));
  }

  SECTION("operator+")
  {
    REQUIRE(+l2f == l2f);
    REQUIRE(+l3f == l3f);
  }

  SECTION("rcp")
  {
    REQUIRE(rcp(l2f) == l2f.inverse());
    REQUIRE(rcp(l3f) == l3f.inverse());
  }

  SECTION("frame")
  {
    REQUIRE(CmpT(frame(vec3f(0, 0, 1)), linear3f(1, 0, 0, 0, 1, 0, 0, 0, 1)));
    REQUIRE(CmpT(frame(vec3f(0, 0, 1), vec3f(0, 1, 0)), linear3f(1, 0, 0, 0, 1, 0, 0, 0, 1)));
  }

  SECTION("clamp")
  {
    REQUIRE(clamp(l3f) == linear3f(1, 1, 1, 1, 1, 1, 1, 1, 1));
  }
}

TEST_CASE("linear space binary operators", "[linear]")
{
  linear2f l2f(2, 1, 1, 2);
  linear3f l3f(2, 1, 1, 1, 2, 1, 1, 1, 2);

  SECTION("operator+")
  {
    REQUIRE(l2f + l2f == linear2f(4, 2, 2, 4));
    REQUIRE(l3f + l3f == linear3f(4, 2, 2, 2, 4, 2, 2, 2, 4));
  }

  SECTION("operator-")
  {
    REQUIRE(l2f - l2f == linear2f(zero));
    REQUIRE(l3f - l3f == linear3f(zero));
  }

  SECTION("operator* and operator*=")
  {
    REQUIRE(2.f * l2f == linear2f(4, 2, 2, 4));
    REQUIRE(2.f * l3f == linear3f(4, 2, 2, 2, 4, 2, 2, 2, 4));

    REQUIRE(l2f * linear2f::Vector(2, 2) == vec2f(6, 6));
    REQUIRE(l3f * linear3f::Vector(2, 2, 2) == vec3f(8, 8, 8));

    REQUIRE(l2f * l2f == linear2f(5, 4, 4, 5));
    l2f *= l2f;
    REQUIRE(l2f == linear2f(5, 4, 4, 5));

    REQUIRE(l3f * l3f == linear3f(6, 5, 5, 5, 6, 5, 5, 5, 6));
    l3f *= l3f;
    REQUIRE(l3f == linear3f(6, 5, 5, 5, 6, 5, 5, 5, 6));
  }

  SECTION("operator/ and operator/=")
  {
    linear2f two_l2f = 2 * l2f;
    linear3f two_l3f = 2 * l3f;
    REQUIRE(two_l2f / 2 == l2f);
    REQUIRE(two_l3f / 2 == l3f);

    REQUIRE(l2f / l2f == linear2f(one));
    l2f /= l2f;
    REQUIRE(l2f == linear2f(one));

    REQUIRE(l3f / l3f == linear3f(one));
    l3f /= l3f;
    REQUIRE(l3f == linear3f(one));
  }

  SECTION("transformations")
  {
    const linear3f rotateY90 = linear3f(0, 0, 1, 0, 1, 0, -1, 0, 0);
    REQUIRE(xfmPoint(rotateY90, vec3f(0, 0, 1)) == vec3f(1, 0, 0));
    REQUIRE(xfmVector(rotateY90, vec3f(0, 0, 1)) == vec3f(1, 0, 0));
    REQUIRE(xfmNormal(rotateY90, vec3f(0, 0, 1)) == vec3f(1, 0, 0));
  }
}