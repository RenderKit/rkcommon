// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "../catch.hpp"

#include "rkcommon/math/vec.h"

using namespace rkcommon::math;

#define RUN_FOR_FP_TYPES(name) \
  name<vec2f>();               \
  name<vec2d>();               \
  name<vec3f>();               \
  name<vec3fa>();              \
  name<vec3d>();               \
  name<vec4f>();               \
  name<vec4d>()

#define RUN_FOR_INT_TYPES(name) \
  name<vec2c>();                \
  name<vec2s>();                \
  name<vec2i>();                \
  name<vec2l>();                \
  name<vec3c>();                \
  name<vec3s>();                \
  name<vec3i>();                \
  name<vec3ia>();               \
  name<vec3l>();                \
  name<vec4c>();                \
  name<vec4s>();                \
  name<vec4i>();                \
  name<vec4l>()

#define RUN_FOR_ALL_TYPES(name) \
  RUN_FOR_FP_TYPES(name);       \
  RUN_FOR_INT_TYPES(name)

template <typename T>
inline void test_unary_op()
{
  REQUIRE((T(-1)) == -T(1));
}

TEST_CASE("Vector unary operators", "[vec]")
{
  RUN_FOR_ALL_TYPES(test_unary_op);
}

#define unary_functor_test(op, v)  \
  template <typename T>            \
  inline void test_##op()          \
  {                                \
    REQUIRE(op(T(v)) == T(op(v))); \
  }

unary_functor_test(rcp, 0.5);
unary_functor_test(rcp_safe, 0.5);
unary_functor_test(abs, -1);
unary_functor_test(sin, 1.0);
unary_functor_test(cos, 1.0);
#undef unary_functor_test

TEST_CASE("Vector unary functors", "[vec]")
{
  SECTION("Test rcp")
  {
    RUN_FOR_FP_TYPES(test_rcp);
  }

  SECTION("Test rcp_safe")
  {
    RUN_FOR_FP_TYPES(test_rcp_safe);
  }

  SECTION("Test abs")
  {
    RUN_FOR_ALL_TYPES(test_abs);
  }

  SECTION("Test sin")
  {
    RUN_FOR_FP_TYPES(test_sin);
  }

  SECTION("Test cos")
  {
    RUN_FOR_FP_TYPES(test_cos);
  }
}

#define binary_op_test(name, op, a, b, r) \
  template <typename T>                   \
  inline void test_binary_##name()        \
  {                                       \
    using Scalar = typename T::scalar_t;  \
    REQUIRE(T(a) op T(b) == T(r));        \
    REQUIRE(T(a) op Scalar(b) == T(r));   \
    REQUIRE(Scalar(a) op T(b) == T(r));   \
  }

binary_op_test(add, +, 1, 2, 3);
binary_op_test(sub, -, 3, 2, 1);
binary_op_test(mul, *, 2, 2, 4);
binary_op_test(div, /, 6, 2, 3);
binary_op_test(mod, %, 5, 3, 2);
#undef binary_op_test

TEST_CASE("Vector binary arithmetic operators", "[vec]")
{
  SECTION("Test add")
  {
    RUN_FOR_ALL_TYPES(test_binary_add);
  }

  SECTION("Test sub")
  {
    RUN_FOR_ALL_TYPES(test_binary_sub);
  }

  SECTION("Test mul")
  {
    RUN_FOR_ALL_TYPES(test_binary_mul);
  }

  SECTION("Test div")
  {
    RUN_FOR_ALL_TYPES(test_binary_div);
  }

  SECTION("Test mod")
  {
    RUN_FOR_INT_TYPES(test_binary_mod);
  }
}

#define assign_op_test(name, op, a, b, r) \
  template <typename T>                   \
  inline void test_assign_##name()        \
  {                                       \
    T a1(a), a2(a);                       \
    a1 op T(b);                           \
    a2 op b;                              \
    REQUIRE(a1 == T(r));                  \
    REQUIRE(a2 == T(r));                  \
  }

assign_op_test(add, +=, 1, 2, 3);
assign_op_test(sub, -=, 3, 2, 1);
assign_op_test(mul, *=, 2, 2, 4);
assign_op_test(div, /=, 6, 2, 3);
assign_op_test(mod, %=, 5, 3, 2);
#undef assign_op_test

TEST_CASE("Vector arithmetic assignment operators", "[vec]")
{
  SECTION("Test add")
  {
    RUN_FOR_ALL_TYPES(test_assign_add);
  }

  SECTION("Test sub")
  {
    RUN_FOR_ALL_TYPES(test_assign_sub);
  }

  SECTION("Test mul")
  {
    RUN_FOR_ALL_TYPES(test_assign_mul);
  }

  SECTION("Test div")
  {
    RUN_FOR_ALL_TYPES(test_assign_div);
  }

  SECTION("Test mod")
  {
    RUN_FOR_INT_TYPES(test_assign_mod);
  }
}

#define compare_op_test(name, op, a, b) \
  template <typename T>                 \
  inline void test_compare_##name()     \
  {                                     \
    REQUIRE(T(a) op T(b));              \
  }

compare_op_test(equal, ==, 1, 1);
compare_op_test(notEqual, !=, 1, 2);
#undef compare_op_test

template <typename T>
inline void test_compare_anyLessThan()
{
  REQUIRE(anyLessThan(T(1), T(2)));
}

TEST_CASE("Vector comparison operators", "[vec]")
{
  SECTION("Test equal")
  {
    RUN_FOR_ALL_TYPES(test_compare_equal);
  }

  SECTION("Test not equal")
  {
    RUN_FOR_ALL_TYPES(test_compare_notEqual);
  }

  SECTION("Test anyLessThan")
  {
    RUN_FOR_ALL_TYPES(test_compare_anyLessThan);
  }
}

template <typename T>
inline void test_dot()
{
  using Scalar = typename T::scalar_t;
  REQUIRE(dot(T(0), T(0)) == Scalar(0));
}

TEST_CASE("Vector dot functions", "[vec]")
{
  RUN_FOR_FP_TYPES(test_dot);
}

template <typename T>
inline void test_length()
{
  using Scalar = typename T::scalar_t;
  REQUIRE(length(T(0)) == Scalar(0));
}

TEST_CASE("Vector length functions", "[vec]")
{
  RUN_FOR_FP_TYPES(test_length);
}

template <typename T>
inline void test_cross()
{
  REQUIRE(cross(T(1, 0, 0), T(0, 1, 0)) == T(0, 0, 1));
}

TEST_CASE("Vector cross product", "[vec]")
{
  test_cross<vec3f>();
  test_cross<vec3d>();
}

template <typename T>
inline void test_normalize()
{
  using Scalar = typename T::scalar_t;
  REQUIRE(abs(Scalar(1) - length(normalize(T(1)))) < 1e-6);
  REQUIRE(abs(Scalar(1) - length(safe_normalize(T(1)))) < 1e-6);
}

TEST_CASE("Vector normalize", "[vec]")
{
  RUN_FOR_FP_TYPES(test_normalize);
}

template <typename T>
inline void test_interpolate()
{
  using Scalar = typename T::scalar_t;
  using Vector = vec_t<Scalar, 3>;
  REQUIRE(interpolate_uv(Vector(0.3, 0.2, 0.1), T(1), T(2), T(3)) == T(1));
}

TEST_CASE("Vector interpolation", "[vec]")
{
  RUN_FOR_FP_TYPES(test_interpolate);
}

#define binary_func_test(name, a, b, r)  \
  template <typename T>                  \
  inline void test_binary_##name()       \
  {                                      \
    using Scalar = typename T::scalar_t; \
    REQUIRE(name(T(a), T(b)) == T(r));   \
  }

binary_func_test(min, 1, 2, 1);
binary_func_test(max, 1, 2, 2);
binary_func_test(divRoundUp, 3, 2, 2);
#undef binary_func_test

TEST_CASE("Vector binary functors", "[vec]")
{
  SECTION("Test min")
  {
    RUN_FOR_ALL_TYPES(test_binary_min);
  }

  SECTION("Test max")
  {
    RUN_FOR_ALL_TYPES(test_binary_max);
  }

  SECTION("Test divRoundUp")
  {
    RUN_FOR_ALL_TYPES(test_binary_divRoundUp);
  }
}

#define reduce_test(dim, op, r, ...)           \
  template <typename T>                        \
  inline void test_reduce_##op##_##dim()       \
  {                                            \
    REQUIRE(reduce_##op(T(__VA_ARGS__)) == r); \
  }

reduce_test(2, add, 3, 1, 2);
reduce_test(2, mul, 4, 2, 2);
reduce_test(2, min, 1, 1, 2);
reduce_test(2, max, 2, 1, 2);

reduce_test(3, add, 6, 1, 2, 3);
reduce_test(3, mul, 8, 2, 2, 2);
reduce_test(3, min, 1, 1, 2, 3);
reduce_test(3, max, 3, 1, 2, 3);

reduce_test(4, add, 10, 1, 2, 3, 4);
reduce_test(4, mul, 16, 2, 2, 2, 2);
reduce_test(4, min, 1, 1, 2, 3, 4);
reduce_test(4, max, 4, 1, 2, 3, 4);
#undef reduce_test

TEST_CASE("Vector reductions", "[vec]")
{
  SECTION("Test reduce_add")
  {
    test_reduce_add_2<vec2c>();
    test_reduce_add_2<vec2s>();
    test_reduce_add_2<vec2i>();
    test_reduce_add_2<vec2l>();
    test_reduce_add_2<vec2f>();
    test_reduce_add_2<vec2d>();

    test_reduce_add_3<vec3c>();
    test_reduce_add_3<vec3s>();
    test_reduce_add_3<vec3i>();
    test_reduce_add_3<vec3ia>();
    test_reduce_add_3<vec3l>();
    test_reduce_add_3<vec3f>();
    test_reduce_add_3<vec3fa>();
    test_reduce_add_3<vec3d>();

    test_reduce_add_4<vec4c>();
    test_reduce_add_4<vec4s>();
    test_reduce_add_4<vec4i>();
    test_reduce_add_4<vec4l>();
    test_reduce_add_4<vec4f>();
    test_reduce_add_4<vec4d>();
  }

  SECTION("Test reduce_mul")
  {
    test_reduce_mul_2<vec2c>();
    test_reduce_mul_2<vec2s>();
    test_reduce_mul_2<vec2i>();
    test_reduce_mul_2<vec2l>();
    test_reduce_mul_2<vec2f>();
    test_reduce_mul_2<vec2d>();

    test_reduce_mul_3<vec3c>();
    test_reduce_mul_3<vec3s>();
    test_reduce_mul_3<vec3i>();
    test_reduce_mul_3<vec3ia>();
    test_reduce_mul_3<vec3l>();
    test_reduce_mul_3<vec3f>();
    test_reduce_mul_3<vec3fa>();
    test_reduce_mul_3<vec3d>();

    test_reduce_mul_4<vec4c>();
    test_reduce_mul_4<vec4s>();
    test_reduce_mul_4<vec4i>();
    test_reduce_mul_4<vec4l>();
    test_reduce_mul_4<vec4f>();
    test_reduce_mul_4<vec4d>();
  }

  SECTION("Test reduce_min")
  {
    test_reduce_min_2<vec2c>();
    test_reduce_min_2<vec2s>();
    test_reduce_min_2<vec2i>();
    test_reduce_min_2<vec2l>();
    test_reduce_min_2<vec2f>();
    test_reduce_min_2<vec2d>();

    test_reduce_min_3<vec3c>();
    test_reduce_min_3<vec3s>();
    test_reduce_min_3<vec3i>();
    test_reduce_min_3<vec3ia>();
    test_reduce_min_3<vec3l>();
    test_reduce_min_3<vec3f>();
    test_reduce_min_3<vec3fa>();
    test_reduce_min_3<vec3d>();

    test_reduce_min_4<vec4c>();
    test_reduce_min_4<vec4s>();
    test_reduce_min_4<vec4i>();
    test_reduce_min_4<vec4l>();
    test_reduce_min_4<vec4f>();
    test_reduce_min_4<vec4d>();
  }

  SECTION("Test reduce_max")
  {
    test_reduce_max_2<vec2c>();
    test_reduce_max_2<vec2s>();
    test_reduce_max_2<vec2i>();
    test_reduce_max_2<vec2l>();
    test_reduce_max_2<vec2f>();
    test_reduce_max_2<vec2d>();

    test_reduce_max_3<vec3c>();
    test_reduce_max_3<vec3s>();
    test_reduce_max_3<vec3i>();
    test_reduce_max_3<vec3ia>();
    test_reduce_max_3<vec3l>();
    test_reduce_max_3<vec3f>();
    test_reduce_max_3<vec3fa>();
    test_reduce_max_3<vec3d>();

    test_reduce_max_4<vec4c>();
    test_reduce_max_4<vec4s>();
    test_reduce_max_4<vec4i>();
    test_reduce_max_4<vec4l>();
    test_reduce_max_4<vec4f>();
    test_reduce_max_4<vec4d>();
  }
}