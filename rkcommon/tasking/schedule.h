// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../traits/rktraits.h"
#include "detail/schedule.inl"

namespace rkcommon {
  namespace tasking {

    // NOTE(jda) - This abstraction takes a lambda which should take captured
    //             variables by *value* to ensure no captured references race
    //             with the task itself.

    // NOTE(jda) - No priority is associated with this call, but could be added
    //             later with a hint enum, using a default value for the
    //             priority to not require specifying it.
    template <typename TASK_T>
    inline void schedule(TASK_T fcn)
    {
      static_assert(traits::has_operator_method<TASK_T>::value,
                    "rkcommon::tasking::schedule() requires the "
                    "implementation of method 'void TASK_T::operator()'.");

      detail::schedule_impl(std::move(fcn));
    }

  }  // namespace tasking
}  // namespace rkcommon
