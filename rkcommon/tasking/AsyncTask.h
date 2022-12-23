// Copyright 2009 Intel Corporation
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
      // declaration before taskImpl: ensure initialization before task finishes
      std::atomic<bool> jobFinished{false};
      detail::AsyncTaskImpl<std::function<void()>> taskImpl;
      T retValue;
    };

  }  // namespace tasking
}  // namespace rkcommon
