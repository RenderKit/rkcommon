// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/math/range.h"

using namespace rkcommon::math;

#define RUN_FOR_ALL_TYPES(name)                                               \
  name<range1f>();                                                            \
  name<range2f>();                                                            \
  name<range3f>();                                                            \
  name<range4f>();                                                            \
  name<range1i>();                                                            \
  name<range2i>();                                                            \
  name<range3i>();                                                            \
  name<range4i>();

template <typename T>
inline void test_size()
{
  using B = typename T::bound_t;
  REQUIRE(T(1, 2).size() == B(1));
}

TEST_CASE("range size function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_size);
}

template <typename T>
inline void test_center()
{
  using B = typename T::bound_t;
  REQUIRE(T(0, 2).center() == B(1));
}

TEST_CASE("range center function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_center);
}

template <typename T>
inline void test_extend()
{
  using B = typename T::bound_t;
  T r(0, 2);

  SECTION("by value")
  {
    r.extend(B(3));
    REQUIRE(r == T(0, 3));
  }

  SECTION("by range")
  {
    r.extend(T(-1, 3));
    REQUIRE(r == T(-1, 3));
  }
}

TEST_CASE("range extend function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_extend);
}

template <typename T>
inline void test_clamp()
{
  using B = typename T::bound_t;
  REQUIRE(T(0, 2).clamp(1) == B(1));
  REQUIRE(T(0, 2).clamp(-1) == B(0));
  REQUIRE(T(0, 2).clamp(3) == B(2));
}

TEST_CASE("range clamp function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_clamp);
}

template <typename T>
inline void test_empty()
{
  REQUIRE(T(1, 0).empty() == true);
  REQUIRE(T(1, 1).empty() == false);
  REQUIRE(T(0, 1).empty() == false);
}

TEST_CASE("range empty function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_empty);
}

template <typename T>
inline void test_contains()
{
  REQUIRE(T(0, 2).contains(-1) == false);
  REQUIRE(T(0, 2).contains(0) == true);
  REQUIRE(T(0, 2).contains(1) == true);
  REQUIRE(T(0, 2).contains(2) == true);
  REQUIRE(T(0, 2).contains(3) == false);
}

TEST_CASE("range contains function", "[range]")
{
  RUN_FOR_ALL_TYPES(test_contains);
}

template <typename T>
inline void test_scale()
{
  using B = typename T::bound_t;
  REQUIRE(T(1, 2) * B(2) == T(2, 4));
  REQUIRE(B(2) * T(1, 2) == T(2, 4));
}

TEST_CASE("range scale (*) operator", "[range]")
{
  RUN_FOR_ALL_TYPES(test_scale);
}

template <typename T>
inline void test_translate()
{
  using B = typename T::bound_t;
  REQUIRE(T(1, 2) + B(1) == T(2, 3));
  REQUIRE(B(1) + T(1, 2) == T(2, 3));
}

TEST_CASE("range translate (+) operator", "[range]")
{
  RUN_FOR_ALL_TYPES(test_translate);
}

template <typename T>
inline void test_comparison()
{
  REQUIRE(T(0, 1) == T(0, 1));
  REQUIRE(T(0, 1) != T(1, 1));
}

TEST_CASE("range comparison operators", "[range]")
{
  RUN_FOR_ALL_TYPES(test_comparison);
}
