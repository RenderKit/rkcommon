// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "vec.h"

namespace rkcommon {
  namespace math {

    template <typename T>
    struct QuaternionT
    {
      using Vector = vec_t<T, 3>;

      QuaternionT() {}
      QuaternionT(const QuaternionT &other)
      {
        r = other.r;
        i = other.i;
        j = other.j;
        k = other.k;
      }
      QuaternionT &operator=(const QuaternionT &other)
      {
        r = other.r;
        i = other.i;
        j = other.j;
        k = other.k;
        return *this;
      }

      QuaternionT(const T &r) : r(r), i(zero), j(zero), k(zero) {}
      explicit QuaternionT(const Vector &v) : r(zero), i(v.x), j(v.y), k(v.z) {}
      QuaternionT(const T &r, const T &i, const T &j, const T &k)
          : r(r), i(i), j(j), k(k)
      {
      }
      QuaternionT(const T &r, const Vector &v) : r(r), i(v.x), j(v.y), k(v.z) {}

      QuaternionT(const Vector &vx, const Vector &vy, const Vector &vz);
      QuaternionT(const T &yaw, const T &pitch, const T &roll);

      QuaternionT(ZeroTy) : r(zero), i(zero), j(zero), k(zero) {}
      QuaternionT(OneTy) : r(one), i(zero), j(zero), k(zero) {}

      static inline QuaternionT rotate(const Vector &u, const T &r)
      {
        return QuaternionT<T>(cos(T(0.5) * r), sin(T(0.5) * r) * normalize(u));
      }

      /*! returns the rotation axis of the quaternion as a vector */
      const Vector v() const
      {
        return Vector(i, j, k);
      }

      T i, j, k, r;
    };

    template <typename T>
    inline QuaternionT<T> operator*(const T &a, const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a * b.r, a * b.i, a * b.j, a * b.k);
    }
    template <typename T>
    inline QuaternionT<T> operator*(const QuaternionT<T> &a, const T &b)
    {
      return QuaternionT<T>(a.r * b, a.i * b, a.j * b, a.k * b);
    }

    template <typename T>
    inline QuaternionT<T> operator+(const QuaternionT<T> &a)
    {
      return QuaternionT<T>(+a.r, +a.i, +a.j, +a.k);
    }
    template <typename T>
    inline QuaternionT<T> operator-(const QuaternionT<T> &a)
    {
      return QuaternionT<T>(-a.r, -a.i, -a.j, -a.k);
    }
    template <typename T>
    inline QuaternionT<T> conj(const QuaternionT<T> &a)
    {
      return QuaternionT<T>(a.r, -a.i, -a.j, -a.k);
    }
    template <typename T>
    inline T abs(const QuaternionT<T> &a)
    {
      return sqrt(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
    }
    template <typename T>
    inline QuaternionT<T> rcp(const QuaternionT<T> &a)
    {
      return conj(a) * rcp(a.r * a.r + a.i * a.i + a.j * a.j + a.k * a.k);
    }
    template <typename T>
    inline float dot(const QuaternionT<T> &a, const QuaternionT<T> &b)
    {
      return a.r * b.r + a.i * b.i + a.j * b.j + a.k * b.k;
    }
    template <typename T>
    inline QuaternionT<T> normalize(const QuaternionT<T> &a)
    {
      return a * rsqrt(dot(a, a));
    }

    template <typename T>
    inline QuaternionT<T> operator+(const T &a, const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a + b.r, b.i, b.j, b.k);
    }
    template <typename T>
    inline QuaternionT<T> operator+(const QuaternionT<T> &a, const T &b)
    {
      return QuaternionT<T>(a.r + b, a.i, a.j, a.k);
    }
    template <typename T>
    inline QuaternionT<T> operator+(const QuaternionT<T> &a,
                                    const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a.r + b.r, a.i + b.i, a.j + b.j, a.k + b.k);
    }
    template <typename T>
    inline QuaternionT<T> operator-(const T &a, const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a - b.r, -b.i, -b.j, -b.k);
    }
    template <typename T>
    inline QuaternionT<T> operator-(const QuaternionT<T> &a, const T &b)
    {
      return QuaternionT<T>(a.r - b, a.i, a.j, a.k);
    }
    template <typename T>
    inline QuaternionT<T> operator-(const QuaternionT<T> &a,
                                    const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a.r - b.r, a.i - b.i, a.j - b.j, a.k - b.k);
    }

    template <typename T>
    inline typename QuaternionT<T>::Vector operator*(
        const QuaternionT<T> &a, const typename QuaternionT<T>::Vector &b)
    {
      return (a * QuaternionT<T>(b) * conj(a)).v();
    }
    template <typename T>
    inline QuaternionT<T> operator*(const QuaternionT<T> &a,
                                    const QuaternionT<T> &b)
    {
      return QuaternionT<T>(a.r * b.r - a.i * b.i - a.j * b.j - a.k * b.k,
                            a.r * b.i + a.i * b.r + a.j * b.k - a.k * b.j,
                            a.r * b.j - a.i * b.k + a.j * b.r + a.k * b.i,
                            a.r * b.k + a.i * b.j - a.j * b.i + a.k * b.r);
    }
    template <typename T>
    inline QuaternionT<T> operator/(const T &a, const QuaternionT<T> &b)
    {
      return a * rcp(b);
    }
    template <typename T>
    inline QuaternionT<T> operator/(const QuaternionT<T> &a, const T &b)
    {
      return a * rcp(b);
    }
    template <typename T>
    inline QuaternionT<T> operator/(const QuaternionT<T> &a,
                                    const QuaternionT<T> &b)
    {
      return a * rcp(b);
    }

    template <typename T>
    inline QuaternionT<T> &operator+=(QuaternionT<T> &a, const T &b)
    {
      return a = a + b;
    }
    template <typename T>
    inline QuaternionT<T> &operator+=(QuaternionT<T> &a,
                                      const QuaternionT<T> &b)
    {
      return a = a + b;
    }
    template <typename T>
    inline QuaternionT<T> &operator-=(QuaternionT<T> &a, const T &b)
    {
      return a = a - b;
    }
    template <typename T>
    inline QuaternionT<T> &operator-=(QuaternionT<T> &a,
                                      const QuaternionT<T> &b)
    {
      return a = a - b;
    }
    template <typename T>
    inline QuaternionT<T> &operator*=(QuaternionT<T> &a, const T &b)
    {
      return a = a * b;
    }
    template <typename T>
    inline QuaternionT<T> &operator*=(QuaternionT<T> &a,
                                      const QuaternionT<T> &b)
    {
      return a = a * b;
    }
    template <typename T>
    inline QuaternionT<T> &operator/=(QuaternionT<T> &a, const T &b)
    {
      return a = a * rcp(b);
    }
    template <typename T>
    inline QuaternionT<T> &operator/=(QuaternionT<T> &a,
                                      const QuaternionT<T> &b)
    {
      return a = a * rcp(b);
    }

    template <typename T>
    inline typename QuaternionT<T>::Vector xfmPoint(
        const QuaternionT<T> &a, const typename QuaternionT<T>::Vector &b)
    {
      return (a * QuaternionT<T>(b) * conj(a)).v();
    }

    template <typename T>
    inline typename QuaternionT<T>::Vector xfmQuaternion(
        const QuaternionT<T> &a, const typename QuaternionT<T>::Vector &b)
    {
      return (a * QuaternionT<T>(b) * conj(a)).v();
    }

    template <typename T>
    inline typename QuaternionT<T>::Vector xfmNormal(
        const QuaternionT<T> &a, const typename QuaternionT<T>::Vector &b)
    {
      return (a * QuaternionT<T>(b) * conj(a)).v();
    }

    template <typename T>
    inline bool operator==(const QuaternionT<T> &a, const QuaternionT<T> &b)
    {
      return a.r == b.r && a.i == b.i && a.j == b.j && a.k == b.k;
    }

    template <typename T>
    inline bool operator!=(const QuaternionT<T> &a, const QuaternionT<T> &b)
    {
      return a.r != b.r || a.i != b.i || a.j != b.j || a.k != b.k;
    }

    template <typename T>
    QuaternionT<T>::QuaternionT(const typename QuaternionT<T>::Vector &vx,
                                const typename QuaternionT<T>::Vector &vy,
                                const typename QuaternionT<T>::Vector &vz)
    {
      if (vx.x + vy.y + vz.z >= T(zero)) {
        const T t = T(one) + (vx.x + vy.y + vz.z);
        const T s = rsqrt(t) * T(0.5f);
        r         = t * s;
        i         = (vy.z - vz.y) * s;
        j         = (vz.x - vx.z) * s;
        k         = (vx.y - vy.x) * s;
      } else if (vx.x >= max(vy.y, vz.z)) {
        const T t = (T(one) + vx.x) - (vy.y + vz.z);
        const T s = rsqrt(t) * T(0.5f);
        r         = (vy.z - vz.y) * s;
        i         = t * s;
        j         = (vx.y + vy.x) * s;
        k         = (vz.x + vx.z) * s;
      } else if (vy.y >= vz.z)  // if ( vy.y >= max(vz.z, vx.x) )
      {
        const T t = (T(one) + vy.y) - (vz.z + vx.x);
        const T s = rsqrt(t) * T(0.5f);
        r         = (vz.x - vx.z) * s;
        i         = (vx.y + vy.x) * s;
        j         = t * s;
        k         = (vy.z + vz.y) * s;
      } else  // if ( vz.z >= max(vy.y, vx.x) )
      {
        const T t = (T(one) + vz.z) - (vx.x + vy.y);
        const T s = rsqrt(t) * T(0.5f);
        r         = (vx.y - vy.x) * s;
        i         = (vz.x + vx.z) * s;
        j         = (vy.z + vz.y) * s;
        k         = t * s;
      }
    }

    template <typename T>
    QuaternionT<T>::QuaternionT(const T &yaw, const T &pitch, const T &roll)
    {
      const T cya = cos(yaw * T(0.5f));
      const T cpi = cos(pitch * T(0.5f));
      const T cro = cos(roll * T(0.5f));
      const T sya = sin(yaw * T(0.5f));
      const T spi = sin(pitch * T(0.5f));
      const T sro = sin(roll * T(0.5f));
      r           = cro * cya * cpi + sro * sya * spi;
      i           = cro * cya * spi + sro * sya * cpi;
      j           = cro * sya * cpi - sro * cya * spi;
      k           = sro * cya * cpi - cro * sya * spi;
    }

    template <typename T>
    static std::ostream &operator<<(std::ostream &cout, const QuaternionT<T> &q)
    {
      return cout << "{ r = " << q.r << ", i = " << q.i << ", j = " << q.j
                  << ", k = " << q.k << " }";
    }

    template <typename T>
    // a, b must be normalized
    inline QuaternionT<T> slerp(const float factor, const QuaternionT<T> &_a, const QuaternionT<T> &b)
    {
      QuaternionT<T> a(_a);
      float d = dot(a, b);
      if (d < 0.f) { // prevent "long way around"
        a = -a;
        d = -d;
      }
      if (d > 0.9995) { // angles too small, fallback to linear interpolation
        return normalize(rkcommon::math::lerp(factor, a, b));
      }

      const float theta0 = std::acos(d);
      const float theta = theta0 * factor;
      const float fb = std::sin(theta) / std::sin(theta0);
      const float fa = std::cos(theta) - d * fb;

      return fa * a + fb * b;
    }

    using quaternionf = QuaternionT<float>;
    using quatf = QuaternionT<float>;
    using quaterniond = QuaternionT<double>;
    using quatd = QuaternionT<double>;

  }  // namespace math
}  // namespace rkcommon
