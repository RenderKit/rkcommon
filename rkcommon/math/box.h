// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "range.h"
#include "vec.h"

namespace rkcommon {
  namespace math {

    // box declaration ////////////////////////////////////////////////////////

    template <typename T, int N, bool ALIGN = false>
    using box_t = range_t<vec_t<T, N, ALIGN>>;

    // box free functions /////////////////////////////////////////////////////

    template <typename scalar_t>
    inline scalar_t area(const box_t<scalar_t, 2> &b)
    {
      return b.size().product();
    }

    template <typename scalar_t, bool A>
    inline scalar_t area(const box_t<scalar_t, 3, A> &b)
    {
      const auto size = b.size();
      return 2.f * (size.x * size.y + size.x * size.z + size.y * size.z);
    }

    /*! return the volume of the 3D box - undefined for empty boxes */
    template <typename scalar_t, bool A>
    inline scalar_t volume(const box_t<scalar_t, 3, A> &b)
    {
      return b.size().product();
    }

    /*! computes whether two boxes are either touching OR overlapping;
        ie, the case where boxes just barely touch side-by side (even if
        they do not have any actual overlapping _volume_!) then this is
        still true */
    template <typename scalar_t, bool A>
    inline bool touchingOrOverlapping(const box_t<scalar_t, 3, A> &a,
                                      const box_t<scalar_t, 3, A> &b)
    {
      if (a.lower.x > b.upper.x)
        return false;
      if (a.lower.y > b.upper.y)
        return false;
      if (a.lower.z > b.upper.z)
        return false;

      if (b.lower.x > a.upper.x)
        return false;
      if (b.lower.y > a.upper.y)
        return false;
      if (b.lower.z > a.upper.z)
        return false;

      return true;
    }

    template <typename scalar_t, bool A>
    inline bool touchingOrOverlapping(const box_t<scalar_t, 2, A> &a,
                                      const box_t<scalar_t, 2, A> &b)
    {
      if (a.lower.x > b.upper.x)
        return false;
      if (a.lower.y > b.upper.y)
        return false;

      if (b.lower.x > a.upper.x)
        return false;
      if (b.lower.y > a.upper.y)
        return false;

      return true;
    }

    /*! compute the intersection of two boxes */
    template <typename T, int N, bool A>
    inline box_t<T, N, A> intersectionOf(const box_t<T, N, A> &a,
                                         const box_t<T, N, A> &b)
    {
      return box_t<T, N, A>(max(a.lower, b.lower), min(a.upper, b.upper));
    }

    template <typename T, int N, bool A>
    inline bool disjoint(const box_t<T, N, A> &a, const box_t<T, N, A> &b)
    {
      return anyLessThan(a.upper, b.lower) || anyLessThan(b.upper, a.lower);
    }

    /*! returns the center of the box (not valid for empty boxes) */
    template <typename T, int N, bool A>
    inline vec_t<T, N, A> center(const box_t<T, N, A> &b)
    {
      return b.center();
    }

    template <typename T, int N>
    inline range_t<T> intersectRayBox(
        const vec_t<T, N> &org,
        const vec_t<T, N> &dir,
        const box_t<T, N> &box,
        const range_t<T> &tRange = range_t<T>(0, inf))
    {
      const auto mins = (box.lower - org) * rcp_safe(dir);
      const auto maxs = (box.upper - org) * rcp_safe(dir);
      return range_t<T>(
          reduce_max(vec_t<T, N + 1>(min(mins, maxs), tRange.lower)),
          reduce_min(vec_t<T, N + 1>(max(mins, maxs), tRange.upper)));
    }

    using box1i  = range_t<int32_t>;
    using box2i  = box_t<int32_t, 2>;
    using box3i  = box_t<int32_t, 3>;
    using box4i  = box_t<int32_t, 4>;
    using box1f  = range_t<float>;
    using box2f  = box_t<float, 2>;
    using box3f  = box_t<float, 3>;
    using box4f  = box_t<float, 4>;
    using box3fa = box_t<float, 3, 1>;

    // this is just a renaming - in some cases the code reads cleaner if
    // we're talking about 'regions' than about boxes
    using region2i = box2i;

  }  // namespace math
}  // namespace rkcommon
