// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include "../traits/rktraits.h"

namespace rkcommon {
  namespace utility {

    /* Execute a given function when a scope exits */
    struct OnScopeExit
    {
      template <typename FCN_T>
      OnScopeExit(FCN_T &&_fcn);

      ~OnScopeExit();

     private:
      std::function<void()> fcn;
    };

    // Inlined OnScopeExit definitions ////////////////////////////////////////

    template <typename FCN_T>
    inline OnScopeExit::OnScopeExit(FCN_T &&_fcn)
    {
      static_assert(traits::has_operator_method<FCN_T>::value,
                    "FCN_T must implement operator() with no arguments!");

      fcn = std::forward<FCN_T>(_fcn);
    }

    inline OnScopeExit::~OnScopeExit()
    {
      fcn();
    }

  }  // namespace utility
}  // namespace rkcommon
