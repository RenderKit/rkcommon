// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "demangle.h"

#ifdef __GNUG__
#include <cxxabi.h>
#include <cstdlib>
#include <memory>
#endif

namespace rkcommon {
  namespace utility {

#ifdef __GNUG__
    std::string demangle(const char *name)
    {
      int status = 0;

      std::unique_ptr<char, void (*)(void *)> res{
          abi::__cxa_demangle(name, NULL, NULL, &status), std::free};

      return (status == 0) ? res.get() : name;
    }
#else
    std::string demangle(const char *name)
    {
      return name;
    }
#endif

  }  // namespace utility
}  // namespace rkcommon