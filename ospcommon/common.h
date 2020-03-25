// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// ospcommon
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
#ifdef ospcommon_EXPORTS
#define OSPCOMMON_INTERFACE __declspec(dllexport)
#else
#define OSPCOMMON_INTERFACE __declspec(dllimport)
#endif
#else
#define OSPCOMMON_INTERFACE
#endif

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

namespace ospcommon {

  using byte_t = unsigned char;

  /*! remove specified num arguments from an ac/av arglist */
  OSPCOMMON_INTERFACE void removeArgs(int &ac,
                                      const char **&av,
                                      int where,
                                      int howMany);

  OSPCOMMON_INTERFACE void loadLibrary(const std::string &name,
                                       bool anchor = false);

  OSPCOMMON_INTERFACE void unloadLibrary(const std::string &name);

  OSPCOMMON_INTERFACE void loadDefaultLibrary();

  OSPCOMMON_INTERFACE void *getSymbol(const std::string &name);

  OSPCOMMON_INTERFACE std::string prettyDouble(double x);

  OSPCOMMON_INTERFACE std::string prettyNumber(size_t x);

  // NOTE(jda) - Implement make_unique() as it didn't show up until C++14...
  template <typename T, typename... Args>
  inline std::unique_ptr<T> make_unique(Args &&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

}  // namespace ospcommon
