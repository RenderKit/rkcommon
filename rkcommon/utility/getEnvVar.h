// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "Optional.h"

namespace rkcommon {
  namespace utility {

    template <typename T>
    inline Optional<T> getEnvVar(const std::string & /*var*/)
    {
      static_assert(!std::is_same<T, float>::value &&
                        !std::is_same<T, int>::value &&
                        !std::is_same<T, std::string>::value,
                    "You can only get an int, float, or std::string "
                    "when using ospray::getEnvVar<T>()!");
      return {};
    }

    template <>
    inline Optional<float> getEnvVar<float>(const std::string &var)
    {
      auto *str  = getenv(var.c_str());
      bool found = (str != nullptr);
      return found ? Optional<float>((float)atof(str)) : Optional<float>();
    }

    template <>
    inline Optional<int> getEnvVar<int>(const std::string &var)
    {
      auto *str  = getenv(var.c_str());
      bool found = (str != nullptr);
      return found ? Optional<int>(atoi(str)) : Optional<int>();
    }

    template <>
    inline Optional<std::string> getEnvVar<std::string>(const std::string &var)
    {
      auto *str  = getenv(var.c_str());
      bool found = (str != nullptr);
      return found ? Optional<std::string>(std::string(str))
                   : Optional<std::string>();
    }

  }  // namespace utility
}  // namespace rkcommon
