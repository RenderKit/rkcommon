// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/math/box.h"

using namespace rkcommon::math;

#define RUN_FOR_ALL_TYPES(name)                                               \
  name<box2f>();                                                              \
  name<box3f>();                                                              \
  name<box3fa>();                                                             \
  name<box4f>();                                                              \
  name<box2i>();                                                              \
  name<box3i>();                                                              \
  name<box4i>();

template <typename T>
inline void test_area_2D()
{
  using B = typename T::bound_t;
  REQUIRE(area(T(B(1), B(2))) == 1);
}

TEST_CASE("2D box area function", "[box]")
{
  test_area_2D<box2i>();
  test_area_2D<box2f>();
}

template <typename T>
inline void test_area_3D()
{
  using B = typename T::bound_t;
  REQUIRE(area(T(B(1), B(2))) == 6);
}

TEST_CASE("3D box area function", "[box]")
{
  test_area_3D<box3i>();
  test_area_3D<box3f>();
  test_area_3D<box3fa>();
}

template <typename T>
inline void test_volume()
{
  using B = typename T::bound_t;
  REQUIRE(volume(T(B(1), B(2))) == 1);
}

TEST_CASE("3D box volume function", "[box]")
{
  test_volume<box3i>();
  test_volume<box3f>();
  test_volume<box3fa>();
}

template <typename T>
inline void test_touchingOrOverlapping()
{
  using B = typename T::bound_t;
  REQUIRE(touchingOrOverlapping(T(B(0), B(1)), T(B(2), B(3))) == false);
  REQUIRE(touchingOrOverlapping(T(B(1), B(2)), T(B(2), B(3))) == true);
  REQUIRE(touchingOrOverlapping(T(B(1), B(2)), T(B(1), B(2))) == true);
  REQUIRE(touchingOrOverlapping(T(B(2), B(3)), T(B(1), B(2))) == true);
  REQUIRE(touchingOrOverlapping(T(B(2), B(3)), T(B(0), B(1))) == false);
}

TEST_CASE("box touchingOrOverlapping function", "[box]")
{
  test_touchingOrOverlapping<box2i>();
  test_touchingOrOverlapping<box2f>();
  test_touchingOrOverlapping<box3i>();
  test_touchingOrOverlapping<box3f>();
  test_touchingOrOverlapping<box3fa>();
}

template <typename T>
inline void test_intersectionOf()
{
  using B = typename T::bound_t;
  REQUIRE(intersectionOf(T(B(0), B(2)), T(B(1), B(3))) == T(B(1), B(2)));
}

TEST_CASE("box intersectionOf function", "[box]")
{
  RUN_FOR_ALL_TYPES(test_intersectionOf);
}

template <typename T>
inline void test_disjoint()
{
  using B = typename T::bound_t;
  REQUIRE(disjoint(T(B(0), B(1)), T(B(2), B(3))) == true);
  REQUIRE(disjoint(T(B(1), B(2)), T(B(2), B(3))) == false);
  REQUIRE(disjoint(T(B(1), B(2)), T(B(1), B(2))) == false);
  REQUIRE(disjoint(T(B(2), B(3)), T(B(1), B(2))) == false);
  REQUIRE(disjoint(T(B(2), B(3)), T(B(0), B(1))) == true);
}

TEST_CASE("box disjoint function", "[box]")
{
  RUN_FOR_ALL_TYPES(test_disjoint);
}

template <typename T>
inline void test_center()
{
  using B = typename T::bound_t;
  REQUIRE(center(T(B(0), B(2))) == B(1));
}

TEST_CASE("box center function", "[box]")
{
  RUN_FOR_ALL_TYPES(test_center);
}

template <typename T>
inline bool CmpT(const T a, const T b, T epsilon = ulp)
{
  return (abs(b - a) > epsilon) ? false : true;
}

template <typename T, int N>
inline void test_intersectRayBox()
{
  using V = vec_t<T, N>;
  using X = box_t<T, N>;
  range_t<T> r = intersectRayBox(V(0), normalize(V(1)), X(V(1), V(2)));
  REQUIRE(abs(r.lower - length(V(1))) <= T(ulp));
  REQUIRE(abs(r.upper - length(V(2))) <= 2 * T(ulp)); // rcp is multiplied by box.upper
}

TEST_CASE("box intersectRayBox function", "[box]")
{
  test_intersectRayBox<float, 2>();
  test_intersectRayBox<float, 3>();
  test_intersectRayBox<double, 2>();
  test_intersectRayBox<double, 3>();
}
