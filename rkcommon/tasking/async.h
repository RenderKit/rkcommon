// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <functional>
#include <future>

#include "schedule.h"

namespace rkcommon {
  namespace tasking {

    template <typename TASK_T>
    using operator_return_t = typename std::result_of<TASK_T()>::type;

    // NOTE(jda) - This abstraction takes a lambda which should take captured
    //             variables by *value* to ensure no captured references race
    //             with the task itself.
    template <typename TASK_T>
    inline auto async(TASK_T &&fcn) -> std::future<operator_return_t<TASK_T>>
    {
      static_assert(traits::has_operator_method<TASK_T>::value,
                    "rkcommon::tasking::async() requires the implementation of"
                    "method 'RETURN_T TASK_T::operator()', where RETURN_T "
                    "is the return value of the passed in task.");

      using package_t = std::packaged_task<operator_return_t<TASK_T>()>;

      auto task   = new package_t(std::forward<TASK_T>(fcn));
      auto future = task->get_future();

      schedule([=]() {
        (*task)();
        delete task;
      });

      return future;
    }

  }  // namespace tasking
}  // namespace rkcommon
