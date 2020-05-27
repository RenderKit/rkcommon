// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// rkcommon
#include "platform.h"
// std
#include <string>

#ifdef _WIN32
// ----------- windows only -----------
typedef unsigned long long id_t;
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#ifdef _M_X64
typedef long long ssize_t;
#else
typedef int ssize_t;
#endif
#else
// ----------- NOT windows -----------
#include "unistd.h"
#endif

#ifdef _WIN32
#ifdef rkcommon_EXPORTS
#define RKCOMMON_INTERFACE __declspec(dllexport)
#else
#define RKCOMMON_INTERFACE __declspec(dllimport)
#endif
#else
#define RKCOMMON_INTERFACE
#endif

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace rkcommon {

  using byte_t = unsigned char;

  /*! remove specified num arguments from an ac/av arglist */
  RKCOMMON_INTERFACE void removeArgs(int &ac,
                                      const char **&av,
                                      int where,
                                      int howMany);

  RKCOMMON_INTERFACE void loadLibrary(const std::string &name,
                                       bool anchor = false);

  RKCOMMON_INTERFACE void unloadLibrary(const std::string &name);

  RKCOMMON_INTERFACE void loadDefaultLibrary();

  RKCOMMON_INTERFACE void *getSymbol(const std::string &name);

  RKCOMMON_INTERFACE std::string prettyDouble(double x);

  RKCOMMON_INTERFACE std::string prettyNumber(size_t x);

  // NOTE(jda) - Implement make_unique() as it didn't show up until C++14...
  template <typename T, typename... Args>
  inline std::unique_ptr<T> make_unique(Args &&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

}  // namespace rkcommon
