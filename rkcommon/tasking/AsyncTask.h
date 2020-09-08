// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "detail/async_task.inl"

#include <atomic>
#include <functional>

namespace rkcommon {
  namespace tasking {

    template <typename T>
    struct AsyncTask
    {
      AsyncTask(std::function<T()> fcn)
          : taskImpl([this, fcn]() {
              retValue    = fcn();
              jobFinished = true;
            })
      {
      }

      virtual ~AsyncTask() noexcept
      {
        wait();
      }

      bool finished() const
      {
        return jobFinished;
      }
      bool valid() const
      {
        return jobFinished;
      }
      void wait()
      {
        taskImpl.wait();
      }

      T get()
      {
        if (!jobFinished)
          wait();
        return retValue;
      }

     private:
      detail::AsyncTaskImpl<std::function<void()>> taskImpl;
      std::atomic<bool> jobFinished{false};
      T retValue;
    };

  }  // namespace tasking
}  // namespace rkcommon
