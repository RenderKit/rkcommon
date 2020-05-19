// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// stl
#include <algorithm>
// common
#include "constants.h"
#include "vec.h"

namespace rkcommon {
  namespace math {

    using std::max;
    using std::min;

    /*! default implementatoin of 'anyLessThan' for scalar types, so we
        can make a range<float>s etc. Vec-types will overwrite that and
        test if _any_ dimension is less */
    template <typename TA, typename TB>
    inline bool anyLessThan(const TA &a, const TB &b)
    {
      return a < b;
    }

    template <typename T>
    struct range_t
    {
      range_t() : lower(pos_inf), upper(neg_inf) {}
      range_t(const EmptyTy &) : lower(pos_inf), upper(neg_inf) {}
      range_t(const ZeroTy &) : lower(zero), upper(zero) {}
      range_t(const OneTy &) : lower(zero), upper(one) {}
      range_t(const T &t) : lower(t), upper(t) {}
      range_t(const T &_lower, const T &_upper) : lower(_lower), upper(_upper)
      {
      }
      range_t(const T *v) : lower(v[0]), upper(v[1]) {}

      template <typename other_t>
      explicit range_t(const range_t<other_t> &other)
          : lower(T(other.lower)), upper(T(other.upper))
      {
      }

      inline T size() const
      {
        return upper - lower;
      }

      inline T center() const
      {
        return .5f * (lower + upper);
      }

      inline void extend(const T &t)
      {
        lower = min(lower, t);
        upper = max(upper, t);
      }

      inline void extend(const range_t<T> &t)
      {
        lower = min(lower, t.lower);
        upper = max(upper, t.upper);
      }

      /*! take given value t, and 'clamp' it to 'this->'range; ie, if it
          already is inside the range return as is, otherwise move it to
          either lower or upper of this range. Warning: the value
          returned by this can be 'upper', which is NOT strictly part of
          the range! */
      inline T clamp(const T &t) const
      {
        return max(lower, min(t, upper));
      }

      /*! Try to parse given string into a range; and return if
        successful. if not, return defaultvalue */
      static range_t<T> fromString(
          const std::string &string,
          const range_t<T> &defaultValue = rkcommon::math::empty);

      inline vec_t<T, 2> toVec2() const
      {
        return vec_t<T, 2>(lower, upper);
      }

      inline bool empty() const
      {
        return anyLessThan(upper, lower);
      }

      inline bool contains(const T &vec) const
      {
        return !anyLessThan(vec, lower) && !anyLessThan(upper, vec);
      }

      inline operator T*()
      {
        return static_cast<T*>(&lower);
      }

      inline operator const T*() const
      {
        return static_cast<const T*>(&lower);
      }

      T lower, upper;
    };

    template <typename T>
    inline std::ostream &operator<<(std::ostream &o, const range_t<T> &r)
    {
      o << "[" << r.lower << "," << r.upper << "]";
      return o;
    }

    /*! scale range, per dimension */
    template <typename T>
    inline range_t<T> operator*(const range_t<T> &range, const T &scale)
    {
      return range_t<T>(range.lower * scale, range.upper * scale);
    }

    /*! scale range, per dimension */
    template <typename T>
    inline range_t<T> operator*(const T &scale, const range_t<T> &range)
    {
      return range_t<T>(range.lower * scale, range.upper * scale);
    }

    /*! translate a range, per dimension */
    template <typename T>
    inline range_t<T> operator+(const range_t<T> &range, const T &translation)
    {
      return range_t<T>(range.lower + translation, range.upper + translation);
    }

    /*! translate a range, per dimension */
    template <typename T>
    inline range_t<T> operator+(const T &translation, const range_t<T> &range)
    {
      return range_t<T>(range.lower + translation, range.upper + translation);
    }

    // range_t aliases ////////////////////////////////////////////////////////

    using range1f = range_t<float>;
    using range2f = range_t<vec2f>;
    using range3f = range_t<vec3f>;
    using range4f = range_t<vec4f>;
    using range1i = range_t<int>;
    using range2i = range_t<vec2i>;
    using range3i = range_t<vec3i>;
    using range4i = range_t<vec4i>;

  }  // namespace math
}  // namespace rkcommon
