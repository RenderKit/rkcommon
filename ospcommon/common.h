// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

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
